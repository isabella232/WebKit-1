/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2000 Frederik Holljen (frederik.holljen@hig.no)
 * Copyright (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2004, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef NodeFilter_h
#define NodeFilter_h

#include "DOMWrapperWorld.h"
#include "NodeFilterCondition.h"
#include <wtf/RefPtr.h>

namespace WebCore {

    class NodeFilter : public RefCounted<NodeFilter> {
    public:
        /**
         * The following constants are returned by the acceptNode()
         * method:
         */
        enum {
            FILTER_ACCEPT = 1,
            FILTER_REJECT = 2,
            FILTER_SKIP   = 3
        };

        /**
         * These are the available values for the whatToShow parameter.
         * They are the same as the set of possible types for Node, and
         * their values are derived by using a bit position corresponding
         * to the value of NodeType for the equivalent node type.
         */
        enum {
            SHOW_ALL                       = 0xFFFFFFFF,
            SHOW_ELEMENT                   = 0x00000001,
            SHOW_ATTRIBUTE                 = 0x00000002,
            SHOW_TEXT                      = 0x00000004,
            SHOW_CDATA_SECTION             = 0x00000008,
            SHOW_ENTITY_REFERENCE          = 0x00000010,
            SHOW_ENTITY                    = 0x00000020,
            SHOW_PROCESSING_INSTRUCTION    = 0x00000040,
            SHOW_COMMENT                   = 0x00000080,
            SHOW_DOCUMENT                  = 0x00000100,
            SHOW_DOCUMENT_TYPE             = 0x00000200,
            SHOW_DOCUMENT_FRAGMENT         = 0x00000400,
            SHOW_NOTATION                  = 0x00000800
        };

        static PassRefPtr<NodeFilter> create(PassRefPtr<NodeFilterCondition> condition)
        {
            return adoptRef(new NodeFilter(condition));
        }

        static PassRefPtr<NodeFilter> create()
        {
            return adoptRef(new NodeFilter());
        }

        short acceptNode(ScriptState*, Node*) const;

        // Do not call these functions. They are just scaffolding to support the Objective-C bindings.
        // They operate in the main thread normal world, and they swallow JS exceptions.
        short acceptNode(Node* node) const { return acceptNode(scriptStateFromNode(mainThreadNormalWorld(), node), node); }
        
        void setCondition(PassRefPtr<NodeFilterCondition> condition) { ASSERT(!m_condition); m_condition = condition; }
        NodeFilterCondition* condition() const { return m_condition.get(); }

    private:
        explicit NodeFilter(PassRefPtr<NodeFilterCondition> condition) : m_condition(condition) { }
        NodeFilter() {}

        RefPtr<NodeFilterCondition> m_condition;
    };

} // namespace WebCore

#endif // NodeFilter_h
