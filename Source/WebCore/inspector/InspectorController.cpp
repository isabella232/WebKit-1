/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(INSPECTOR)

#include "InspectorController.h"

#include "Frame.h"
#include "GraphicsContext.h"
#include "IdentifiersFactory.h"
#include "InjectedScriptHost.h"
#include "InjectedScriptManager.h"
#include "InspectorAgent.h"
#include "InspectorApplicationCacheAgent.h"
#include "InspectorBackendDispatcher.h"
#include "InspectorBaseAgent.h"
#include "InspectorCSSAgent.h"
#include "InspectorCanvasAgent.h"
#include "InspectorClient.h"
#include "InspectorDOMAgent.h"
#include "InspectorDOMDebuggerAgent.h"
#include "InspectorDOMStorageAgent.h"
#include "InspectorDatabaseAgent.h"
#include "InspectorDebuggerAgent.h"
#include "InspectorFileSystemAgent.h"
#include "InspectorFrontend.h"
#include "InspectorFrontendClient.h"
#include "InspectorIndexedDBAgent.h"
#include "InspectorInputAgent.h"
#include "InspectorInstrumentation.h"
#include "InspectorLayerTreeAgent.h"
#include "InspectorMemoryAgent.h"
#include "InspectorOverlay.h"
#include "InspectorPageAgent.h"
#include "InspectorProfilerAgent.h"
#include "InspectorResourceAgent.h"
#include "InspectorState.h"
#include "InspectorTimelineAgent.h"
#include "InspectorWorkerAgent.h"
#include "InstrumentingAgents.h"
#include "PageConsoleAgent.h"
#include "PageDebuggerAgent.h"
#include "PageRuntimeAgent.h"
#include "Page.h"
#include "ScriptObject.h"
#include "Settings.h"
#include "WebCoreMemoryInstrumentation.h"
#include <wtf/MemoryInstrumentationVector.h>
#include <wtf/UnusedParam.h>

