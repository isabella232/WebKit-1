/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 * Copyright (C) 2010 Pawel Hajdan (phajdan.jr@chromium.org)
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
#include "TestRunner.h"

#include "WebAnimationController.h"
#include "WebBindings.h"
#include "WebDocument.h"
#include "WebElement.h"
#include "WebFindOptions.h"
#include "WebFrame.h"
#include "WebInputElement.h"
#include "WebPreferences.h"
#include "WebScriptSource.h"
#include "WebSecurityPolicy.h"
#include "WebSettings.h"
#include "WebSurroundingText.h"
#include "WebTestDelegate.h"
#include "WebView.h"
#include "WebWorkerInfo.h"
#include "platform/WebPoint.h"
#include "v8/include/v8.h"
#include <wtf/text/WTFString.h>

#if OS(LINUX) || OS(ANDROID)
#include "linux/WebFontRendering.h"
#endif

using namespace WebKit;
using namespace std;

namespace WebTestRunner {

namespace {

// Sets map based on scriptFontPairs, a collapsed vector of pairs of ISO 15924
// four-letter script code and font such as:
// { "Arab", "My Arabic Font", "Grek", "My Greek Font" }
static void setFontMap(WebPreferences::ScriptFontFamilyMap& map, const Vector<WebString>& scriptFontPairs)
{
    map.clear();
    size_t i = 0;
    while (i + 1 < scriptFontPairs.size()) {
        const WebString& script = scriptFontPairs[i++];
        const WebString& font = scriptFontPairs[i++];

        int32_t code = u_getPropertyValueEnum(UCHAR_SCRIPT, script.utf8().data());
        if (code >= 0 && code < USCRIPT_CODE_LIMIT)
            map[static_cast<int>(code)] = font;
    }
}

}

TestRunner::TestRunner()
    : m_delegate(0)
    , m_webView(0)
{
    // Methods implemented in terms of chromium's public WebKit API.
    bindMethod("setTabKeyCyclesThroughElements", &TestRunner::setTabKeyCyclesThroughElements);
    bindMethod("setAsynchronousSpellCheckingEnabled", &TestRunner::setAsynchronousSpellCheckingEnabled);
    bindMethod("execCommand", &TestRunner::execCommand);
    bindMethod("isCommandEnabled", &TestRunner::isCommandEnabled);
    bindMethod("pauseAnimationAtTimeOnElementWithId", &TestRunner::pauseAnimationAtTimeOnElementWithId);
    bindMethod("pauseTransitionAtTimeOnElementWithId", &TestRunner::pauseTransitionAtTimeOnElementWithId);
    bindMethod("elementDoesAutoCompleteForElementWithId", &TestRunner::elementDoesAutoCompleteForElementWithId);
    bindMethod("numberOfActiveAnimations", &TestRunner::numberOfActiveAnimations);
    bindMethod("callShouldCloseOnWebView", &TestRunner::callShouldCloseOnWebView);
    bindMethod("setDomainRelaxationForbiddenForURLScheme", &TestRunner::setDomainRelaxationForbiddenForURLScheme);
    bindMethod("evaluateScriptInIsolatedWorldAndReturnValue", &TestRunner::evaluateScriptInIsolatedWorldAndReturnValue);
    bindMethod("evaluateScriptInIsolatedWorld", &TestRunner::evaluateScriptInIsolatedWorld);
    bindMethod("setIsolatedWorldSecurityOrigin", &TestRunner::setIsolatedWorldSecurityOrigin);
    bindMethod("setIsolatedWorldContentSecurityPolicy", &TestRunner::setIsolatedWorldContentSecurityPolicy);
    bindMethod("addOriginAccessWhitelistEntry", &TestRunner::addOriginAccessWhitelistEntry);
    bindMethod("removeOriginAccessWhitelistEntry", &TestRunner::removeOriginAccessWhitelistEntry);
    bindMethod("hasCustomPageSizeStyle", &TestRunner::hasCustomPageSizeStyle);
    bindMethod("forceRedSelectionColors", &TestRunner::forceRedSelectionColors);
    bindMethod("addUserScript", &TestRunner::addUserScript);
    bindMethod("addUserStyleSheet", &TestRunner::addUserStyleSheet);
    bindMethod("startSpeechInput", &TestRunner::startSpeechInput);
    bindMethod("loseCompositorContext", &TestRunner::loseCompositorContext);
    bindMethod("markerTextForListItem", &TestRunner::markerTextForListItem);
    bindMethod("findString", &TestRunner::findString);
    bindMethod("setMinimumTimerInterval", &TestRunner::setMinimumTimerInterval);
    bindMethod("setAutofilled", &TestRunner::setAutofilled);
    bindMethod("setValueForUser", &TestRunner::setValueForUser);
    bindMethod("enableFixedLayoutMode", &TestRunner::enableFixedLayoutMode);
    bindMethod("setFixedLayoutSize", &TestRunner::setFixedLayoutSize);
    bindMethod("selectionAsMarkup", &TestRunner::selectionAsMarkup);
    bindMethod("setTextSubpixelPositioning", &TestRunner::setTextSubpixelPositioning);
    bindMethod("resetPageVisibility", &TestRunner::resetPageVisibility);
    bindMethod("setPageVisibility", &TestRunner::setPageVisibility);
    bindMethod("setTextDirection", &TestRunner::setTextDirection);
    bindMethod("textSurroundingNode", &TestRunner::textSurroundingNode);
    bindMethod("setTouchDragDropEnabled", &TestRunner::setTouchDragDropEnabled);

    // The following modify WebPreferences.
    bindMethod("setUserStyleSheetEnabled", &TestRunner::setUserStyleSheetEnabled);
    bindMethod("setUserStyleSheetLocation", &TestRunner::setUserStyleSheetLocation);
    bindMethod("setAuthorAndUserStylesEnabled", &TestRunner::setAuthorAndUserStylesEnabled);
    bindMethod("setPopupBlockingEnabled", &TestRunner::setPopupBlockingEnabled);
    bindMethod("setJavaScriptCanAccessClipboard", &TestRunner::setJavaScriptCanAccessClipboard);
    bindMethod("setXSSAuditorEnabled", &TestRunner::setXSSAuditorEnabled);
    bindMethod("setAllowUniversalAccessFromFileURLs", &TestRunner::setAllowUniversalAccessFromFileURLs);
    bindMethod("setAllowFileAccessFromFileURLs", &TestRunner::setAllowFileAccessFromFileURLs);
    bindMethod("overridePreference", &TestRunner::overridePreference);
    bindMethod("setPluginsEnabled", &TestRunner::setPluginsEnabled);

    // Properties.
    bindProperty("workerThreadCount", &TestRunner::workerThreadCount);
    bindProperty("globalFlag", &m_globalFlag);
    bindProperty("platformName", &m_platformName);

    // The following are stubs.
    bindMethod("dumpDatabaseCallbacks", &TestRunner::notImplemented);
#if ENABLE(NOTIFICATIONS)
    bindMethod("denyWebNotificationPermission", &TestRunner::notImplemented);
    bindMethod("removeAllWebNotificationPermissions", &TestRunner::notImplemented);
    bindMethod("simulateWebNotificationClick", &TestRunner::notImplemented);
#endif
    bindMethod("setIconDatabaseEnabled", &TestRunner::notImplemented);
    bindMethod("setScrollbarPolicy", &TestRunner::notImplemented);
    bindMethod("clearAllApplicationCaches", &TestRunner::notImplemented);
    bindMethod("clearApplicationCacheForOrigin", &TestRunner::notImplemented);
    bindMethod("clearBackForwardList", &TestRunner::notImplemented);
    bindMethod("keepWebHistory", &TestRunner::notImplemented);
    bindMethod("setApplicationCacheOriginQuota", &TestRunner::notImplemented);
    bindMethod("setCallCloseOnWebViews", &TestRunner::notImplemented);
    bindMethod("setMainFrameIsFirstResponder", &TestRunner::notImplemented);
    bindMethod("setPrivateBrowsingEnabled", &TestRunner::notImplemented);
    bindMethod("setUseDashboardCompatibilityMode", &TestRunner::notImplemented);
    bindMethod("deleteAllLocalStorage", &TestRunner::notImplemented);
    bindMethod("localStorageDiskUsageForOrigin", &TestRunner::notImplemented);
    bindMethod("originsWithLocalStorage", &TestRunner::notImplemented);
    bindMethod("deleteLocalStorageForOrigin", &TestRunner::notImplemented);
    bindMethod("observeStorageTrackerNotifications", &TestRunner::notImplemented);
    bindMethod("syncLocalStorage", &TestRunner::notImplemented);
    bindMethod("addDisallowedURL", &TestRunner::notImplemented);
    bindMethod("applicationCacheDiskUsageForOrigin", &TestRunner::notImplemented);
    bindMethod("abortModal", &TestRunner::notImplemented);

    // The fallback method is called when an unknown method is invoked.
    bindFallbackMethod(&TestRunner::fallbackMethod);
}

void TestRunner::reset()
{
    if (m_webView) {
        m_webView->setZoomLevel(false, 0);
        m_webView->setTabKeyCyclesThroughElements(true);
#if !OS(DARWIN) && !OS(WINDOWS) // Actually, TOOLKIT_GTK
        // (Constants copied because we can't depend on the header that defined
        // them from this file.)
        m_webView->setSelectionColors(0xff1e90ff, 0xff000000, 0xffc8c8c8, 0xff323232);
#endif
        m_webView->removeAllUserContent();
        m_webView->disableAutoResizeMode();
    }
    WebSecurityPolicy::resetOriginAccessWhitelists();
#if OS(LINUX) || OS(ANDROID)
    WebFontRendering::setSubpixelPositioning(false);
#endif

    m_globalFlag.set(false);
    m_platformName.set("chromium");

    m_userStyleSheetLocation = WebURL();
}

void TestRunner::setTabKeyCyclesThroughElements(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool())
        m_webView->setTabKeyCyclesThroughElements(arguments[0].toBoolean());
    result->setNull();
}

