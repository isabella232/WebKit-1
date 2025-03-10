/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef Document_h
#define Document_h

#include "CollectionType.h"
#include "Color.h"
#include "ContainerNode.h"
#include "DOMTimeStamp.h"
#include "DocumentEventQueue.h"
#include "DocumentTiming.h"
#include "IconURL.h"
#include "InspectorCounters.h"
#include "IntRect.h"
#include "MutationObserver.h"
#include "PageVisibilityState.h"
#include "PlatformScreen.h"
#include "QualifiedName.h"
#include "ReferrerPolicy.h"
#include "ScriptExecutionContext.h"
#include "StringWithDirection.h"
#include "Timer.h"
#include "TreeScope.h"
#include "ViewportArguments.h"
#include <wtf/Deque.h>
#include <wtf/FixedArray.h>
#include <wtf/OwnPtr.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/PassRefPtr.h>

namespace WebCore {

class AXObjectCache;
class Attr;
class CDATASection;
class CSSStyleDeclaration;
class CSSStyleSheet;
class CachedCSSStyleSheet;
class CachedResourceLoader;
class CachedScript;
class CanvasRenderingContext;
class CharacterData;
class Comment;
class ContextFeatures;
class DOMImplementation;
class DOMNamedFlowCollection;
class DOMSelection;
class DOMWindow;
class Database;
class DatabaseThread;
class DocumentFragment;
class DocumentLoader;
class DocumentMarkerController;
class DocumentParser;
class DocumentSharedObjectPool;
class DocumentStyleSheetCollection;
class DocumentType;
class DocumentWeakReference;
class Element;
class EntityReference;
class Event;
class EventListener;
class FloatRect;
class FloatQuad;
class FormController;
class Frame;
class FrameView;
class HTMLCanvasElement;
class HTMLCollection;
class HTMLAllCollection;
class HTMLDocument;
class HTMLElement;
class HTMLFrameOwnerElement;
class HTMLHeadElement;
class HTMLIFrameElement;
class HTMLMapElement;
class HTMLNameCollection;
class HitTestRequest;
class HitTestResult;
class IntPoint;
class LayoutPoint;
class LayoutRect;
class LiveNodeListBase;
class DOMWrapperWorld;
class JSNode;
class Locale;
class MediaCanStartListener;
class MediaQueryList;
class MediaQueryMatcher;
class MouseEventWithHitTestResults;
class NamedFlowCollection;
class NodeFilter;
class NodeIterator;
class Page;
class PlatformMouseEvent;
class ProcessingInstruction;
class Range;
class RegisteredEventListener;
class RenderArena;
class RenderView;
class RenderFullScreen;
class ScriptableDocumentParser;
class ScriptElementData;
class ScriptRunner;
class SecurityOrigin;
class SelectorQueryCache;
class SerializedScriptValue;
class SegmentedString;
class Settings;
class StyleResolver;
class StyleSheet;
class StyleSheetContents;
class StyleSheetList;
class Text;
class TextResourceDecoder;
class TreeWalker;
class WebKitNamedFlow;
class XMLHttpRequest;
class XPathEvaluator;
class XPathExpression;
class XPathNSResolver;
class XPathResult;

#if ENABLE(SVG)
class SVGDocumentExtensions;
#endif

#if ENABLE(XSLT)
class TransformSource;
#endif

#if ENABLE(DASHBOARD_SUPPORT) || ENABLE(DRAGGABLE_REGION)
struct AnnotatedRegionValue;
#endif

#if ENABLE(TOUCH_EVENTS)
class Touch;
class TouchList;
#endif

#if ENABLE(REQUEST_ANIMATION_FRAME)
class RequestAnimationFrameCallback;
class ScriptedAnimationController;
#endif

#if ENABLE(MICRODATA)
class MicroDataItemList;
#endif

#if ENABLE(LINK_PRERENDER)
class Prerenderer;
#endif

#if ENABLE(TEXT_AUTOSIZING)
class TextAutosizer;
#endif

#if ENABLE(CSP_NEXT)
class DOMSecurityPolicy;
#endif

typedef int ExceptionCode;

enum PageshowEventPersistence {
    PageshowEventNotPersisted = 0,
    PageshowEventPersisted = 1
};

enum StyleResolverUpdateFlag { RecalcStyleImmediately, DeferRecalcStyle, RecalcStyleIfNeeded };

enum NodeListInvalidationType {
    DoNotInvalidateOnAttributeChanges = 0,
    InvalidateOnClassAttrChange,
    InvalidateOnIdNameAttrChange,
    InvalidateOnNameAttrChange,
    InvalidateOnForAttrChange,
    InvalidateForFormControls,
    InvalidateOnHRefAttrChange,
    InvalidateOnItemAttrChange,
    InvalidateOnAnyAttrChange,
};
const int numNodeListInvalidationTypes = InvalidateOnAnyAttrChange + 1;

class Document : public ContainerNode, public TreeScope, public ScriptExecutionContext {
public:
    static PassRefPtr<Document> create(Frame* frame, const KURL& url)
    {
        return adoptRef(new Document(frame, url, false, false));
    }
    static PassRefPtr<Document> createXHTML(Frame* frame, const KURL& url)
    {
        return adoptRef(new Document(frame, url, true, false));
    }
    virtual ~Document();

    MediaQueryMatcher* mediaQueryMatcher();

    using ContainerNode::ref;
    using ContainerNode::deref;

    // Nodes belonging to this document hold guard references -
    // these are enough to keep the document from being destroyed, but
    // not enough to keep it from removing its children. This allows a
    // node that outlives its document to still have a valid document
    // pointer without introducing reference cycles.
    void guardRef()
    {
        ASSERT(!m_deletionHasBegun);
        ++m_guardRefCount;
    }

    void guardDeref()
    {
        ASSERT(!m_deletionHasBegun);
        --m_guardRefCount;
        if (!m_guardRefCount && !refCount()) {
#ifndef NDEBUG
            m_deletionHasBegun = true;
#endif
            delete this;
        }
    }

    Element* getElementById(const AtomicString& id) const;

    virtual bool canContainRangeEndPoint() const { return true; }

    Element* getElementByAccessKey(const String& key);
    void invalidateAccessKeyMap();

    SelectorQueryCache* selectorQueryCache();

    // DOM methods & attributes for Document

    DEFINE_ATTRIBUTE_EVENT_LISTENER(abort);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(change);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(click);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(contextmenu);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(dblclick);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(dragenter);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(dragover);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(dragleave);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(drop);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(dragstart);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(drag);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(dragend);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(input);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(invalid);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(keydown);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(keypress);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(keyup);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(mousedown);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(mousemove);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(mouseout);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(mouseover);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(mouseup);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(mousewheel);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(scroll);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(select);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(submit);

    DEFINE_ATTRIBUTE_EVENT_LISTENER(blur);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(error);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(focus);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(load);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(readystatechange);

    // WebKit extensions
    DEFINE_ATTRIBUTE_EVENT_LISTENER(beforecut);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(cut);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(beforecopy);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(copy);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(beforepaste);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(paste);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(reset);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(search);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(selectstart);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(selectionchange);
#if ENABLE(TOUCH_EVENTS)
    DEFINE_ATTRIBUTE_EVENT_LISTENER(touchstart);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(touchmove);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(touchend);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(touchcancel);
#endif
#if ENABLE(FULLSCREEN_API)
    DEFINE_ATTRIBUTE_EVENT_LISTENER(webkitfullscreenchange);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(webkitfullscreenerror);
#endif
#if ENABLE(POINTER_LOCK)
    DEFINE_ATTRIBUTE_EVENT_LISTENER(webkitpointerlockchange);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(webkitpointerlockerror);
#endif
#if ENABLE(PAGE_VISIBILITY_API)
    DEFINE_ATTRIBUTE_EVENT_LISTENER(webkitvisibilitychange);
#endif

