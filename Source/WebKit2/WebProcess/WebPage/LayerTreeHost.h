/*
 * Copyright (C) 2011, 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LayerTreeHost_h
#define LayerTreeHost_h

#include "LayerTreeContext.h"
#include <WebCore/Color.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>

namespace CoreIPC {
class Connection;
class MessageDecoder;
class MessageID;
}

namespace WebCore {
class FloatPoint;
class FloatRect;
class IntRect;
class IntSize;
class GraphicsLayer;
class GraphicsLayerFactory;

#if PLATFORM(WIN) && USE(AVFOUNDATION)
struct GraphicsDeviceAdapter;
#endif
}

namespace WebKit {

class UpdateInfo;
class WebPage;

#if PLATFORM(WIN)
struct WindowGeometry;
#endif

class LayerTreeHost : public RefCounted<LayerTreeHost> {
public:
    static PassRefPtr<LayerTreeHost> create(WebPage*);
    virtual ~LayerTreeHost();

    static bool supportsAcceleratedCompositing();

    virtual const LayerTreeContext& layerTreeContext() = 0;
    virtual void scheduleLayerFlush() = 0;
    virtual void setLayerFlushSchedulingEnabled(bool) = 0;
    virtual void setShouldNotifyAfterNextScheduledLayerFlush(bool) = 0;
    virtual void setRootCompositingLayer(WebCore::GraphicsLayer*) = 0;
    virtual void invalidate() = 0;

    virtual void setNonCompositedContentsNeedDisplay(const WebCore::IntRect&) = 0;
    virtual void scrollNonCompositedContents(const WebCore::IntRect& scrollRect, const WebCore::IntSize& scrollOffset) = 0;
    virtual void forceRepaint() = 0;
    virtual bool forceRepaintAsync(uint64_t /*callbackID*/) { return false; }
    virtual void sizeDidChange(const WebCore::IntSize& newSize) = 0;
    virtual void deviceScaleFactorDidChange() = 0;

    virtual void didInstallPageOverlay() = 0;
    virtual void didUninstallPageOverlay() = 0;
    virtual void setPageOverlayNeedsDisplay(const WebCore::IntRect&) = 0;
    virtual void setPageOverlayOpacity(float) { }
    virtual bool pageOverlayShouldApplyFadeWhenPainting() const { return true; }

    virtual void pauseRendering() { }
    virtual void resumeRendering() { }

    virtual WebCore::GraphicsLayerFactory* graphicsLayerFactory() { return 0; }
    virtual void setBackgroundColor(const WebCore::Color&) { }

#if USE(COORDINATED_GRAPHICS)
    virtual void setVisibleContentsRect(const WebCore::FloatRect&, float /* scale */, const WebCore::FloatPoint&) { }
    virtual void renderNextFrame() { }
    virtual void purgeBackingStores() { }
    virtual void didReceiveLayerTreeCoordinatorMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder&) = 0;
#endif

#if PLATFORM(WIN)
    virtual void scheduleChildWindowGeometryUpdate(const WindowGeometry&) = 0;
#endif

#if PLATFORM(MAC)
    virtual void setLayerHostingMode(LayerHostingMode) { }
#endif

#if PLATFORM(WIN) && USE(AVFOUNDATION)
    virtual WebCore::GraphicsDeviceAdapter* graphicsDeviceAdapter() const { return 0; }
#endif

#if USE(COORDINATED_GRAPHICS) && ENABLE(REQUEST_ANIMATION_FRAME)
    virtual void scheduleAnimation() = 0;
#endif

protected:
    explicit LayerTreeHost(WebPage*);

    WebPage* m_webPage;

#if USE(COORDINATED_GRAPHICS)
    bool m_waitingForUIProcess;
#endif
};

#if !PLATFORM(WIN) && !USE(COORDINATED_GRAPHICS)
inline bool LayerTreeHost::supportsAcceleratedCompositing()
{
    return true;
}
#endif

} // namespace WebKit

#endif // LayerTreeHost_h