void TestRunner::setAsynchronousSpellCheckingEnabled(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool())
        m_webView->settings()->setAsynchronousSpellCheckingEnabled(cppVariantToBool(arguments[0]));
    result->setNull();
}

void TestRunner::execCommand(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() <= 0 || !arguments[0].isString())
        return;

    std::string command = arguments[0].toString();
    std::string value("");
    // Ignore the second parameter (which is userInterface)
    // since this command emulates a manual action.
    if (arguments.size() >= 3 && arguments[2].isString())
        value = arguments[2].toString();

    // Note: webkit's version does not return the boolean, so neither do we.
    m_webView->focusedFrame()->executeCommand(WebString::fromUTF8(command), WebString::fromUTF8(value));
}

void TestRunner::isCommandEnabled(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() <= 0 || !arguments[0].isString()) {
        result->setNull();
        return;
    }

    std::string command = arguments[0].toString();
    bool rv = m_webView->focusedFrame()->isCommandEnabled(WebString::fromUTF8(command));
    result->set(rv);
}

bool TestRunner::pauseAnimationAtTimeOnElementWithId(const WebString& animationName, double time, const WebString& elementId)
{
    WebFrame* webFrame = m_webView->mainFrame();
    if (!webFrame)
        return false;

    WebAnimationController* controller = webFrame->animationController();
    if (!controller)
        return false;

    WebElement element = webFrame->document().getElementById(elementId);
    if (element.isNull())
        return false;
    return controller->pauseAnimationAtTime(element, animationName, time);
}