    void setViewportArguments(const ViewportArguments& viewportArguments) { m_viewportArguments = viewportArguments; }
    ViewportArguments viewportArguments() const { return m_viewportArguments; }
#ifndef NDEBUG
    bool didDispatchViewportPropertiesChanged() const { return m_didDispatchViewportPropertiesChanged; }
#endif

    void setReferrerPolicy(ReferrerPolicy referrerPolicy) { m_referrerPolicy = referrerPolicy; }
    ReferrerPolicy referrerPolicy() const { return m_referrerPolicy; }

    DocumentType* doctype() const { return m_docType.get(); }

    DOMImplementation* implementation();
    
    Element* documentElement() const
    {
        return m_documentElement.get();
    }
    
    virtual PassRefPtr<Element> createElement(const AtomicString& tagName, ExceptionCode&);
    PassRefPtr<DocumentFragment> createDocumentFragment();
    PassRefPtr<Text> createTextNode(const String& data);
    PassRefPtr<Comment> createComment(const String& data);
    PassRefPtr<CDATASection> createCDATASection(const String& data, ExceptionCode&);
    PassRefPtr<ProcessingInstruction> createProcessingInstruction(const String& target, const String& data, ExceptionCode&);
    PassRefPtr<Attr> createAttribute(const String& name, ExceptionCode&);
    PassRefPtr<Attr> createAttributeNS(const String& namespaceURI, const String& qualifiedName, ExceptionCode&, bool shouldIgnoreNamespaceChecks = false);
    PassRefPtr<EntityReference> createEntityReference(const String& name, ExceptionCode&);
    PassRefPtr<Node> importNode(Node* importedNode, ExceptionCode& ec) { return importNode(importedNode, true, ec); }
    PassRefPtr<Node> importNode(Node* importedNode, bool deep, ExceptionCode&);
    virtual PassRefPtr<Element> createElementNS(const String& namespaceURI, const String& qualifiedName, ExceptionCode&);
    PassRefPtr<Element> createElement(const QualifiedName&, bool createdByParser);

    bool cssStickyPositionEnabled() const;
    bool cssRegionsEnabled() const;
#if ENABLE(CSS_REGIONS)
    PassRefPtr<DOMNamedFlowCollection> webkitGetNamedFlows();
#endif

    NamedFlowCollection* namedFlows();

    bool regionBasedColumnsEnabled() const;

    bool cssGridLayoutEnabled() const;

    /**
     * Retrieve all nodes that intersect a rect in the window's document, until it is fully enclosed by
     * the boundaries of a node.
     *
     * @param centerX x reference for the rectangle in CSS pixels
     * @param centerY y reference for the rectangle in CSS pixels
     * @param topPadding How much to expand the top of the rectangle
     * @param rightPadding How much to expand the right of the rectangle
     * @param bottomPadding How much to expand the bottom of the rectangle
     * @param leftPadding How much to expand the left of the rectangle
     * @param ignoreClipping whether or not to ignore the root scroll frame when retrieving the element.
     *        If false, this method returns null for coordinates outside of the viewport.
     */
    PassRefPtr<NodeList> nodesFromRect(int centerX, int centerY, unsigned topPadding, unsigned rightPadding,
                                       unsigned bottomPadding, unsigned leftPadding, bool ignoreClipping, bool allowShadowContent) const;
    Element* elementFromPoint(int x, int y) const;
    PassRefPtr<Range> caretRangeFromPoint(int x, int y);

    String readyState() const;

    String defaultCharset() const;

    String inputEncoding() const { return Document::encoding(); }
    String charset() const { return Document::encoding(); }
    String characterSet() const { return Document::encoding(); }

    String encoding() const;

    void setCharset(const String&);

    void setContent(const String&);

    String suggestedMIMEType() const;

    String contentLanguage() const { return m_contentLanguage; }
    void setContentLanguage(const String&);

    String xmlEncoding() const { return m_xmlEncoding; }
    String xmlVersion() const { return m_xmlVersion; }
    enum StandaloneStatus { StandaloneUnspecified, Standalone, NotStandalone };
    bool xmlStandalone() const { return m_xmlStandalone == Standalone; }
    StandaloneStatus xmlStandaloneStatus() const { return static_cast<StandaloneStatus>(m_xmlStandalone); }
    bool hasXMLDeclaration() const { return m_hasXMLDeclaration; }

    void setXMLEncoding(const String& encoding) { m_xmlEncoding = encoding; } // read-only property, only to be set from XMLDocumentParser
    void setXMLVersion(const String&, ExceptionCode&);
    void setXMLStandalone(bool, ExceptionCode&);
    void setHasXMLDeclaration(bool hasXMLDeclaration) { m_hasXMLDeclaration = hasXMLDeclaration ? 1 : 0; }

    String documentURI() const { return m_documentURI; }
    void setDocumentURI(const String&);

    virtual KURL baseURI() const;

#if ENABLE(PAGE_VISIBILITY_API)
    String webkitVisibilityState() const;
    bool webkitHidden() const;
    void dispatchVisibilityStateChangeEvent();
#endif

#if ENABLE(CSP_NEXT)
    DOMSecurityPolicy* securityPolicy();
#endif

    PassRefPtr<Node> adoptNode(PassRefPtr<Node> source, ExceptionCode&);

    PassRefPtr<HTMLCollection> images();
    PassRefPtr<HTMLCollection> embeds();
    PassRefPtr<HTMLCollection> plugins(); // an alias for embeds() required for the JS DOM bindings.
    PassRefPtr<HTMLCollection> applets();
    PassRefPtr<HTMLCollection> links();
    PassRefPtr<HTMLCollection> forms();
    PassRefPtr<HTMLCollection> anchors();
    PassRefPtr<HTMLCollection> scripts();
    PassRefPtr<HTMLCollection> all();

    PassRefPtr<HTMLCollection> windowNamedItems(const AtomicString& name);
    PassRefPtr<HTMLCollection> documentNamedItems(const AtomicString& name);

    // Other methods (not part of DOM)
    bool isHTMLDocument() const { return m_isHTML; }
    bool isXHTMLDocument() const { return m_isXHTML; }
    virtual bool isImageDocument() const { return false; }
#if ENABLE(SVG)
    virtual bool isSVGDocument() const { return false; }
    bool hasSVGRootNode() const;
#else
    static bool isSVGDocument() { return false; }
    static bool hasSVGRootNode() { return false; }
#endif
    virtual bool isPluginDocument() const { return false; }
    virtual bool isMediaDocument() const { return false; }
    virtual bool isFrameSet() const { return false; }

    bool isSrcdocDocument() const { return m_isSrcdocDocument; }

    StyleResolver* styleResolverIfExists() const { return m_styleResolver.get(); }

    bool isViewSource() const { return m_isViewSource; }
    void setIsViewSource(bool);

    bool sawElementsInKnownNamespaces() const { return m_sawElementsInKnownNamespaces; }

    StyleResolver* styleResolver()
    { 
        if (!m_styleResolver)
            createStyleResolver();
        return m_styleResolver.get();
    }

    void notifyRemovePendingSheetIfNeeded();

    bool haveStylesheetsLoaded() const;

    // This is a DOM function.
    StyleSheetList* styleSheets();

    DocumentStyleSheetCollection* styleSheetCollection() { return m_styleSheetCollection.get(); }