namespace WebCore {

InspectorController::InspectorController(Page* page, InspectorClient* inspectorClient)
    : m_instrumentingAgents(adoptPtr(new InstrumentingAgents()))
    , m_injectedScriptManager(InjectedScriptManager::createForPage())
    , m_state(adoptPtr(new InspectorState(inspectorClient)))
    , m_overlay(InspectorOverlay::create(page, inspectorClient))
    , m_page(page)
    , m_inspectorClient(inspectorClient)
{
    OwnPtr<InspectorAgent> inspectorAgentPtr(InspectorAgent::create(page, m_injectedScriptManager.get(), m_instrumentingAgents.get(), m_state.get()));
    m_inspectorAgent = inspectorAgentPtr.get();
    m_agents.append(inspectorAgentPtr.release());

    OwnPtr<InspectorPageAgent> pageAgentPtr(InspectorPageAgent::create(m_instrumentingAgents.get(), page, m_inspectorAgent, m_state.get(), m_injectedScriptManager.get(), inspectorClient, m_overlay.get()));
    InspectorPageAgent* pageAgent = pageAgentPtr.get();
    m_pageAgent = pageAgentPtr.get();
    m_agents.append(pageAgentPtr.release());

    OwnPtr<InspectorDOMAgent> domAgentPtr(InspectorDOMAgent::create(m_instrumentingAgents.get(), pageAgent, m_state.get(), m_injectedScriptManager.get(), m_overlay.get()));
    m_domAgent = domAgentPtr.get();
    m_agents.append(domAgentPtr.release());

    m_agents.append(InspectorCSSAgent::create(m_instrumentingAgents.get(), m_state.get(), m_domAgent));

#if ENABLE(SQL_DATABASE)
    OwnPtr<InspectorDatabaseAgent> databaseAgentPtr(InspectorDatabaseAgent::create(m_instrumentingAgents.get(), m_state.get()));
    InspectorDatabaseAgent* databaseAgent = databaseAgentPtr.get();
    m_agents.append(databaseAgentPtr.release());
#endif

#if ENABLE(INDEXED_DATABASE)
    m_agents.append(InspectorIndexedDBAgent::create(m_instrumentingAgents.get(), m_state.get(), m_injectedScriptManager.get(), pageAgent));
#endif

#if ENABLE(FILE_SYSTEM)
    m_agents.append(InspectorFileSystemAgent::create(m_instrumentingAgents.get(), pageAgent, m_state.get()));
#endif
    OwnPtr<InspectorDOMStorageAgent> domStorageAgentPtr(InspectorDOMStorageAgent::create(m_instrumentingAgents.get(), m_state.get()));
    InspectorDOMStorageAgent* domStorageAgent = domStorageAgentPtr.get();
    m_agents.append(domStorageAgentPtr.release());
    m_agents.append(InspectorMemoryAgent::create(m_instrumentingAgents.get(), inspectorClient, m_state.get(), m_page));
    m_agents.append(InspectorTimelineAgent::create(m_instrumentingAgents.get(), pageAgent, m_state.get(), InspectorTimelineAgent::PageInspector,
       inspectorClient));
    m_agents.append(InspectorApplicationCacheAgent::create(m_instrumentingAgents.get(), m_state.get(), pageAgent));

    OwnPtr<InspectorResourceAgent> resourceAgentPtr(InspectorResourceAgent::create(m_instrumentingAgents.get(), pageAgent, inspectorClient, m_state.get()));
    m_resourceAgent = resourceAgentPtr.get();
    m_agents.append(resourceAgentPtr.release());

    OwnPtr<InspectorRuntimeAgent> runtimeAgentPtr(PageRuntimeAgent::create(m_instrumentingAgents.get(), m_state.get(), m_injectedScriptManager.get(), page, pageAgent));
    InspectorRuntimeAgent* runtimeAgent = runtimeAgentPtr.get();
    m_agents.append(runtimeAgentPtr.release());

    OwnPtr<InspectorConsoleAgent> consoleAgentPtr(PageConsoleAgent::create(m_instrumentingAgents.get(), m_inspectorAgent, m_state.get(), m_injectedScriptManager.get(), m_domAgent));
    InspectorConsoleAgent* consoleAgent = consoleAgentPtr.get();
    m_agents.append(consoleAgentPtr.release());

#if ENABLE(JAVASCRIPT_DEBUGGER)
    OwnPtr<InspectorDebuggerAgent> debuggerAgentPtr(PageDebuggerAgent::create(m_instrumentingAgents.get(), m_state.get(), pageAgent, m_injectedScriptManager.get(), m_overlay.get()));
    m_debuggerAgent = debuggerAgentPtr.get();
    m_agents.append(debuggerAgentPtr.release());

    m_agents.append(InspectorDOMDebuggerAgent::create(m_instrumentingAgents.get(), m_state.get(), m_domAgent, m_debuggerAgent, m_inspectorAgent));

    OwnPtr<InspectorProfilerAgent> profilerAgentPtr(InspectorProfilerAgent::create(m_instrumentingAgents.get(), consoleAgent, page, m_state.get(), m_injectedScriptManager.get()));
    m_profilerAgent = profilerAgentPtr.get();
    m_agents.append(profilerAgentPtr.release());
#endif

#if ENABLE(WORKERS)
    m_agents.append(InspectorWorkerAgent::create(m_instrumentingAgents.get(), m_state.get()));
#endif

    m_agents.append(InspectorCanvasAgent::create(m_instrumentingAgents.get(), m_state.get(), page, m_injectedScriptManager.get()));

    m_agents.append(InspectorInputAgent::create(m_instrumentingAgents.get(), m_state.get(), page));

    m_agents.append(InspectorLayerTreeAgent::create(m_instrumentingAgents.get(), m_state.get(), page));

    ASSERT_ARG(inspectorClient, inspectorClient);
    m_injectedScriptManager->injectedScriptHost()->init(m_inspectorAgent
        , consoleAgent
#if ENABLE(SQL_DATABASE)
        , databaseAgent
#endif
        , domStorageAgent
        , m_domAgent
        , m_debuggerAgent
    );

#if ENABLE(JAVASCRIPT_DEBUGGER)
    runtimeAgent->setScriptDebugServer(&m_debuggerAgent->scriptDebugServer());
#endif

    InspectorInstrumentation::registerInstrumentingAgents(m_instrumentingAgents.get());
}

InspectorController::~InspectorController()
{
    m_agents.discardAgents();
    ASSERT(!m_inspectorClient);
}

PassOwnPtr<InspectorController> InspectorController::create(Page* page, InspectorClient* client)
{
    return adoptPtr(new InspectorController(page, client));
}

void InspectorController::inspectedPageDestroyed()
{
    disconnectFrontend();
    InspectorInstrumentation::unregisterInstrumentingAgents(m_instrumentingAgents.get());
    m_injectedScriptManager->disconnect();
    m_inspectorClient->inspectorDestroyed();
    m_inspectorClient = 0;
    m_page = 0;
}

void InspectorController::setInspectorFrontendClient(PassOwnPtr<InspectorFrontendClient> inspectorFrontendClient)
{
    m_inspectorFrontendClient = inspectorFrontendClient;
}

bool InspectorController::hasInspectorFrontendClient() const
{
    return m_inspectorFrontendClient;
}

void InspectorController::didClearWindowObjectInWorld(Frame* frame, DOMWrapperWorld* world)
{
    if (world != mainThreadNormalWorld())
        return;

    // If the page is supposed to serve as InspectorFrontend notify inspector frontend
    // client that it's cleared so that the client can expose inspector bindings.
    if (m_inspectorFrontendClient && frame == m_page->mainFrame())
        m_inspectorFrontendClient->windowObjectCleared();
}

void InspectorController::connectFrontend(InspectorFrontendChannel* frontendChannel)
{
    ASSERT(frontendChannel);

    m_inspectorFrontend = adoptPtr(new InspectorFrontend(frontendChannel));
    // We can reconnect to existing front-end -> unmute state.
    m_state->unmute();

    m_agents.setFrontend(m_inspectorFrontend.get());

    InspectorInstrumentation::frontendCreated();

    ASSERT(m_inspectorClient);
    m_inspectorBackendDispatcher = InspectorBackendDispatcher::create(frontendChannel);

    m_agents.registerInDispatcher(m_inspectorBackendDispatcher.get());
}

void InspectorController::disconnectFrontend()
{
    if (!m_inspectorFrontend)
        return;
    m_inspectorBackendDispatcher->clearFrontend();
    m_inspectorBackendDispatcher.clear();

    // Destroying agents would change the state, but we don't want that.
    // Pre-disconnect state will be used to restore inspector agents.
    m_state->mute();

    m_agents.clearFrontend();

    m_inspectorFrontend.clear();

    // relese overlay page resources
    m_overlay->freePage();
    InspectorInstrumentation::frontendDeleted();
}

void InspectorController::show()
{
    if (!enabled())
        return;

    if (m_inspectorFrontend)
        m_inspectorClient->bringFrontendToFront();
    else {
        InspectorFrontendChannel* frontendChannel = m_inspectorClient->openInspectorFrontend(this);
        if (frontendChannel)
            connectFrontend(frontendChannel);
    }
}

void InspectorController::close()
{
    if (!m_inspectorFrontend)
        return;
    disconnectFrontend();
    m_inspectorClient->closeInspectorFrontend();
}

void InspectorController::reconnectFrontend(InspectorFrontendChannel* frontendChannel, const String& inspectorStateCookie)
{
    ASSERT(!m_inspectorFrontend);
    connectFrontend(frontendChannel);
    m_state->loadFromCookie(inspectorStateCookie);
    m_agents.restore();
}

void InspectorController::setProcessId(long processId)
{
    IdentifiersFactory::setProcessId(processId);
}

void InspectorController::webViewResized(const IntSize& size)
{
    m_pageAgent->webViewResized(size);
}

void InspectorController::evaluateForTestInFrontend(long callId, const String& script)
{
    m_inspectorAgent->evaluateForTestInFrontend(callId, script);
}

void InspectorController::drawHighlight(GraphicsContext& context) const
{
    m_overlay->paint(context);
}

void InspectorController::getHighlight(Highlight* highlight) const
{
    m_overlay->getHighlight(highlight);
}

void InspectorController::inspect(Node* node)
{
    if (!enabled())
        return;

    show();

    m_domAgent->inspect(node);
}

bool InspectorController::enabled() const
{
    return m_inspectorAgent->developerExtrasEnabled();
}

Page* InspectorController::inspectedPage() const
{
    return m_page;
}

void InspectorController::setInjectedScriptForOrigin(const String& origin, const String& source)
{
    m_inspectorAgent->setInjectedScriptForOrigin(origin, source);
}

void InspectorController::dispatchMessageFromFrontend(const String& message)
{
    if (m_inspectorBackendDispatcher)
        m_inspectorBackendDispatcher->dispatch(message);
}

void InspectorController::hideHighlight()
{
    ErrorString error;
    m_domAgent->hideHighlight(&error);
}

Node* InspectorController::highlightedNode() const
{
    return m_overlay->highlightedNode();
}

#if ENABLE(JAVASCRIPT_DEBUGGER)
bool InspectorController::profilerEnabled()
{
    return m_profilerAgent->enabled();
}

void InspectorController::setProfilerEnabled(bool enable)
{
    ErrorString error;
    if (enable)
        m_profilerAgent->enable(&error);
    else
        m_profilerAgent->disable(&error);
}

void InspectorController::resume()
{
    if (m_debuggerAgent) {
        ErrorString error;
        m_debuggerAgent->resume(&error);
    }
}
#endif

void InspectorController::setResourcesDataSizeLimitsFromInternals(int maximumResourcesContentSize, int maximumSingleResourceContentSize)
{
    m_resourceAgent->setResourcesDataSizeLimitsFromInternals(maximumResourcesContentSize, maximumSingleResourceContentSize);
}

void InspectorController::reportMemoryUsage(MemoryObjectInfo* memoryObjectInfo) const
{
    MemoryClassInfo info(memoryObjectInfo, this, WebCoreMemoryTypes::InspectorController);
    info.addMember(m_inspectorAgent);
    info.addMember(m_instrumentingAgents);
    info.addMember(m_injectedScriptManager);
    info.addMember(m_state);
    info.addMember(m_overlay);

    info.addMember(m_inspectorAgent);
    info.addMember(m_domAgent);
    info.addMember(m_resourceAgent);
    info.addMember(m_pageAgent);
#if ENABLE(JAVASCRIPT_DEBUGGER)
    info.addMember(m_debuggerAgent);
    info.addMember(m_profilerAgent);
#endif

    info.addMember(m_inspectorBackendDispatcher);
    info.addMember(m_inspectorFrontendClient);
    info.addMember(m_inspectorFrontend);
    info.addMember(m_page);
    info.addWeakPointer(m_inspectorClient);
    info.addMember(m_agents);
}

void InspectorController::willProcessTask()
{
    if (InspectorTimelineAgent* timelineAgent = m_instrumentingAgents->inspectorTimelineAgent())
        timelineAgent->willProcessTask();
}

void InspectorController::didProcessTask()
{
    if (InspectorTimelineAgent* timelineAgent = m_instrumentingAgents->inspectorTimelineAgent())
        timelineAgent->didProcessTask();
}

} // namespace WebCore

#endif // ENABLE(INSPECTOR)