bool TestRunner::pauseTransitionAtTimeOnElementWithId(const WebString& propertyName, double time, const WebString& elementId)
{
    WebFrame* webFrame = m_webView->mainFrame();
    if (!webFrame)
        return false;

    WebAnimationController* controller = webFrame->animationController();
    if (!controller)
        return false;

    WebElement element = webFrame->document().getElementById(elementId);
    if (element.isNull())
        return false;
    return controller->pauseTransitionAtTime(element, propertyName, time);
}

bool TestRunner::elementDoesAutoCompleteForElementWithId(const WebString& elementId)
{
    WebFrame* webFrame = m_webView->mainFrame();
    if (!webFrame)
        return false;

    WebElement element = webFrame->document().getElementById(elementId);
    if (element.isNull() || !element.hasTagName("input"))
        return false;

    WebInputElement inputElement = element.to<WebInputElement>();
    return inputElement.autoComplete();
}

int TestRunner::numberOfActiveAnimations()
{
    WebFrame* webFrame = m_webView->mainFrame();
    if (!webFrame)
        return -1;

    WebAnimationController* controller = webFrame->animationController();
    if (!controller)
        return -1;

    return controller->numberOfActiveAnimations();
}

void TestRunner::pauseAnimationAtTimeOnElementWithId(const CppArgumentList& arguments, CppVariant* result)
{
    result->set(false);
    if (arguments.size() > 2 && arguments[0].isString() && arguments[1].isNumber() && arguments[2].isString()) {
        WebString animationName = cppVariantToWebString(arguments[0]);
        double time = arguments[1].toDouble();
        WebString elementId = cppVariantToWebString(arguments[2]);
        result->set(pauseAnimationAtTimeOnElementWithId(animationName, time, elementId));
    }
}

void TestRunner::pauseTransitionAtTimeOnElementWithId(const CppArgumentList& arguments, CppVariant* result)
{
    result->set(false);
    if (arguments.size() > 2 && arguments[0].isString() && arguments[1].isNumber() && arguments[2].isString()) {
        WebString propertyName = cppVariantToWebString(arguments[0]);
        double time = arguments[1].toDouble();
        WebString elementId = cppVariantToWebString(arguments[2]);
        result->set(pauseTransitionAtTimeOnElementWithId(propertyName, time, elementId));
    }
}

void TestRunner::elementDoesAutoCompleteForElementWithId(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() != 1 || !arguments[0].isString()) {
        result->set(false);
        return;
    }
    WebString elementId = cppVariantToWebString(arguments[0]);
    result->set(elementDoesAutoCompleteForElementWithId(elementId));
}