    bool gotoAnchorNeededAfterStylesheetsLoad() { return m_gotoAnchorNeededAfterStylesheetsLoad; }
    void setGotoAnchorNeededAfterStylesheetsLoad(bool b) { m_gotoAnchorNeededAfterStylesheetsLoad = b; }

    /**
     * Called when one or more stylesheets in the document may have been added, removed or changed.
     *
     * Creates a new style resolver and assign it to this document. This is done by iterating through all nodes in
     * document (or those before <BODY> in a HTML document), searching for stylesheets. Stylesheets can be contained in
     * <LINK>, <STYLE> or <BODY> elements, as well as processing instructions (XML documents only). A list is
     * constructed from these which is used to create the a new style selector which collates all of the stylesheets
     * found and is used to calculate the derived styles for all rendering objects.
     */
    void styleResolverChanged(StyleResolverUpdateFlag);

    void didAccessStyleResolver();

    void evaluateMediaQueryList();

    // Never returns 0.
    FormController* formController();
    Vector<String> formElementsState() const;
    void setStateForNewFormElements(const Vector<String>&);

    FrameView* view() const; // can be NULL
    Frame* frame() const { return m_frame; } // can be NULL
    Page* page() const; // can be NULL
    Settings* settings() const; // can be NULL

    PassRefPtr<Range> createRange();

    PassRefPtr<NodeIterator> createNodeIterator(Node* root, unsigned whatToShow,
        PassRefPtr<NodeFilter>, bool expandEntityReferences, ExceptionCode&);

    PassRefPtr<TreeWalker> createTreeWalker(Node* root, unsigned whatToShow,
        PassRefPtr<NodeFilter>, bool expandEntityReferences, ExceptionCode&);

    // Special support for editing
    PassRefPtr<CSSStyleDeclaration> createCSSStyleDeclaration();
    PassRefPtr<Text> createEditingTextNode(const String&);

    void recalcStyle(StyleChange = NoChange);
    bool childNeedsAndNotInStyleRecalc();
    void updateStyleIfNeeded();
    void updateLayout();
    void updateLayoutIgnorePendingStylesheets();
    PassRefPtr<RenderStyle> styleForElementIgnoringPendingStylesheets(Element*);
    PassRefPtr<RenderStyle> styleForPage(int pageIndex);

    // Returns true if page box (margin boxes and page borders) is visible.
    bool isPageBoxVisible(int pageIndex);

    // Returns the preferred page size and margins in pixels, assuming 96
    // pixels per inch. pageSize, marginTop, marginRight, marginBottom,
    // marginLeft must be initialized to the default values that are used if
    // auto is specified.
    void pageSizeAndMarginsInPixels(int pageIndex, IntSize& pageSize, int& marginTop, int& marginRight, int& marginBottom, int& marginLeft);

    static void updateStyleForAllDocuments(); // FIXME: Try to reduce the # of calls to this function.
    CachedResourceLoader* cachedResourceLoader() { return m_cachedResourceLoader.get(); }

    virtual void attach();
    virtual void detach();
    void prepareForDestruction();

    // Override ScriptExecutionContext methods to do additional work
    virtual void suspendActiveDOMObjects(ActiveDOMObject::ReasonForSuspension) OVERRIDE;
    virtual void resumeActiveDOMObjects() OVERRIDE;

    RenderArena* renderArena() { return m_renderArena.get(); }

    // Implemented in RenderView.h to avoid a cyclic header dependency this just
    // returns renderer so callers can avoid verbose casts.
    RenderView* renderView() const;

    // Shadow the implementations on Node to provide faster access for documents.
    RenderObject* renderer() const { return m_renderer; }
    void setRenderer(RenderObject* renderer)
    {
        m_renderer = renderer;
        Node::setRenderer(renderer);
    }

    void clearAXObjectCache();
    AXObjectCache* axObjectCache() const;
    bool axObjectCacheExists() const;
    
    // to get visually ordered hebrew and arabic pages right
    void setVisuallyOrdered();
    bool visuallyOrdered() const { return m_visuallyOrdered; }
    
    DocumentLoader* loader() const;

    void open(Document* ownerDocument = 0);
    void implicitOpen();

    // close() is the DOM API document.close()
    void close();
    // In some situations (see the code), we ignore document.close().
    // explicitClose() bypass these checks and actually tries to close the
    // input stream.
    void explicitClose();
    // implicitClose() actually does the work of closing the input stream.
    void implicitClose();

    void cancelParsing();

    void write(const SegmentedString& text, Document* ownerDocument = 0);
    void write(const String& text, Document* ownerDocument = 0);
    void writeln(const String& text, Document* ownerDocument = 0);

    bool wellFormed() const { return m_wellFormed; }

    const KURL& url() const { return m_url; }
    void setURL(const KURL&);

    // To understand how these concepts relate to one another, please see the
    // comments surrounding their declaration.
    const KURL& baseURL() const { return m_baseURL; }
    void setBaseURLOverride(const KURL&);
    const KURL& baseURLOverride() const { return m_baseURLOverride; }
    const KURL& baseElementURL() const { return m_baseElementURL; }
    const String& baseTarget() const { return m_baseTarget; }
    void processBaseElement();

    KURL completeURL(const String&) const;
    KURL completeURL(const String&, const KURL& baseURLOverride) const;

    virtual String userAgent(const KURL&) const;

    virtual void disableEval(const String& errorMessage);

    bool canNavigate(Frame* targetFrame);
    Frame* findUnsafeParentScrollPropagationBoundary();

    CSSStyleSheet* elementSheet();
    
    virtual PassRefPtr<DocumentParser> createParser();
    DocumentParser* parser() const { return m_parser.get(); }
    ScriptableDocumentParser* scriptableDocumentParser() const;
    
    bool printing() const { return m_printing; }
    void setPrinting(bool p) { m_printing = p; }

    bool paginatedForScreen() const { return m_paginatedForScreen; }
    void setPaginatedForScreen(bool p) { m_paginatedForScreen = p; }
    
    bool paginated() const { return printing() || paginatedForScreen(); }

    enum CompatibilityMode { QuirksMode, LimitedQuirksMode, NoQuirksMode };

    virtual void setCompatibilityModeFromDoctype() { }
    void setCompatibilityMode(CompatibilityMode m);
    void lockCompatibilityMode() { m_compatibilityModeLocked = true; }
    CompatibilityMode compatibilityMode() const { return m_compatibilityMode; }

    String compatMode() const;

    bool inQuirksMode() const { return m_compatibilityMode == QuirksMode; }
    bool inLimitedQuirksMode() const { return m_compatibilityMode == LimitedQuirksMode; }
    bool inNoQuirksMode() const { return m_compatibilityMode == NoQuirksMode; }

    enum ReadyState {
        Loading,
        Interactive,
        Complete
    };
    void setReadyState(ReadyState);
    void setParsing(bool);
    bool parsing() const { return m_bParsing; }
    int minimumLayoutDelay();

    bool shouldScheduleLayout();
    bool isLayoutTimerActive();
    int elapsedTime() const;
    
    void setTextColor(const Color& color) { m_textColor = color; }
    Color textColor() const { return m_textColor; }

    const Color& linkColor() const { return m_linkColor; }
    const Color& visitedLinkColor() const { return m_visitedLinkColor; }
    const Color& activeLinkColor() const { return m_activeLinkColor; }
    void setLinkColor(const Color& c) { m_linkColor = c; }
    void setVisitedLinkColor(const Color& c) { m_visitedLinkColor = c; }
    void setActiveLinkColor(const Color& c) { m_activeLinkColor = c; }
    void resetLinkColor();
    void resetVisitedLinkColor();
    void resetActiveLinkColor();
    
