/*
 * Copyright (C) 2006, 2007, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "SubresourceLoader.h"

#include "CachedResourceLoader.h"
#include "Document.h"
#include "DocumentLoader.h"
#include "Frame.h"
#include "Logging.h"
#include "MemoryCache.h"
#include "ResourceBuffer.h"
#include "WebCoreMemoryInstrumentation.h"
#include <wtf/RefCountedLeakCounter.h>
#include <wtf/StdLibExtras.h>
#include <wtf/text/CString.h>

namespace WebCore {

DEFINE_DEBUG_ONLY_GLOBAL(WTF::RefCountedLeakCounter, subresourceLoaderCounter, ("SubresourceLoader"));

SubresourceLoader::RequestCountTracker::RequestCountTracker(CachedResourceLoader* cachedResourceLoader, CachedResource* resource)
    : m_cachedResourceLoader(cachedResourceLoader)
    , m_resource(resource)
{
    m_cachedResourceLoader->incrementRequestCount(m_resource);
}

SubresourceLoader::RequestCountTracker::~RequestCountTracker()
{
    m_cachedResourceLoader->decrementRequestCount(m_resource);
}

SubresourceLoader::SubresourceLoader(Frame* frame, CachedResource* resource, const ResourceLoaderOptions& options)
    : ResourceLoader(frame, options)
    , m_resource(resource)
    , m_loadingMultipartContent(false)
    , m_state(Uninitialized)
    , m_requestCountTracker(adoptPtr(new RequestCountTracker(frame->document()->cachedResourceLoader(), resource)))
{
#ifndef NDEBUG
    subresourceLoaderCounter.increment();
#endif
}

SubresourceLoader::~SubresourceLoader()
{
    ASSERT(m_state != Initialized);
    ASSERT(reachedTerminalState());
#ifndef NDEBUG
    subresourceLoaderCounter.decrement();
#endif
}

PassRefPtr<SubresourceLoader> SubresourceLoader::create(Frame* frame, CachedResource* resource, const ResourceRequest& request, const ResourceLoaderOptions& options)
{
    RefPtr<SubresourceLoader> subloader(adoptRef(new SubresourceLoader(frame, resource, options)));
    if (!subloader->init(request))
        return 0;
    return subloader.release();
}

CachedResource* SubresourceLoader::cachedResource()
{
    return m_resource;
}

void SubresourceLoader::cancelIfNotFinishing()
{
    if (m_state != Initialized)
        return;

    ResourceLoader::cancel();
}

void SubresourceLoader::reportMemoryUsage(MemoryObjectInfo* memoryObjectInfo) const
{
    MemoryClassInfo info(memoryObjectInfo, this, WebCoreMemoryTypes::Loader);
    ResourceLoader::reportMemoryUsage(memoryObjectInfo);
    info.addMember(m_resource);
    info.addMember(m_documentLoader);
    info.addMember(m_requestCountTracker);
}

bool SubresourceLoader::init(const ResourceRequest& request)
{
    if (!ResourceLoader::init(request))
        return false;

    ASSERT(!reachedTerminalState());
    m_state = Initialized;
    m_documentLoader->addSubresourceLoader(this);
    return true;
}

bool SubresourceLoader::isSubresourceLoader()
{
    return true;
}

void SubresourceLoader::willSendRequest(ResourceRequest& newRequest, const ResourceResponse& redirectResponse)
{
    // Store the previous URL because the call to ResourceLoader::willSendRequest will modify it.
    KURL previousURL = request().url();
    RefPtr<SubresourceLoader> protect(this);

    ASSERT(!newRequest.isNull());
    if (!previousURL.isNull() && previousURL != newRequest.url()) {
        if (!m_documentLoader->cachedResourceLoader()->canRequest(m_resource->type(), newRequest.url())) {
            cancel();
            return;
        }
        if (m_resource->type() == CachedResource::ImageResource && m_documentLoader->cachedResourceLoader()->shouldDeferImageLoad(newRequest.url())) {
            cancel();
            return;
        }
        m_resource->willSendRequest(newRequest, redirectResponse);
    }

    if (newRequest.isNull() || reachedTerminalState())
        return;

    ResourceLoader::willSendRequest(newRequest, redirectResponse);
    if (newRequest.isNull())
        cancel();
}

void SubresourceLoader::didSendData(unsigned long long bytesSent, unsigned long long totalBytesToBeSent)
{
    ASSERT(m_state == Initialized);
    RefPtr<SubresourceLoader> protect(this);
    m_resource->didSendData(bytesSent, totalBytesToBeSent);
}

void SubresourceLoader::didReceiveResponse(const ResourceResponse& response)
{
    ASSERT(!response.isNull());
    ASSERT(m_state == Initialized);

    // Reference the object in this method since the additional processing can do
    // anything including removing the last reference to this object; one example of this is 3266216.
    RefPtr<SubresourceLoader> protect(this);

    if (m_resource->resourceToRevalidate()) {
        if (response.httpStatusCode() == 304) {
            // 304 Not modified / Use local copy
            // Existing resource is ok, just use it updating the expiration time.
            memoryCache()->revalidationSucceeded(m_resource, response);
            if (!reachedTerminalState())
                ResourceLoader::didReceiveResponse(response);
            return;
        }
        // Did not get 304 response, continue as a regular resource load.
        memoryCache()->revalidationFailed(m_resource);
    }

    m_resource->setResponse(response);
    if (reachedTerminalState())
        return;
    ResourceLoader::didReceiveResponse(response);

    // FIXME: Main resources have a different set of rules for multipart than images do.
    // Hopefully we can merge those 2 paths.
    if (response.isMultipart() && m_resource->type() != CachedResource::MainResource) {
        m_loadingMultipartContent = true;

        // We don't count multiParts in a CachedResourceLoader's request count
        m_requestCountTracker.clear();
        if (!m_resource->isImage()) {
            cancel();
            return;
        }
    }

    RefPtr<ResourceBuffer> buffer = resourceData();
    if (m_loadingMultipartContent && buffer && buffer->size()) {
        sendDataToResource(buffer->data(), buffer->size());
        clearResourceData();
        // Since a subresource loader does not load multipart sections progressively, data was delivered to the loader all at once.        
        // After the first multipart section is complete, signal to delegates that this load is "finished" 
        m_documentLoader->subresourceLoaderFinishedLoadingOnePart(this);
        didFinishLoadingOnePart(0);
    }

    checkForHTTPStatusCodeError();
}

void SubresourceLoader::didReceiveData(const char* data, int length, long long encodedDataLength, bool allAtOnce)
{
    ASSERT(!m_resource->resourceToRevalidate());
    ASSERT(!m_resource->errorOccurred());
    ASSERT(m_state == Initialized);
    // Reference the object in this method since the additional processing can do
    // anything including removing the last reference to this object; one example of this is 3266216.
    RefPtr<SubresourceLoader> protect(this);
    addData(data, length, allAtOnce);
    if (!m_loadingMultipartContent)
        sendDataToResource(data, length);
    if (shouldSendResourceLoadCallbacks() && m_frame)
        frameLoader()->notifier()->didReceiveData(this, data, length, static_cast<int>(encodedDataLength));
}

bool SubresourceLoader::checkForHTTPStatusCodeError()
{
    if (m_resource->response().httpStatusCode() < 400 || m_resource->shouldIgnoreHTTPStatusCodeErrors())
        return false;

    m_resource->error(CachedResource::LoadError);
    m_state = Finishing;
    cancel();
    return true;
}

void SubresourceLoader::sendDataToResource(const char* data, int length)
{
    // There are two cases where we might need to create our own SharedBuffer instead of copying the one in ResourceLoader. 
    // (1) Multipart content: The loader delivers the data in a multipart section all at once, then sends eof. 
    //     The resource data will change as the next part is loaded, so we need to make a copy. 
    // (2) Our client requested that the data not be buffered at the ResourceLoader level via ResourceLoaderOptions. In this case, 
    //     ResourceLoader::resourceData() will be null. However, unlike the multipart case, we don't want to tell the CachedResource 
    //     that all data has been received yet. 
    if (m_loadingMultipartContent || !resourceData()) { 
        RefPtr<ResourceBuffer> copiedData = ResourceBuffer::create(data, length); 
        m_resource->data(copiedData.release(), m_loadingMultipartContent);
    } else 
        m_resource->data(resourceData(), false);
}

void SubresourceLoader::didReceiveCachedMetadata(const char* data, int length)
{
    ASSERT(m_state == Initialized);
    ASSERT(!m_resource->resourceToRevalidate());
    m_resource->setSerializedCachedMetadata(data, length);
}

void SubresourceLoader::didFinishLoading(double finishTime)
{
    if (m_state != Initialized)
        return;
    ASSERT(!reachedTerminalState());
    ASSERT(!m_resource->resourceToRevalidate());
    ASSERT(!m_resource->errorOccurred());
    LOG(ResourceLoading, "Received '%s'.", m_resource->url().string().latin1().data());

    RefPtr<SubresourceLoader> protect(this);
    CachedResourceHandle<CachedResource> protectResource(m_resource);
    m_state = Finishing;
    m_resource->setLoadFinishTime(finishTime);
    m_resource->data(resourceData(), true);
    m_resource->finish();
    ResourceLoader::didFinishLoading(finishTime);
}

void SubresourceLoader::didFail(const ResourceError& error)
{
    if (m_state != Initialized)
        return;
    ASSERT(!reachedTerminalState());
    ASSERT(!m_resource->resourceToRevalidate());
    LOG(ResourceLoading, "Failed to load '%s'.\n", m_resource->url().string().latin1().data());

    RefPtr<SubresourceLoader> protect(this);
    CachedResourceHandle<CachedResource> protectResource(m_resource);
    m_state = Finishing;
    m_resource->setResourceError(error);
    m_resource->error(CachedResource::LoadError);
    if (!m_resource->isPreloaded())
        memoryCache()->remove(m_resource);
    ResourceLoader::didFail(error);
}

void SubresourceLoader::willCancel(const ResourceError& error)
{
    if (m_state != Initialized)
        return;
    ASSERT(!reachedTerminalState());
    LOG(ResourceLoading, "Cancelled load of '%s'.\n", m_resource->url().string().latin1().data());

    RefPtr<SubresourceLoader> protect(this);
    m_state = Finishing;
    if (m_resource->resourceToRevalidate())
        memoryCache()->revalidationFailed(m_resource);
    m_resource->setResourceError(error);
    memoryCache()->remove(m_resource);
}

void SubresourceLoader::releaseResources()
{
    ASSERT(!reachedTerminalState());
    if (m_state != Uninitialized) {
        m_requestCountTracker.clear();
        m_documentLoader->cachedResourceLoader()->loadDone(m_resource);
        if (reachedTerminalState())
            return;
        m_resource->stopLoading();
        m_documentLoader->removeSubresourceLoader(this);
    }
    ResourceLoader::releaseResources();
}

}