void TestRunner::numberOfActiveAnimations(const CppArgumentList&, CppVariant* result)
{
    result->set(numberOfActiveAnimations());
}

void TestRunner::callShouldCloseOnWebView(const CppArgumentList&, CppVariant* result)
{
    result->set(m_webView->dispatchBeforeUnloadEvent());
}

void TestRunner::setDomainRelaxationForbiddenForURLScheme(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() != 2 || !arguments[0].isBool() || !arguments[1].isString())
        return;
    m_webView->setDomainRelaxationForbidden(cppVariantToBool(arguments[0]), cppVariantToWebString(arguments[1]));
}

void TestRunner::evaluateScriptInIsolatedWorldAndReturnValue(const CppArgumentList& arguments, CppVariant* result)
{
    v8::HandleScope scope;
    WebVector<v8::Local<v8::Value> > values;
    if (arguments.size() >= 2 && arguments[0].isNumber() && arguments[1].isString()) {
        WebScriptSource source(cppVariantToWebString(arguments[1]));
        // This relies on the iframe focusing itself when it loads. This is a bit
        // sketchy, but it seems to be what other tests do.
        m_webView->focusedFrame()->executeScriptInIsolatedWorld(arguments[0].toInt32(), &source, 1, 1, &values);
    }
    result->setNull();
    // Since only one script was added, only one result is expected
    if (values.size() == 1 && !values[0].IsEmpty()) {
        v8::Local<v8::Value> scriptValue = values[0];
        // FIXME: There are many more types that can be handled.
        if (scriptValue->IsString()) {
            v8::String::AsciiValue asciiV8(scriptValue);
            result->set(std::string(*asciiV8));
        } else if (scriptValue->IsBoolean())
            result->set(scriptValue->ToBoolean()->Value());
        else if (scriptValue->IsNumber()) {
            if (scriptValue->IsInt32())
                result->set(scriptValue->ToInt32()->Value());
            else
                result->set(scriptValue->ToNumber()->Value());
        } else if (scriptValue->IsNull())
            result->setNull();
    }
}

void TestRunner::evaluateScriptInIsolatedWorld(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() >= 2 && arguments[0].isNumber() && arguments[1].isString()) {
        WebScriptSource source(cppVariantToWebString(arguments[1]));
        // This relies on the iframe focusing itself when it loads. This is a bit
        // sketchy, but it seems to be what other tests do.
        m_webView->focusedFrame()->executeScriptInIsolatedWorld(arguments[0].toInt32(), &source, 1, 1);
    }
    result->setNull();
}

void TestRunner::setIsolatedWorldSecurityOrigin(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();

    if (arguments.size() != 2 || !arguments[0].isNumber() || !(arguments[1].isString() || arguments[1].isNull()))
        return;

    WebSecurityOrigin origin;
    if (arguments[1].isString())
        origin = WebSecurityOrigin::createFromString(cppVariantToWebString(arguments[1]));
    m_webView->focusedFrame()->setIsolatedWorldSecurityOrigin(arguments[0].toInt32(), origin);
}

void TestRunner::setIsolatedWorldContentSecurityPolicy(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();

    if (arguments.size() != 2 || !arguments[0].isNumber() || !arguments[1].isString())
        return;

    m_webView->focusedFrame()->setIsolatedWorldContentSecurityPolicy(arguments[0].toInt32(), cppVariantToWebString(arguments[1]));
}

void TestRunner::addOriginAccessWhitelistEntry(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();

    if (arguments.size() != 4 || !arguments[0].isString() || !arguments[1].isString()
        || !arguments[2].isString() || !arguments[3].isBool())
        return;

    WebKit::WebURL url(GURL(arguments[0].toString()));
    if (!url.isValid())
        return;

    WebSecurityPolicy::addOriginAccessWhitelistEntry(
        url,
        cppVariantToWebString(arguments[1]),
        cppVariantToWebString(arguments[2]),
        arguments[3].toBoolean());
}

void TestRunner::removeOriginAccessWhitelistEntry(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();

    if (arguments.size() != 4 || !arguments[0].isString() || !arguments[1].isString()
        || !arguments[2].isString() || !arguments[3].isBool())
        return;

    WebKit::WebURL url(GURL(arguments[0].toString()));
    if (!url.isValid())
        return;

    WebSecurityPolicy::removeOriginAccessWhitelistEntry(
        url,
        cppVariantToWebString(arguments[1]),
        cppVariantToWebString(arguments[2]),
        arguments[3].toBoolean());
}

void TestRunner::hasCustomPageSizeStyle(const CppArgumentList& arguments, CppVariant* result)
{
    result->set(false);
    int pageIndex = 0;
    if (arguments.size() > 1)
        return;
    if (arguments.size() == 1)
        pageIndex = cppVariantToInt32(arguments[0]);
    WebFrame* frame = m_webView->mainFrame();
    if (!frame)
        return;
    result->set(frame->hasCustomPageSizeStyle(pageIndex));
}