    MouseEventWithHitTestResults prepareMouseEvent(const HitTestRequest&, const LayoutPoint&, const PlatformMouseEvent&);

    /* Newly proposed CSS3 mechanism for selecting alternate
       stylesheets using the DOM. May be subject to change as
       spec matures. - dwh
    */
    String preferredStylesheetSet() const;
    String selectedStylesheetSet() const;
    void setSelectedStylesheetSet(const String&);

    bool setFocusedNode(PassRefPtr<Node>);
    Node* focusedNode() const { return m_focusedNode.get(); }

    void getFocusableNodes(Vector<RefPtr<Node> >&);
    
    // The m_ignoreAutofocus flag specifies whether or not the document has been changed by the user enough 
    // for WebCore to ignore the autofocus attribute on any form controls
    bool ignoreAutofocus() const { return m_ignoreAutofocus; };
    void setIgnoreAutofocus(bool shouldIgnore = true) { m_ignoreAutofocus = shouldIgnore; };

    void setHoverNode(PassRefPtr<Node>);
    Node* hoverNode() const { return m_hoverNode.get(); }

    void setActiveNode(PassRefPtr<Node>);
    Node* activeNode() const { return m_activeNode.get(); }

    void focusedNodeRemoved();
    void removeFocusedNodeOfSubtree(Node*, bool amongChildrenOnly = false);
    void hoveredNodeDetached(Node*);
    void activeChainNodeDetached(Node*);

    void updateHoverActiveState(const HitTestRequest&, HitTestResult&);

    // Updates for :target (CSS3 selector).
    void setCSSTarget(Element*);
    Element* cssTarget() const { return m_cssTarget; }
    
    void scheduleForcedStyleRecalc();
    void scheduleStyleRecalc();
    void unscheduleStyleRecalc();
    bool hasPendingStyleRecalc() const;
    bool hasPendingForcedStyleRecalc() const;
    void styleRecalcTimerFired(Timer<Document>*);

    void registerNodeList(LiveNodeListBase*);
    void unregisterNodeList(LiveNodeListBase*);
    bool shouldInvalidateNodeListCaches(const QualifiedName* attrName = 0) const;
    void invalidateNodeListCaches(const QualifiedName* attrName);

    void attachNodeIterator(NodeIterator*);
    void detachNodeIterator(NodeIterator*);
    void moveNodeIteratorsToNewDocument(Node*, Document*);

    void attachRange(Range*);
    void detachRange(Range*);

    void updateRangesAfterChildrenChanged(ContainerNode*);
    // nodeChildrenWillBeRemoved is used when removing all node children at once.
    void nodeChildrenWillBeRemoved(ContainerNode*);
    // nodeWillBeRemoved is only safe when removing one node at a time.
    void nodeWillBeRemoved(Node*);
    bool canReplaceChild(Node* newChild, Node* oldChild);

    void textInserted(Node*, unsigned offset, unsigned length);
    void textRemoved(Node*, unsigned offset, unsigned length);
    void textNodesMerged(Text* oldNode, unsigned offset);
    void textNodeSplit(Text* oldNode);

    void createDOMWindow();
    void takeDOMWindowFrom(Document*);

    DOMWindow* domWindow() const { return m_domWindow.get(); }
    // In DOM Level 2, the Document's DOMWindow is called the defaultView.
    DOMWindow* defaultView() const { return domWindow(); } 

    // Helper functions for forwarding DOMWindow event related tasks to the DOMWindow if it exists.
    void setWindowAttributeEventListener(const AtomicString& eventType, PassRefPtr<EventListener>);
    EventListener* getWindowAttributeEventListener(const AtomicString& eventType);
    void dispatchWindowEvent(PassRefPtr<Event>, PassRefPtr<EventTarget> = 0);
    void dispatchWindowLoadEvent();

    PassRefPtr<Event> createEvent(const String& eventType, ExceptionCode&);

    // keep track of what types of event listeners are registered, so we don't
    // dispatch events unnecessarily
    enum ListenerType {
        DOMSUBTREEMODIFIED_LISTENER          = 1,
        DOMNODEINSERTED_LISTENER             = 1 << 1,
        DOMNODEREMOVED_LISTENER              = 1 << 2,
        DOMNODEREMOVEDFROMDOCUMENT_LISTENER  = 1 << 3,
        DOMNODEINSERTEDINTODOCUMENT_LISTENER = 1 << 4,
        DOMCHARACTERDATAMODIFIED_LISTENER    = 1 << 5,
        OVERFLOWCHANGED_LISTENER             = 1 << 6,
        ANIMATIONEND_LISTENER                = 1 << 7,
        ANIMATIONSTART_LISTENER              = 1 << 8,
        ANIMATIONITERATION_LISTENER          = 1 << 9,
        TRANSITIONEND_LISTENER               = 1 << 10,
        BEFORELOAD_LISTENER                  = 1 << 11,
        SCROLL_LISTENER                      = 1 << 12
        // 3 bits remaining
    };

    bool hasListenerType(ListenerType listenerType) const { return (m_listenerTypes & listenerType); }
    void addListenerTypeIfNeeded(const AtomicString& eventType);

#if ENABLE(MUTATION_OBSERVERS)
    bool hasMutationObserversOfType(MutationObserver::MutationType type) const
    {
        return m_mutationObserverTypes & type;
    }
    bool hasMutationObservers() const { return m_mutationObserverTypes; }
    void addMutationObserverTypes(MutationObserverOptions types) { m_mutationObserverTypes |= types; }
#endif

    CSSStyleDeclaration* getOverrideStyle(Element*, const String& pseudoElt);

    int nodeAbsIndex(Node*);
    Node* nodeWithAbsIndex(int absIndex);

    /**
     * Handles a HTTP header equivalent set by a meta tag using <meta http-equiv="..." content="...">. This is called
     * when a meta tag is encountered during document parsing, and also when a script dynamically changes or adds a meta
     * tag. This enables scripts to use meta tags to perform refreshes and set expiry dates in addition to them being
     * specified in a HTML file.
     *
     * @param equiv The http header name (value of the meta tag's "equiv" attribute)
     * @param content The header value (value of the meta tag's "content" attribute)
     */
    void processHttpEquiv(const String& equiv, const String& content);
    void processViewport(const String& features, ViewportArguments::Type origin);
    void updateViewportArguments();
    void processReferrerPolicy(const String& policy);

    // Returns the owning element in the parent document.
    // Returns 0 if this is the top level document.
    HTMLFrameOwnerElement* ownerElement() const;

    HTMLIFrameElement* seamlessParentIFrame() const;
    bool shouldDisplaySeamlesslyWithParent() const;

    // Used by DOM bindings; no direction known.
    String title() const { return m_title.string(); }
    void setTitle(const String&);

    void setTitleElement(const StringWithDirection&, Element* titleElement);
    void removeTitle(Element* titleElement);

    String cookie(ExceptionCode&) const;
    void setCookie(const String&, ExceptionCode&);

    String referrer() const;

    String domain() const;
    void setDomain(const String& newDomain, ExceptionCode&);

    String lastModified() const;

    // The cookieURL is used to query the cookie database for this document's
    // cookies. For example, if the cookie URL is http://example.com, we'll
    // use the non-Secure cookies for example.com when computing
    // document.cookie.
    //
    // Q: How is the cookieURL different from the document's URL?
    // A: The two URLs are the same almost all the time.  However, if one
    //    document inherits the security context of another document, it
    //    inherits its cookieURL but not its URL.
    //
    const KURL& cookieURL() const { return m_cookieURL; }
    void setCookieURL(const KURL& url) { m_cookieURL = url; }

