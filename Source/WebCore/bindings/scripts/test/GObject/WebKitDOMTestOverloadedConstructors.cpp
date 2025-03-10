/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "WebKitDOMTestOverloadedConstructors.h"

#include "DOMObjectCache.h"
#include "ExceptionCode.h"
#include "JSMainThreadExecState.h"
#include "WebKitDOMBinding.h"
#include "WebKitDOMTestOverloadedConstructorsPrivate.h"
#include "gobject/ConvertToUTF8String.h"
#include "webkitglobalsprivate.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_TEST_OVERLOADED_CONSTRUCTORS_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_TYPE_DOM_TEST_OVERLOADED_CONSTRUCTORS, WebKitDOMTestOverloadedConstructorsPrivate)

typedef struct _WebKitDOMTestOverloadedConstructorsPrivate {
    RefPtr<WebCore::TestOverloadedConstructors> coreObject;
} WebKitDOMTestOverloadedConstructorsPrivate;

namespace WebKit {

WebKitDOMTestOverloadedConstructors* kit(WebCore::TestOverloadedConstructors* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_TEST_OVERLOADED_CONSTRUCTORS(ret);

    return wrapTestOverloadedConstructors(obj);
}

WebCore::TestOverloadedConstructors* core(WebKitDOMTestOverloadedConstructors* request)
{
    return request ? static_cast<WebCore::TestOverloadedConstructors*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

WebKitDOMTestOverloadedConstructors* wrapTestOverloadedConstructors(WebCore::TestOverloadedConstructors* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_TEST_OVERLOADED_CONSTRUCTORS(g_object_new(WEBKIT_TYPE_DOM_TEST_OVERLOADED_CONSTRUCTORS, "core-object", coreObject, NULL));
}

} // namespace WebKit

G_DEFINE_TYPE(WebKitDOMTestOverloadedConstructors, webkit_dom_test_overloaded_constructors, WEBKIT_TYPE_DOM_OBJECT)

static void webkit_dom_test_overloaded_constructors_finalize(GObject* object)
{
    WebKitDOMTestOverloadedConstructorsPrivate* priv = WEBKIT_DOM_TEST_OVERLOADED_CONSTRUCTORS_GET_PRIVATE(object);

    WebKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~WebKitDOMTestOverloadedConstructorsPrivate();
    G_OBJECT_CLASS(webkit_dom_test_overloaded_constructors_parent_class)->finalize(object);
}

static GObject* webkit_dom_test_overloaded_constructors_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_test_overloaded_constructors_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    WebKitDOMTestOverloadedConstructorsPrivate* priv = WEBKIT_DOM_TEST_OVERLOADED_CONSTRUCTORS_GET_PRIVATE(object);
    priv->coreObject = static_cast<WebCore::TestOverloadedConstructors*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    WebKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_test_overloaded_constructors_class_init(WebKitDOMTestOverloadedConstructorsClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(WebKitDOMTestOverloadedConstructorsPrivate));
    gobjectClass->constructor = webkit_dom_test_overloaded_constructors_constructor;
    gobjectClass->finalize = webkit_dom_test_overloaded_constructors_finalize;
}

static void webkit_dom_test_overloaded_constructors_init(WebKitDOMTestOverloadedConstructors* request)
{
    WebKitDOMTestOverloadedConstructorsPrivate* priv = WEBKIT_DOM_TEST_OVERLOADED_CONSTRUCTORS_GET_PRIVATE(request);
    new (priv) WebKitDOMTestOverloadedConstructorsPrivate();
}