void TestRunner::forceRedSelectionColors(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    m_webView->setSelectionColors(0xffee0000, 0xff00ee00, 0xff000000, 0xffc0c0c0);
}

void TestRunner::addUserScript(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() < 3 || !arguments[0].isString() || !arguments[1].isBool() || !arguments[2].isBool())
        return;
    WebView::addUserScript(
        cppVariantToWebString(arguments[0]), WebVector<WebString>(),
        arguments[1].toBoolean() ? WebView::UserScriptInjectAtDocumentStart : WebView::UserScriptInjectAtDocumentEnd,
        arguments[2].toBoolean() ? WebView::UserContentInjectInAllFrames : WebView::UserContentInjectInTopFrameOnly);
}

void TestRunner::addUserStyleSheet(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() < 2 || !arguments[0].isString() || !arguments[1].isBool())
        return;
    WebView::addUserStyleSheet(
        cppVariantToWebString(arguments[0]), WebVector<WebString>(),
        arguments[1].toBoolean() ? WebView::UserContentInjectInAllFrames : WebView::UserContentInjectInTopFrameOnly,
        // Chromium defaults to InjectInSubsequentDocuments, but for compatibility
        // with the other ports' DRTs, we use UserStyleInjectInExistingDocuments.
        WebView::UserStyleInjectInExistingDocuments);
}

void TestRunner::startSpeechInput(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() != 1)
        return;

    WebElement element;
    if (!WebBindings::getElement(arguments[0].value.objectValue, &element))
        return;

    WebInputElement* input = toWebInputElement(&element);
    if (!input)
        return;

    if (!input->isSpeechInputEnabled())
        return;

    input->startSpeechInput();
}

void TestRunner::loseCompositorContext(const CppArgumentList& args, CppVariant*)
{
    int numTimes;
    if (args.size() == 1 || !args[0].isNumber())
        numTimes = 1;
    else
        numTimes = args[0].toInt32();
    m_webView->loseCompositorContext(numTimes);
}

void TestRunner::markerTextForListItem(const CppArgumentList& args, CppVariant* result)
{
    WebElement element;
    if (!WebBindings::getElement(args[0].value.objectValue, &element))
        result->setNull();
    else
        result->set(element.document().frame()->markerTextForListItem(element).utf8());
}

void TestRunner::findString(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() < 1 || !arguments[0].isString())
        return;

    WebFindOptions findOptions;
    bool wrapAround = false;
    if (arguments.size() >= 2) {
        Vector<std::string> optionsArray = arguments[1].toStringVector();
        findOptions.matchCase = true;

        for (size_t i = 0; i < optionsArray.size(); ++i) {
            const std::string& option = optionsArray[i];
            // FIXME: Support all the options, so we can run findString.html too.
            if (option == "CaseInsensitive")
                findOptions.matchCase = false;
            else if (option == "Backwards")
                findOptions.forward = false;
            else if (option == "WrapAround")
                wrapAround = true;
        }
    }

    WebFrame* frame = m_webView->mainFrame();
    const bool findResult = frame->find(0, cppVariantToWebString(arguments[0]), findOptions, wrapAround, 0);
    result->set(findResult);
}

void TestRunner::setMinimumTimerInterval(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() < 1 || !arguments[0].isNumber())
        return;
    m_webView->settings()->setMinimumTimerInterval(arguments[0].toDouble());
}

void TestRunner::setAutofilled(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() != 2 || !arguments[1].isBool())
        return;

    WebElement element;
    if (!WebBindings::getElement(arguments[0].value.objectValue, &element))
        return;

    WebInputElement* input = toWebInputElement(&element);
    if (!input)
        return;

    input->setAutofilled(arguments[1].value.boolValue);
}

void TestRunner::setValueForUser(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() != 2)
        return;

    WebElement element;
    if (!WebBindings::getElement(arguments[0].value.objectValue, &element))
        return;

    WebInputElement* input = toWebInputElement(&element);
    if (!input)
        return;

    input->setValue(cppVariantToWebString(arguments[1]), true);
}

void TestRunner::enableFixedLayoutMode(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() <  1 || !arguments[0].isBool())
        return;
    bool enableFixedLayout = arguments[0].toBoolean();
    m_webView->enableFixedLayoutMode(enableFixedLayout);
}

void TestRunner::setFixedLayoutSize(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() <  2 || !arguments[0].isNumber() || !arguments[1].isNumber())
        return;
    int width = arguments[0].toInt32();
    int height = arguments[1].toInt32();
    m_webView->setFixedLayoutSize(WebSize(width, height));
}