    // The firstPartyForCookies is used to compute whether this document
    // appears in a "third-party" context for the purpose of third-party
    // cookie blocking.  The document is in a third-party context if the
    // cookieURL and the firstPartyForCookies are from different hosts.
    //
    // Note: Some ports (including possibly Apple's) only consider the
    //       document in a third-party context if the cookieURL and the
    //       firstPartyForCookies have a different registry-controlled
    //       domain.
    //
    const KURL& firstPartyForCookies() const { return m_firstPartyForCookies; }
    void setFirstPartyForCookies(const KURL& url) { m_firstPartyForCookies = url; }
    
    // The following implements the rule from HTML 4 for what valid names are.
    // To get this right for all the XML cases, we probably have to improve this or move it
    // and make it sensitive to the type of document.
    static bool isValidName(const String&);

    // The following breaks a qualified name into a prefix and a local name.
    // It also does a validity check, and returns false if the qualified name
    // is invalid.  It also sets ExceptionCode when name is invalid.
    static bool parseQualifiedName(const String& qualifiedName, String& prefix, String& localName, ExceptionCode&);

    // Checks to make sure prefix and namespace do not conflict (per DOM Core 3)
    static bool hasValidNamespaceForElements(const QualifiedName&);
    static bool hasValidNamespaceForAttributes(const QualifiedName&);

    HTMLElement* body() const;
    void setBody(PassRefPtr<HTMLElement>, ExceptionCode&);

    HTMLHeadElement* head();

    DocumentMarkerController* markers() const { return m_markers.get(); }

    bool directionSetOnDocumentElement() const { return m_directionSetOnDocumentElement; }
    bool writingModeSetOnDocumentElement() const { return m_writingModeSetOnDocumentElement; }
    void setDirectionSetOnDocumentElement(bool b) { m_directionSetOnDocumentElement = b; }
    void setWritingModeSetOnDocumentElement(bool b) { m_writingModeSetOnDocumentElement = b; }

    bool execCommand(const String& command, bool userInterface = false, const String& value = String());
    bool queryCommandEnabled(const String& command);
    bool queryCommandIndeterm(const String& command);
    bool queryCommandState(const String& command);
    bool queryCommandSupported(const String& command);
    String queryCommandValue(const String& command);

    KURL openSearchDescriptionURL();

    // designMode support
    enum InheritedBool { off = false, on = true, inherit };    
    void setDesignMode(InheritedBool value);
    InheritedBool getDesignMode() const;
    bool inDesignMode() const;

    Document* parentDocument() const;
    Document* topDocument() const;

    int docID() const { return m_docID; }
    
    ScriptRunner* scriptRunner() { return m_scriptRunner.get(); }

#if ENABLE(XSLT)
    void applyXSLTransform(ProcessingInstruction* pi);
    PassRefPtr<Document> transformSourceDocument() { return m_transformSourceDocument; }
    void setTransformSourceDocument(Document* doc) { m_transformSourceDocument = doc; }

    void setTransformSource(PassOwnPtr<TransformSource>);
    TransformSource* transformSource() const { return m_transformSource.get(); }
#endif

    void incDOMTreeVersion() { m_domTreeVersion = ++s_globalTreeVersion; }
    uint64_t domTreeVersion() const { return m_domTreeVersion; }

    void setDocType(PassRefPtr<DocumentType>);

    // XPathEvaluator methods
    PassRefPtr<XPathExpression> createExpression(const String& expression,
                                                 XPathNSResolver* resolver,
                                                 ExceptionCode& ec);
    PassRefPtr<XPathNSResolver> createNSResolver(Node *nodeResolver);
    PassRefPtr<XPathResult> evaluate(const String& expression,
                                     Node* contextNode,
                                     XPathNSResolver* resolver,
                                     unsigned short type,
                                     XPathResult* result,
                                     ExceptionCode& ec);

    enum PendingSheetLayout { NoLayoutWithPendingSheets, DidLayoutWithPendingSheets, IgnoreLayoutWithPendingSheets };

    bool didLayoutWithPendingStylesheets() const { return m_pendingSheetLayout == DidLayoutWithPendingSheets; }

    bool hasNodesWithPlaceholderStyle() const { return m_hasNodesWithPlaceholderStyle; }
    void setHasNodesWithPlaceholderStyle() { m_hasNodesWithPlaceholderStyle = true; }

    const Vector<IconURL>& iconURLs();
    void addIconURL(const String& url, const String& mimeType, const String& size, IconType);

    void setUseSecureKeyboardEntryWhenActive(bool);
    bool useSecureKeyboardEntryWhenActive() const;

    void updateFocusAppearanceSoon(bool restorePreviousSelection);
    void cancelFocusAppearanceUpdate();
        
    // Extension for manipulating canvas drawing contexts for use in CSS
    CanvasRenderingContext* getCSSCanvasContext(const String& type, const String& name, int width, int height);
    HTMLCanvasElement* getCSSCanvasElement(const String& name);

    bool isDNSPrefetchEnabled() const { return m_isDNSPrefetchEnabled; }
    void parseDNSPrefetchControlHeader(const String&);

    virtual void postTask(PassOwnPtr<Task>); // Executes the task on context's thread asynchronously.

    virtual void suspendScriptedAnimationControllerCallbacks();
    virtual void resumeScriptedAnimationControllerCallbacks();
    
    void windowScreenDidChange(PlatformDisplayID);

    virtual void finishedParsing();

    bool inPageCache() const { return m_inPageCache; }
    void setInPageCache(bool flag);

    // Elements can register themselves for the "documentWillSuspendForPageCache()" and  
    // "documentDidResumeFromPageCache()" callbacks
    void registerForPageCacheSuspensionCallbacks(Element*);
    void unregisterForPageCacheSuspensionCallbacks(Element*);

    void documentWillBecomeInactive();
    void documentWillSuspendForPageCache();
    void documentDidResumeFromPageCache();

    void registerForMediaVolumeCallbacks(Element*);
    void unregisterForMediaVolumeCallbacks(Element*);
    void mediaVolumeDidChange();

    void registerForPrivateBrowsingStateChangedCallbacks(Element*);
    void unregisterForPrivateBrowsingStateChangedCallbacks(Element*);
    void storageBlockingStateDidChange();
    void privateBrowsingStateDidChange();

    void setShouldCreateRenderers(bool);
    bool shouldCreateRenderers();

    void setDecoder(PassRefPtr<TextResourceDecoder>);
    TextResourceDecoder* decoder() const { return m_decoder.get(); }

    String displayStringModifiedByEncoding(const String&) const;
    PassRefPtr<StringImpl> displayStringModifiedByEncoding(PassRefPtr<StringImpl>) const;
    void displayBufferModifiedByEncoding(LChar* buffer, unsigned len) const
    {
        displayBufferModifiedByEncodingInternal(buffer, len);
    }
    void displayBufferModifiedByEncoding(UChar* buffer, unsigned len) const
    {
        displayBufferModifiedByEncodingInternal(buffer, len);
    }

