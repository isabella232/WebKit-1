/*
 * Copyright (C) 2004, 2005, 2006, 2008 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2006 Rob Buis <buis@kde.org>
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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
 */

#ifndef SVGElement_h
#define SVGElement_h

#if ENABLE(SVG)
#include "SVGLocatable.h"
#include "SVGParsingError.h"
#include "SVGPropertyInfo.h"
#include "StyledElement.h"
#include "Timer.h"
#include <wtf/HashMap.h>

namespace WebCore {

class AffineTransform;
class CSSCursorImageValue;
class Document;
class SVGAttributeToPropertyMap;
class SVGCursorElement;
class SVGDocumentExtensions;
class SVGElementInstance;
class SVGElementRareData;
class SVGSVGElement;

class SVGElement : public StyledElement {
public:
    static PassRefPtr<SVGElement> create(const QualifiedName&, Document*);
    virtual ~SVGElement();

    bool isOutermostSVGSVGElement() const;

    String xmlbase() const;
    void setXmlbase(const String&, ExceptionCode&);

    SVGSVGElement* ownerSVGElement() const;
    SVGElement* viewportElement() const;

    SVGDocumentExtensions* accessDocumentSVGExtensions();

    virtual bool isStyled() const { return false; }
    virtual bool isStyledTransformable() const { return false; }
    virtual bool isStyledLocatable() const { return false; }
    virtual bool isSVG() const { return false; }
    virtual bool isFilterEffect() const { return false; }
    virtual bool isGradientStop() const { return false; }
    virtual bool isTextContent() const { return false; }

    // For SVGTests
    virtual bool isValid() const { return true; }

    virtual void svgAttributeChanged(const QualifiedName&) { }

    virtual void animatedPropertyTypeForAttribute(const QualifiedName&, Vector<AnimatedPropertyType>&);

    void sendSVGLoadEventIfPossible(bool sendParentLoadEvents = false);
    void sendSVGLoadEventIfPossibleAsynchronously();
    void svgLoadEventTimerFired(Timer<SVGElement>*);
    virtual Timer<SVGElement>* svgLoadEventTimer();

    virtual AffineTransform* supplementalTransform() { return 0; }

    void invalidateSVGAttributes() { ensureAttributeData()->m_animatedSVGAttributesAreDirty = true; }

    const HashSet<SVGElementInstance*>& instancesForElement() const;

    bool getBoundingBox(FloatRect&, SVGLocatable::StyleUpdateStrategy = SVGLocatable::AllowStyleUpdate);

    void setCursorElement(SVGCursorElement*);
    void cursorElementRemoved();
    void setCursorImageValue(CSSCursorImageValue*);
    void cursorImageValueRemoved();

    SVGElement* correspondingElement();
    void setCorrespondingElement(SVGElement*);

    virtual void updateAnimatedSVGAttribute(const QualifiedName&) const;
 
    virtual PassRefPtr<RenderStyle> customStyleForRenderer() OVERRIDE;

    static void synchronizeRequiredFeatures(void* contextElement);
    static void synchronizeRequiredExtensions(void* contextElement);
    static void synchronizeSystemLanguage(void* contextElement);

    virtual void synchronizeRequiredFeatures() { }
    virtual void synchronizeRequiredExtensions() { }
    virtual void synchronizeSystemLanguage() { }

    virtual SVGAttributeToPropertyMap& localAttributeToPropertyMap();

#ifndef NDEBUG
    static bool isAnimatableAttribute(const QualifiedName&);
#endif

    StylePropertySet* animatedSMILStyleProperties() const;
    StylePropertySet* ensureAnimatedSMILStyleProperties();
    void setUseOverrideComputedStyle(bool);

    virtual bool haveLoadedRequiredResources();

    virtual bool addEventListener(const AtomicString& eventType, PassRefPtr<EventListener>, bool useCapture) OVERRIDE;
    virtual bool removeEventListener(const AtomicString& eventType, EventListener*, bool useCapture) OVERRIDE;

protected:
    SVGElement(const QualifiedName&, Document*, ConstructionType = CreateSVGElement);

    virtual void parseAttribute(const QualifiedName&, const AtomicString&) OVERRIDE;

    virtual void finishParsingChildren();
    virtual void attributeChanged(const QualifiedName&, const AtomicString&) OVERRIDE;
    virtual bool childShouldCreateRenderer(const NodeRenderingContext&) const OVERRIDE;
    
    virtual void removedFrom(ContainerNode*) OVERRIDE;

    SVGElementRareData* svgRareData() const;
    SVGElementRareData* ensureSVGRareData();

    void reportAttributeParsingError(SVGParsingError, const QualifiedName&, const AtomicString&);

    // FIXME: Author shadows should be allowed
    // https://bugs.webkit.org/show_bug.cgi?id=77938
    virtual bool areAuthorShadowsAllowed() const OVERRIDE { return false; }

private:
    friend class SVGElementInstance;

    RenderStyle* computedStyle(PseudoId = NOPSEUDO);
    virtual RenderStyle* virtualComputedStyle(PseudoId pseudoElementSpecifier = NOPSEUDO) { return computedStyle(pseudoElementSpecifier); }
    virtual bool willRecalcStyle(StyleChange);

    virtual bool rendererIsNeeded(const NodeRenderingContext&) { return false; }

    virtual bool isSupported(StringImpl* feature, StringImpl* version) const;

    void mapInstanceToElement(SVGElementInstance*);
    void removeInstanceMapping(SVGElementInstance*);

};

struct SVGAttributeHashTranslator {
    static unsigned hash(const QualifiedName& key)
    {
        if (key.hasPrefix()) {
            QualifiedNameComponents components = { nullAtom.impl(), key.localName().impl(), key.namespaceURI().impl() };
            return hashComponents(components);
        }
        return DefaultHash<QualifiedName>::Hash::hash(key);
    }
    static bool equal(const QualifiedName& a, const QualifiedName& b) { return a.matches(b); }
};

inline SVGElement* toSVGElement(Element* element)
{
    ASSERT(!element || element->isSVGElement());
    return static_cast<SVGElement*>(element);
}

}

#endif
#endif