void TestRunner::selectionAsMarkup(const CppArgumentList& arguments, CppVariant* result)
{
    result->set(m_webView->mainFrame()->selectionAsMarkup().utf8());
}

void TestRunner::setTextSubpixelPositioning(const CppArgumentList& arguments, CppVariant* result)
{
#if OS(LINUX) || OS(ANDROID)
    // Since FontConfig doesn't provide a variable to control subpixel positioning, we'll fall back
    // to setting it globally for all fonts.
    if (arguments.size() > 0 && arguments[0].isBool())
        WebFontRendering::setSubpixelPositioning(arguments[0].value.boolValue);
#endif
    result->setNull();
}

void TestRunner::resetPageVisibility(const CppArgumentList& arguments, CppVariant* result)
{
    m_webView->setVisibilityState(WebPageVisibilityStateVisible, true);
}

void TestRunner::setPageVisibility(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isString()) {
        string newVisibility = arguments[0].toString();
        if (newVisibility == "visible")
            m_webView->setVisibilityState(WebPageVisibilityStateVisible, false);
        else if (newVisibility == "hidden")
            m_webView->setVisibilityState(WebPageVisibilityStateHidden, false);
        else if (newVisibility == "prerender")
            m_webView->setVisibilityState(WebPageVisibilityStatePrerender, false);
        else if (newVisibility == "preview")
            m_webView->setVisibilityState(WebPageVisibilityStatePreview, false);
    }
}

void TestRunner::setTextDirection(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() != 1 || !arguments[0].isString())
        return;

    // Map a direction name to a WebTextDirection value.
    std::string directionName = arguments[0].toString();
    WebKit::WebTextDirection direction;
    if (directionName == "auto")
        direction = WebKit::WebTextDirectionDefault;
    else if (directionName == "rtl")
        direction = WebKit::WebTextDirectionRightToLeft;
    else if (directionName == "ltr")
        direction = WebKit::WebTextDirectionLeftToRight;
    else
        return;

    m_webView->setTextDirection(direction);
}

void TestRunner::textSurroundingNode(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() < 4 || !arguments[0].isObject() || !arguments[1].isNumber() || !arguments[2].isNumber() || !arguments[3].isNumber())
        return;

    WebNode node;
    if (!WebBindings::getNode(arguments[0].value.objectValue, &node))
        return;

    if (node.isNull() || !node.isTextNode())
        return;

    WebPoint point(arguments[1].toInt32(), arguments[2].toInt32());
    unsigned maxLength = arguments[3].toInt32();

    WebSurroundingText surroundingText;
    surroundingText.initialize(node, point, maxLength);
    if (surroundingText.isNull())
        return;

    result->set(surroundingText.textContent().utf8());
}

void TestRunner::setTouchDragDropEnabled(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() != 1 || !arguments[0].isBool())
        return;

    m_webView->settings()->setTouchDragDropEnabled(arguments[0].toBoolean());
}

void TestRunner::setUserStyleSheetEnabled(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool()) {
        m_delegate->preferences()->userStyleSheetLocation = arguments[0].value.boolValue ? m_userStyleSheetLocation : WebURL();
        m_delegate->applyPreferences();
    }
    result->setNull();
}

void TestRunner::setUserStyleSheetLocation(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isString()) {
        m_userStyleSheetLocation = m_delegate->localFileToDataURL(m_delegate->rewriteLayoutTestsURL(arguments[0].toString()));
        m_delegate->preferences()->userStyleSheetLocation = m_userStyleSheetLocation;
        m_delegate->applyPreferences();
    }
    result->setNull();
}

void TestRunner::setAuthorAndUserStylesEnabled(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool()) {
        m_delegate->preferences()->authorAndUserStylesEnabled = arguments[0].value.boolValue;
        m_delegate->applyPreferences();
    }
    result->setNull();
}

void TestRunner::setPopupBlockingEnabled(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool()) {
        bool blockPopups = arguments[0].toBoolean();
        m_delegate->preferences()->javaScriptCanOpenWindowsAutomatically = !blockPopups;
        m_delegate->applyPreferences();
    }
    result->setNull();
}

void TestRunner::setJavaScriptCanAccessClipboard(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool()) {
        m_delegate->preferences()->javaScriptCanAccessClipboard = arguments[0].value.boolValue;
        m_delegate->applyPreferences();
    }
    result->setNull();
}

void TestRunner::setXSSAuditorEnabled(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool()) {
        m_delegate->preferences()->XSSAuditorEnabled = arguments[0].value.boolValue;
        m_delegate->applyPreferences();
    }
    result->setNull();
}

void TestRunner::setAllowUniversalAccessFromFileURLs(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool()) {
        m_delegate->preferences()->allowUniversalAccessFromFileURLs = arguments[0].value.boolValue;
        m_delegate->applyPreferences();
    }
    result->setNull();
}