    // Quirk for the benefit of Apple's Dictionary application.
    void setFrameElementsShouldIgnoreScrolling(bool ignore) { m_frameElementsShouldIgnoreScrolling = ignore; }
    bool frameElementsShouldIgnoreScrolling() const { return m_frameElementsShouldIgnoreScrolling; }

#if ENABLE(DASHBOARD_SUPPORT) || ENABLE(DRAGGABLE_REGION)
    void setAnnotatedRegionsDirty(bool f) { m_annotatedRegionsDirty = f; }
    bool annotatedRegionsDirty() const { return m_annotatedRegionsDirty; }
    bool hasAnnotatedRegions () const { return m_hasAnnotatedRegions; }
    void setHasAnnotatedRegions(bool f) { m_hasAnnotatedRegions = f; }
    const Vector<AnnotatedRegionValue>& annotatedRegions() const;
    void setAnnotatedRegions(const Vector<AnnotatedRegionValue>&);
#endif

    virtual void removeAllEventListeners();

#if ENABLE(SVG)
    const SVGDocumentExtensions* svgExtensions();
    SVGDocumentExtensions* accessSVGExtensions();
#endif

    void initSecurityContext();
    void initContentSecurityPolicy();

    void updateURLForPushOrReplaceState(const KURL&);
    void statePopped(PassRefPtr<SerializedScriptValue>);

    bool processingLoadEvent() const { return m_processingLoadEvent; }
    bool loadEventFinished() const { return m_loadEventFinished; }

    virtual bool isContextThread() const;
    virtual bool isJSExecutionForbidden() const { return false; }

    bool containsValidityStyleRules() const { return m_containsValidityStyleRules; }
    void setContainsValidityStyleRules() { m_containsValidityStyleRules = true; }

    void enqueueWindowEvent(PassRefPtr<Event>);
    void enqueueDocumentEvent(PassRefPtr<Event>);
    void enqueuePageshowEvent(PageshowEventPersistence);
    void enqueueHashchangeEvent(const String& oldURL, const String& newURL);
    void enqueuePopstateEvent(PassRefPtr<SerializedScriptValue> stateObject);
    virtual DocumentEventQueue* eventQueue() const { return m_eventQueue.get(); }

    void addMediaCanStartListener(MediaCanStartListener*);
    void removeMediaCanStartListener(MediaCanStartListener*);
    MediaCanStartListener* takeAnyMediaCanStartListener();

    const QualifiedName& idAttributeName() const { return m_idAttributeName; }
    
#if ENABLE(FULLSCREEN_API)
    bool webkitIsFullScreen() const { return m_fullScreenElement.get(); }
    bool webkitFullScreenKeyboardInputAllowed() const { return m_fullScreenElement.get() && m_areKeysEnabledInFullScreen; }
    Element* webkitCurrentFullScreenElement() const { return m_fullScreenElement.get(); }
    
    enum FullScreenCheckType {
        EnforceIFrameAllowFullScreenRequirement,
        ExemptIFrameAllowFullScreenRequirement,
    };

    void requestFullScreenForElement(Element*, unsigned short flags, FullScreenCheckType);
    void webkitCancelFullScreen();
    
    void webkitWillEnterFullScreenForElement(Element*);
    void webkitDidEnterFullScreenForElement(Element*);
    void webkitWillExitFullScreenForElement(Element*);
    void webkitDidExitFullScreenForElement(Element*);
    
    void setFullScreenRenderer(RenderFullScreen*);
    RenderFullScreen* fullScreenRenderer() const { return m_fullScreenRenderer; }
    void fullScreenRendererDestroyed();
    
    void setFullScreenRendererSize(const IntSize&);
    void setFullScreenRendererBackgroundColor(Color);
    
    void fullScreenChangeDelayTimerFired(Timer<Document>*);
    bool fullScreenIsAllowedForElement(Element*) const;
    void fullScreenElementRemoved();
    void removeFullScreenElementOfSubtree(Node*, bool amongChildrenOnly = false);
    bool isAnimatingFullScreen() const;
    void setAnimatingFullScreen(bool);

    // W3C API
    bool webkitFullscreenEnabled() const;
    Element* webkitFullscreenElement() const { return !m_fullScreenElementStack.isEmpty() ? m_fullScreenElementStack.last().get() : 0; }
    void webkitExitFullscreen();
#endif

#if ENABLE(POINTER_LOCK)
    void webkitExitPointerLock();
    Element* webkitPointerLockElement() const;
#endif

    // Used to allow element that loads data without going through a FrameLoader to delay the 'load' event.
    void incrementLoadEventDelayCount() { ++m_loadEventDelayCount; }
    void decrementLoadEventDelayCount();
    bool isDelayingLoadEvent() const { return m_loadEventDelayCount; }

#if ENABLE(TOUCH_EVENTS)
    PassRefPtr<Touch> createTouch(DOMWindow*, EventTarget*, int identifier, int pageX, int pageY, int screenX, int screenY, int radiusX, int radiusY, float rotationAngle, float force, ExceptionCode&) const;
#endif

    const DocumentTiming* timing() const { return &m_documentTiming; }

#if ENABLE(REQUEST_ANIMATION_FRAME)
    int requestAnimationFrame(PassRefPtr<RequestAnimationFrameCallback>);
    void cancelAnimationFrame(int id);
    void serviceScriptedAnimations(double monotonicAnimationStartTime);
#endif

    virtual EventTarget* errorEventTarget();
    virtual void logExceptionToConsole(const String& errorMessage, const String& sourceURL, int lineNumber, PassRefPtr<ScriptCallStack>);

    void initDNSPrefetch();

    unsigned wheelEventHandlerCount() const { return m_wheelEventHandlerCount; }
    void didAddWheelEventHandler();
    void didRemoveWheelEventHandler();

#if ENABLE(TOUCH_EVENTS)
    unsigned touchEventHandlerCount() const { return m_touchEventHandlerCount; }
#else
    unsigned touchEventHandlerCount() const { return 0; }
#endif

    void didAddTouchEventHandler();
    void didRemoveTouchEventHandler();

    bool visualUpdatesAllowed() const { return m_visualUpdatesAllowed; }

#if ENABLE(MICRODATA)
    PassRefPtr<NodeList> getItems(const String& typeNames);
#endif

    bool isInDocumentWrite() { return m_writeRecursionDepth > 0; }

    void suspendScheduledTasks(ActiveDOMObject::ReasonForSuspension);
    void resumeScheduledTasks();

    IntSize viewportSize() const;

#if ENABLE(CSS_DEVICE_ADAPTATION)
    IntSize initialViewportSize() const;
#endif

#if ENABLE(LINK_PRERENDER)
    Prerenderer* prerenderer() { return m_prerenderer.get(); }
#endif

#if ENABLE(TEXT_AUTOSIZING)
    TextAutosizer* textAutosizer() { return m_textAutosizer.get(); }
#endif

    void adjustFloatQuadsForScrollAndAbsoluteZoomAndFrameScale(Vector<FloatQuad>&, RenderObject*);
    void adjustFloatRectForScrollAndAbsoluteZoomAndFrameScale(FloatRect&, RenderObject*);

    void setContextFeatures(PassRefPtr<ContextFeatures>);
    ContextFeatures* contextFeatures() { return m_contextFeatures.get(); }

    virtual void reportMemoryUsage(MemoryObjectInfo*) const OVERRIDE;

    DocumentSharedObjectPool* sharedObjectPool() { return m_sharedObjectPool.get(); }

    void didRemoveAllPendingStylesheet();
    void setNeedsNotifyRemoveAllPendingStylesheet() { m_needsNotifyRemoveAllPendingStylesheet = true; }
    void clearStyleResolver();
    void notifySeamlessChildDocumentsOfStylesheetUpdate() const;

    bool inStyleRecalc() { return m_inStyleRecalc; }