void TestRunner::setAllowFileAccessFromFileURLs(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool()) {
        m_delegate->preferences()->allowFileAccessFromFileURLs = arguments[0].value.boolValue;
        m_delegate->applyPreferences();
    }
    result->setNull();
}

void TestRunner::overridePreference(const CppArgumentList& arguments, CppVariant* result)
{
    result->setNull();
    if (arguments.size() != 2 || !arguments[0].isString())
        return;

    string key = arguments[0].toString();
    CppVariant value = arguments[1];
    WebPreferences* prefs = m_delegate->preferences();
    if (key == "WebKitStandardFont")
        prefs->standardFontFamily = cppVariantToWebString(value);
    else if (key == "WebKitFixedFont")
        prefs->fixedFontFamily = cppVariantToWebString(value);
    else if (key == "WebKitSerifFont")
        prefs->serifFontFamily = cppVariantToWebString(value);
    else if (key == "WebKitSansSerifFont")
        prefs->sansSerifFontFamily = cppVariantToWebString(value);
    else if (key == "WebKitCursiveFont")
        prefs->cursiveFontFamily = cppVariantToWebString(value);
    else if (key == "WebKitFantasyFont")
        prefs->fantasyFontFamily = cppVariantToWebString(value);
    else if (key == "WebKitStandardFontMap")
        setFontMap(prefs->standardFontMap, cppVariantToWebStringArray(value));
    else if (key == "WebKitFixedFontMap")
        setFontMap(prefs->fixedFontMap, cppVariantToWebStringArray(value));
    else if (key == "WebKitSerifFontMap")
        setFontMap(prefs->serifFontMap, cppVariantToWebStringArray(value));
    else if (key == "WebKitSansSerifFontMap")
        setFontMap(prefs->sansSerifFontMap, cppVariantToWebStringArray(value));
    else if (key == "WebKitCursiveFontMap")
        setFontMap(prefs->cursiveFontMap, cppVariantToWebStringArray(value));
    else if (key == "WebKitFantasyFontMap")
        setFontMap(prefs->fantasyFontMap, cppVariantToWebStringArray(value));
    else if (key == "WebKitDefaultFontSize")
        prefs->defaultFontSize = cppVariantToInt32(value);
    else if (key == "WebKitDefaultFixedFontSize")
        prefs->defaultFixedFontSize = cppVariantToInt32(value);
    else if (key == "WebKitMinimumFontSize")
        prefs->minimumFontSize = cppVariantToInt32(value);
    else if (key == "WebKitMinimumLogicalFontSize")
        prefs->minimumLogicalFontSize = cppVariantToInt32(value);
    else if (key == "WebKitDefaultTextEncodingName")
        prefs->defaultTextEncodingName = cppVariantToWebString(value);
    else if (key == "WebKitJavaScriptEnabled")
        prefs->javaScriptEnabled = cppVariantToBool(value);
    else if (key == "WebKitWebSecurityEnabled")
        prefs->webSecurityEnabled = cppVariantToBool(value);
    else if (key == "WebKitJavaScriptCanOpenWindowsAutomatically")
        prefs->javaScriptCanOpenWindowsAutomatically = cppVariantToBool(value);
    else if (key == "WebKitSupportsMultipleWindows")
        prefs->supportsMultipleWindows = cppVariantToBool(value);
    else if (key == "WebKitDisplayImagesKey")
        prefs->loadsImagesAutomatically = cppVariantToBool(value);
    else if (key == "WebKitPluginsEnabled")
        prefs->pluginsEnabled = cppVariantToBool(value);
    else if (key == "WebKitDOMPasteAllowedPreferenceKey")
        prefs->DOMPasteAllowed = cppVariantToBool(value);
    else if (key == "WebKitDeveloperExtrasEnabledPreferenceKey")
        prefs->developerExtrasEnabled = cppVariantToBool(value);
    else if (key == "WebKitShrinksStandaloneImagesToFit")
        prefs->shrinksStandaloneImagesToFit = cppVariantToBool(value);
    else if (key == "WebKitTextAreasAreResizable")
        prefs->textAreasAreResizable = cppVariantToBool(value);
    else if (key == "WebKitJavaEnabled")
        prefs->javaEnabled = cppVariantToBool(value);
    else if (key == "WebKitUsesPageCachePreferenceKey")
        prefs->usesPageCache = cppVariantToBool(value);
    else if (key == "WebKitPageCacheSupportsPluginsPreferenceKey")
        prefs->pageCacheSupportsPlugins = cppVariantToBool(value);
    else if (key == "WebKitJavaScriptCanAccessClipboard")
        prefs->javaScriptCanAccessClipboard = cppVariantToBool(value);
    else if (key == "WebKitXSSAuditorEnabled")
        prefs->XSSAuditorEnabled = cppVariantToBool(value);
    else if (key == "WebKitLocalStorageEnabledPreferenceKey")
        prefs->localStorageEnabled = cppVariantToBool(value);
    else if (key == "WebKitOfflineWebApplicationCacheEnabled")
        prefs->offlineWebApplicationCacheEnabled = cppVariantToBool(value);
    else if (key == "WebKitTabToLinksPreferenceKey")
        prefs->tabsToLinks = cppVariantToBool(value);
    else if (key == "WebKitWebGLEnabled")
        prefs->experimentalWebGLEnabled = cppVariantToBool(value);
    else if (key == "WebKitCSSRegionsEnabled")
        prefs->experimentalCSSRegionsEnabled = cppVariantToBool(value);
    else if (key == "WebKitCSSGridLayoutEnabled")
        prefs->experimentalCSSGridLayoutEnabled = cppVariantToBool(value);
    else if (key == "WebKitHyperlinkAuditingEnabled")
        prefs->hyperlinkAuditingEnabled = cppVariantToBool(value);
    else if (key == "WebKitEnableCaretBrowsing")
        prefs->caretBrowsingEnabled = cppVariantToBool(value);
    else if (key == "WebKitAllowDisplayingInsecureContent")
        prefs->allowDisplayOfInsecureContent = cppVariantToBool(value);
    else if (key == "WebKitAllowRunningInsecureContent")
        prefs->allowRunningOfInsecureContent = cppVariantToBool(value);
    else if (key == "WebKitCSSCustomFilterEnabled")
        prefs->cssCustomFilterEnabled = cppVariantToBool(value);
    else if (key == "WebKitShouldRespectImageOrientation")
        prefs->shouldRespectImageOrientation = cppVariantToBool(value);
    else if (key == "WebKitWebAudioEnabled")
        ASSERT(cppVariantToBool(value));
    else {
        string message("Invalid name for preference: ");
        message.append(key);
        printErrorMessage(message);
    }
    m_delegate->applyPreferences();
}