    // Return a Locale for the default locale if the argument is null or empty.
    Locale& getCachedLocale(const AtomicString& locale = nullAtom);

#if ENABLE(DIALOG_ELEMENT)
    void addToTopLayer(Element*);
    void removeFromTopLayer(Element*);
    const Vector<RefPtr<Element> >& topLayerElements() const { return m_topLayerElements; }
#endif

#if ENABLE(TEMPLATE_ELEMENT)
    Document* templateContentsOwnerDocument();
#endif

    virtual void addConsoleMessage(MessageSource, MessageType, MessageLevel, const String& message, unsigned long requestIdentifier = 0);

protected:
    Document(Frame*, const KURL&, bool isXHTML, bool isHTML);

    virtual void didUpdateSecurityOrigin() OVERRIDE;

    void clearXMLVersion() { m_xmlVersion = String(); }

private:
    friend class Node;
    friend class IgnoreDestructiveWriteCountIncrementer;

    void removedLastRef();
    
    void detachParser();

    typedef void (*ArgumentsCallback)(const String& keyString, const String& valueString, Document*, void* data);
    void processArguments(const String& features, void* data, ArgumentsCallback);

    virtual bool isDocument() const { return true; }

    virtual void childrenChanged(bool changedByParser = false, Node* beforeChange = 0, Node* afterChange = 0, int childCountDelta = 0);

    virtual String nodeName() const;
    virtual NodeType nodeType() const;
    virtual bool childTypeAllowed(NodeType) const;
    virtual PassRefPtr<Node> cloneNode(bool deep);

    virtual void refScriptExecutionContext() { ref(); }
    virtual void derefScriptExecutionContext() { deref(); }

    virtual const KURL& virtualURL() const; // Same as url(), but needed for ScriptExecutionContext to implement it without a performance loss for direct calls.
    virtual KURL virtualCompleteURL(const String&) const; // Same as completeURL() for the same reason as above.

    virtual void addMessage(MessageSource, MessageType, MessageLevel, const String& message, const String& sourceURL, unsigned lineNumber, PassRefPtr<ScriptCallStack>, ScriptState* = 0, unsigned long requestIdentifier = 0);

    virtual double minimumTimerInterval() const;

    virtual double timerAlignmentInterval() const;

    void updateTitle(const StringWithDirection&);
    void updateFocusAppearanceTimerFired(Timer<Document>*);
    void updateBaseURL();

    void buildAccessKeyMap(TreeScope* root);

    void createStyleResolver();

    void seamlessParentUpdatedStylesheets();

    PassRefPtr<NodeList> handleZeroPadding(const HitTestRequest&, HitTestResult&) const;

    void loadEventDelayTimerFired(Timer<Document>*);

    void pendingTasksTimerFired(Timer<Document>*);

    static void didReceiveTask(void*);
    
    template <typename CharacterType>
    void displayBufferModifiedByEncodingInternal(CharacterType*, unsigned) const;

#if ENABLE(PAGE_VISIBILITY_API)
    PageVisibilityState visibilityState() const;
#endif

    PassRefPtr<HTMLCollection> ensureCachedCollection(CollectionType);

#if ENABLE(FULLSCREEN_API)
    void clearFullscreenElementStack();
    void popFullscreenElementStack();
    void pushFullscreenElementStack(Element*);
    void addDocumentToFullScreenChangeEventQueue(Document*);
#endif

    void setVisualUpdatesAllowed(ReadyState);
    void setVisualUpdatesAllowed(bool);
    void visualUpdatesSuppressionTimerFired(Timer<Document>*);

    void addListenerType(ListenerType listenerType) { m_listenerTypes |= listenerType; }
    void addMutationEventListenerTypeIfEnabled(ListenerType);

    int m_guardRefCount;

    void styleResolverThrowawayTimerFired(Timer<Document>*);
    Timer<Document> m_styleResolverThrowawayTimer;
    double m_lastStyleResolverAccessTime;

    OwnPtr<StyleResolver> m_styleResolver;
    bool m_didCalculateStyleResolver;
    bool m_hasDirtyStyleResolver;
    bool m_hasNodesWithPlaceholderStyle;
    bool m_needsNotifyRemoveAllPendingStylesheet;
    // But sometimes you need to ignore pending stylesheet count to
    // force an immediate layout when requested by JS.
    bool m_ignorePendingStylesheets;

    // If we do ignore the pending stylesheet count, then we need to add a boolean
    // to track that this happened so that we can do a full repaint when the stylesheets
    // do eventually load.
    PendingSheetLayout m_pendingSheetLayout;

    Frame* m_frame;
    RefPtr<DOMWindow> m_domWindow;

    RefPtr<CachedResourceLoader> m_cachedResourceLoader;
    RefPtr<DocumentParser> m_parser;
    RefPtr<ContextFeatures> m_contextFeatures;

    bool m_wellFormed;

    // Document URLs.
    KURL m_url; // Document.URL: The URL from which this document was retrieved.
    KURL m_baseURL; // Node.baseURI: The URL to use when resolving relative URLs.
    KURL m_baseURLOverride; // An alternative base URL that takes precedence over m_baseURL (but not m_baseElementURL).
    KURL m_baseElementURL; // The URL set by the <base> element.
    KURL m_cookieURL; // The URL to use for cookie access.
    KURL m_firstPartyForCookies; // The policy URL for third-party cookie blocking.

    // Document.documentURI:
    // Although URL-like, Document.documentURI can actually be set to any
    // string by content.  Document.documentURI affects m_baseURL unless the
    // document contains a <base> element, in which case the <base> element
    // takes precedence.
    //
    // This property is read-only from JavaScript, but writable from Objective C.
    String m_documentURI;

    String m_baseTarget;

    RefPtr<DocumentType> m_docType;
    OwnPtr<DOMImplementation> m_implementation;

    RefPtr<CSSStyleSheet> m_elemSheet;

    bool m_printing;
    bool m_paginatedForScreen;

    bool m_ignoreAutofocus;

    CompatibilityMode m_compatibilityMode;
    bool m_compatibilityModeLocked; // This is cheaper than making setCompatibilityMode virtual.

    Color m_textColor;

    RefPtr<Node> m_focusedNode;
    RefPtr<Node> m_hoverNode;
    RefPtr<Node> m_activeNode;
    RefPtr<Element> m_documentElement;

    uint64_t m_domTreeVersion;
    static uint64_t s_globalTreeVersion;
    
    HashSet<NodeIterator*> m_nodeIterators;
    HashSet<Range*> m_ranges;

    unsigned short m_listenerTypes;

#if ENABLE(MUTATION_OBSERVERS)
    MutationObserverOptions m_mutationObserverTypes;
#endif


    OwnPtr<DocumentStyleSheetCollection> m_styleSheetCollection;
    RefPtr<StyleSheetList> m_styleSheetList;

    OwnPtr<FormController> m_formController;

    Color m_linkColor;
    Color m_visitedLinkColor;
    Color m_activeLinkColor;

    bool m_loadingSheet;
    bool m_visuallyOrdered;
    ReadyState m_readyState;
    bool m_bParsing;
    
    Timer<Document> m_styleRecalcTimer;
    bool m_pendingStyleRecalcShouldForce;
    bool m_inStyleRecalc;
    bool m_closeAfterStyleRecalc;

    bool m_gotoAnchorNeededAfterStylesheetsLoad;
    bool m_isDNSPrefetchEnabled;
    bool m_haveExplicitlyDisabledDNSPrefetch;
    bool m_frameElementsShouldIgnoreScrolling;
    bool m_containsValidityStyleRules;
    bool m_updateFocusAppearanceRestoresSelection;