void TestRunner::setPluginsEnabled(const CppArgumentList& arguments, CppVariant* result)
{
    if (arguments.size() > 0 && arguments[0].isBool()) {
        m_delegate->preferences()->pluginsEnabled = arguments[0].toBoolean();
        m_delegate->applyPreferences();
    }
    result->setNull();
}

void TestRunner::workerThreadCount(CppVariant* result)
{
    result->set(static_cast<int>(WebWorkerInfo::dedicatedWorkerCount()));
}

// Need these conversions because the format of the value for booleans
// may vary - for example, on mac "1" and "0" are used for boolean.
bool TestRunner::cppVariantToBool(const CppVariant& value)
{
    if (value.isBool())
        return value.toBoolean();
    if (value.isNumber())
        return value.toInt32();
    if (value.isString()) {
        string valueString = value.toString();
        if (valueString == "true" || valueString == "1")
            return true;
        if (valueString == "false" || valueString == "0")
            return false;
    }
    printErrorMessage("Invalid value. Expected boolean value.");
    return false;
}

int32_t TestRunner::cppVariantToInt32(const CppVariant& value)
{
    if (value.isNumber())
        return value.toInt32();
    if (value.isString()) {
        string stringSource = value.toString();
        const char* source = stringSource.data();
        char* end;
        long number = strtol(source, &end, 10);
        if (end == source + stringSource.length() && number >= numeric_limits<int32_t>::min() && number <= numeric_limits<int32_t>::max())
            return static_cast<int32_t>(number);
    }
    printErrorMessage("Invalid value for preference. Expected integer value.");
    return 0;
}

WebString TestRunner::cppVariantToWebString(const CppVariant& value)
{
    if (!value.isString()) {
        printErrorMessage("Invalid value for preference. Expected string value.");
        return WebString();
    }
    return WebString::fromUTF8(value.toString());
}

Vector<WebString> TestRunner::cppVariantToWebStringArray(const CppVariant& value)
{
    if (!value.isObject()) {
        printErrorMessage("Invalid value for preference. Expected object value.");
        return Vector<WebString>();
    }
    Vector<WebString> resultVector;
    Vector<string> stringVector = value.toStringVector();
    for (size_t i = 0; i < stringVector.size(); ++i)
        resultVector.append(WebString::fromUTF8(stringVector[i].c_str()));
    return resultVector;
}

void TestRunner::printErrorMessage(const string& text)
{
    m_delegate->printMessage(string("CONSOLE MESSAGE: ") + text + "\n");
}

void TestRunner::fallbackMethod(const CppArgumentList&, CppVariant* result)
{
    printErrorMessage("JavaScript ERROR: unknown method called on TestRunner");
    result->setNull();
}

void TestRunner::notImplemented(const CppArgumentList&, CppVariant* result)
{
    result->setNull();
}

}