    // http://www.whatwg.org/specs/web-apps/current-work/#ignore-destructive-writes-counter
    unsigned m_ignoreDestructiveWriteCount;

    StringWithDirection m_title;
    StringWithDirection m_rawTitle;
    bool m_titleSetExplicitly;
    RefPtr<Element> m_titleElement;

    OwnPtr<RenderArena> m_renderArena;

    OwnPtr<AXObjectCache> m_axObjectCache;
    OwnPtr<DocumentMarkerController> m_markers;
    
    Timer<Document> m_updateFocusAppearanceTimer;

    Element* m_cssTarget;

    // FIXME: Merge these 2 variables into an enum. Also, FrameLoader::m_didCallImplicitClose
    // is almost a duplication of this data, so that should probably get merged in too.
    // FIXME: Document::m_processingLoadEvent and DocumentLoader::m_wasOnloadHandled are roughly the same
    // and should be merged.
    bool m_processingLoadEvent;
    bool m_loadEventFinished;

    RefPtr<SerializedScriptValue> m_pendingStateObject;
    double m_startTime;
    bool m_overMinimumLayoutThreshold;
    
    OwnPtr<ScriptRunner> m_scriptRunner;

#if ENABLE(XSLT)
    OwnPtr<TransformSource> m_transformSource;
    RefPtr<Document> m_transformSourceDocument;
#endif

    int m_docID; // A unique document identifier used for things like document-specific mapped attributes.

    String m_xmlEncoding;
    String m_xmlVersion;
    unsigned m_xmlStandalone : 2;
    unsigned m_hasXMLDeclaration : 1;

    String m_contentLanguage;

    RenderObject* m_savedRenderer;
    
    RefPtr<TextResourceDecoder> m_decoder;

    InheritedBool m_designMode;

    HashSet<LiveNodeListBase*> m_listsInvalidatedAtDocument;
    unsigned m_nodeListCounts[numNodeListInvalidationTypes];

    RefPtr<XPathEvaluator> m_xpathEvaluator;

#if ENABLE(SVG)
    OwnPtr<SVGDocumentExtensions> m_svgExtensions;
#endif

#if ENABLE(DASHBOARD_SUPPORT) || ENABLE(DRAGGABLE_REGION)
    Vector<AnnotatedRegionValue> m_annotatedRegions;
    bool m_hasAnnotatedRegions;
    bool m_annotatedRegionsDirty;
#endif

    HashMap<String, RefPtr<HTMLCanvasElement> > m_cssCanvasElements;

    bool m_createRenderers;
    bool m_inPageCache;
    Vector<IconURL> m_iconURLs;

    HashSet<Element*> m_documentSuspensionCallbackElements;
    HashSet<Element*> m_mediaVolumeCallbackElements;
    HashSet<Element*> m_privateBrowsingStateChangedElements;

    HashMap<StringImpl*, Element*, CaseFoldingHash> m_elementsByAccessKey;    
    bool m_accessKeyMapValid;

    OwnPtr<SelectorQueryCache> m_selectorQueryCache;

    bool m_useSecureKeyboardEntryWhenActive;

    bool m_isXHTML;
    bool m_isHTML;

    bool m_isViewSource;
    bool m_sawElementsInKnownNamespaces;
    bool m_isSrcdocDocument;

    RenderObject* m_renderer;
    RefPtr<DocumentEventQueue> m_eventQueue;

    RefPtr<DocumentWeakReference> m_weakReference;

    HashSet<MediaCanStartListener*> m_mediaCanStartListeners;

    QualifiedName m_idAttributeName;

#if ENABLE(FULLSCREEN_API)
    bool m_areKeysEnabledInFullScreen;
    RefPtr<Element> m_fullScreenElement;
    Vector<RefPtr<Element> > m_fullScreenElementStack;
    RenderFullScreen* m_fullScreenRenderer;
    Timer<Document> m_fullScreenChangeDelayTimer;
    Deque<RefPtr<Node> > m_fullScreenChangeEventTargetQueue;
    Deque<RefPtr<Node> > m_fullScreenErrorEventTargetQueue;
    bool m_isAnimatingFullScreen;
    LayoutRect m_savedPlaceholderFrameRect;
    RefPtr<RenderStyle> m_savedPlaceholderRenderStyle;
#endif

#if ENABLE(DIALOG_ELEMENT)
    Vector<RefPtr<Element> > m_topLayerElements;
#endif

    int m_loadEventDelayCount;
    Timer<Document> m_loadEventDelayTimer;

    ViewportArguments m_viewportArguments;

    ReferrerPolicy m_referrerPolicy;

    bool m_directionSetOnDocumentElement;
    bool m_writingModeSetOnDocumentElement;

    DocumentTiming m_documentTiming;
    RefPtr<MediaQueryMatcher> m_mediaQueryMatcher;
    bool m_writeRecursionIsTooDeep;
    unsigned m_writeRecursionDepth;
    
    unsigned m_wheelEventHandlerCount;
#if ENABLE(TOUCH_EVENTS)
    unsigned m_touchEventHandlerCount;
#endif

#if ENABLE(REQUEST_ANIMATION_FRAME)
    RefPtr<ScriptedAnimationController> m_scriptedAnimationController;
#endif

    Timer<Document> m_pendingTasksTimer;
    Vector<OwnPtr<Task> > m_pendingTasks;

#if ENABLE(LINK_PRERENDER)
    OwnPtr<Prerenderer> m_prerenderer;
#endif

#if ENABLE(TEXT_AUTOSIZING)
    OwnPtr<TextAutosizer> m_textAutosizer;
#endif

    bool m_scheduledTasksAreSuspended;
    
    bool m_visualUpdatesAllowed;
    Timer<Document> m_visualUpdatesSuppressionTimer;

    RefPtr<NamedFlowCollection> m_namedFlows;

#if ENABLE(CSP_NEXT)
    RefPtr<DOMSecurityPolicy> m_domSecurityPolicy;
#endif

    void sharedObjectPoolClearTimerFired(Timer<Document>*);
    Timer<Document> m_sharedObjectPoolClearTimer;

    OwnPtr<DocumentSharedObjectPool> m_sharedObjectPool;

#ifndef NDEBUG
    bool m_didDispatchViewportPropertiesChanged;
#endif

    typedef HashMap<AtomicString, OwnPtr<Locale> > LocaleIdentifierToLocaleMap;
    LocaleIdentifierToLocaleMap m_localeCache;

#if ENABLE(TEMPLATE_ELEMENT)
    RefPtr<Document> m_templateContentsOwnerDocument;
#endif
};

inline void Document::notifyRemovePendingSheetIfNeeded()
{
    if (m_needsNotifyRemoveAllPendingStylesheet)
        didRemoveAllPendingStylesheet();
}

// Put these methods here, because they require the Document definition, but we really want to inline them.

inline bool Node::isDocumentNode() const
{
    return this == m_document;
}

inline TreeScope* Node::treeScope() const
{
    return hasRareData() ? m_data.m_rareData->treeScope() : documentInternal();
}

inline Node::Node(Document* document, ConstructionType type)
    : m_nodeFlags(type)
    , m_document(document)
    , m_previous(0)
    , m_next(0)
{
    if (document)
        document->guardRef();
#if !defined(NDEBUG) || (defined(DUMP_NODE_STATISTICS) && DUMP_NODE_STATISTICS)
    trackForDebugging();
#endif
    InspectorCounters::incrementCounter(InspectorCounters::NodeCounter);
}

Node* eventTargetNodeForDocument(Document*);

} // namespace WebCore

#endif // Document_h
