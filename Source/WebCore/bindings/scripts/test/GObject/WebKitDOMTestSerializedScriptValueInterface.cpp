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
#include "WebKitDOMTestSerializedScriptValueInterface.h"

#include "DOMObjectCache.h"
#include "ExceptionCode.h"
#include "JSMainThreadExecState.h"
#include "WebKitDOMArrayPrivate.h"
#include "WebKitDOMBinding.h"
#include "WebKitDOMMessagePortArrayPrivate.h"
#include "WebKitDOMSerializedScriptValuePrivate.h"
#include "WebKitDOMTestSerializedScriptValueInterfacePrivate.h"
#include "gobject/ConvertToUTF8String.h"
#include "webkitglobalsprivate.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_TYPE_DOM_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE, WebKitDOMTestSerializedScriptValueInterfacePrivate)

typedef struct _WebKitDOMTestSerializedScriptValueInterfacePrivate {
#if ENABLE(Condition1) || ENABLE(Condition2)
    RefPtr<WebCore::TestSerializedScriptValueInterface> coreObject;
#endif // ENABLE(Condition1) || ENABLE(Condition2)
} WebKitDOMTestSerializedScriptValueInterfacePrivate;

#if ENABLE(Condition1) || ENABLE(Condition2)

namespace WebKit {

WebKitDOMTestSerializedScriptValueInterface* kit(WebCore::TestSerializedScriptValueInterface* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(ret);

    return wrapTestSerializedScriptValueInterface(obj);
}

WebCore::TestSerializedScriptValueInterface* core(WebKitDOMTestSerializedScriptValueInterface* request)
{
    return request ? static_cast<WebCore::TestSerializedScriptValueInterface*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

WebKitDOMTestSerializedScriptValueInterface* wrapTestSerializedScriptValueInterface(WebCore::TestSerializedScriptValueInterface* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(g_object_new(WEBKIT_TYPE_DOM_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE, "core-object", coreObject, NULL));
}

} // namespace WebKit

#endif // ENABLE(Condition1) || ENABLE(Condition2)

G_DEFINE_TYPE(WebKitDOMTestSerializedScriptValueInterface, webkit_dom_test_serialized_script_value_interface, WEBKIT_TYPE_DOM_OBJECT)

enum {
    PROP_0,
    PROP_VALUE,
    PROP_READONLY_VALUE,
    PROP_CACHED_VALUE,
    PROP_PORTS,
    PROP_CACHED_READONLY_VALUE,
};

static void webkit_dom_test_serialized_script_value_interface_finalize(GObject* object)
{
    WebKitDOMTestSerializedScriptValueInterfacePrivate* priv = WEBKIT_DOM_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE_GET_PRIVATE(object);
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebKit::DOMObjectCache::forget(priv->coreObject.get());
#endif // ENABLE(Condition1) || ENABLE(Condition2)
    priv->~WebKitDOMTestSerializedScriptValueInterfacePrivate();
    G_OBJECT_CLASS(webkit_dom_test_serialized_script_value_interface_parent_class)->finalize(object);
}

static void webkit_dom_test_serialized_script_value_interface_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebKitDOMTestSerializedScriptValueInterface* self = WEBKIT_DOM_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(object);
    WebCore::TestSerializedScriptValueInterface* coreSelf = WebKit::core(self);
#endif // ENABLE(Condition1) || ENABLE(Condition2)
    switch (propertyId) {
    case PROP_VALUE: {
#if ENABLE(Condition1) || ENABLE(Condition2)
        RefPtr<WebCore::SerializedScriptValue> ptr = coreSelf->value();
        g_value_set_object(value, WebKit::kit(ptr.get()));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_READONLY_VALUE: {
#if ENABLE(Condition1) || ENABLE(Condition2)
        RefPtr<WebCore::SerializedScriptValue> ptr = coreSelf->readonlyValue();
        g_value_set_object(value, WebKit::kit(ptr.get()));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_CACHED_VALUE: {
#if ENABLE(Condition1) || ENABLE(Condition2)
        RefPtr<WebCore::SerializedScriptValue> ptr = coreSelf->cachedValue();
        g_value_set_object(value, WebKit::kit(ptr.get()));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_PORTS: {
#if ENABLE(Condition1) || ENABLE(Condition2)
        RefPtr<WebCore::MessagePortArray> ptr = coreSelf->ports();
        g_value_set_object(value, WebKit::kit(ptr.get()));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_CACHED_READONLY_VALUE: {
#if ENABLE(Condition1) || ENABLE(Condition2)
        RefPtr<WebCore::SerializedScriptValue> ptr = coreSelf->cachedReadonlyValue();
        g_value_set_object(value, WebKit::kit(ptr.get()));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_test_serialized_script_value_interface_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_test_serialized_script_value_interface_parent_class)->constructor(type, constructPropertiesCount, constructProperties);
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebKitDOMTestSerializedScriptValueInterfacePrivate* priv = WEBKIT_DOM_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE_GET_PRIVATE(object);
    priv->coreObject = static_cast<WebCore::TestSerializedScriptValueInterface*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    WebKit::DOMObjectCache::put(priv->coreObject.get(), object);
#endif // ENABLE(Condition1) || ENABLE(Condition2)
    return object;
}

static void webkit_dom_test_serialized_script_value_interface_class_init(WebKitDOMTestSerializedScriptValueInterfaceClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(WebKitDOMTestSerializedScriptValueInterfacePrivate));
    gobjectClass->constructor = webkit_dom_test_serialized_script_value_interface_constructor;
    gobjectClass->finalize = webkit_dom_test_serialized_script_value_interface_finalize;
    gobjectClass->get_property = webkit_dom_test_serialized_script_value_interface_get_property;

    g_object_class_install_property(gobjectClass,
                                    PROP_VALUE,
                                    g_param_spec_object("value", /* name */
                                                           "test_serialized_script_value_interface_value", /* short description */
                                                           "read-write  WebKitDOMSerializedScriptValue* TestSerializedScriptValueInterface.value", /* longer - could do with some extra doc stuff here */
                                                           WEBKIT_TYPE_DOM_SERIALIZED_SCRIPT_VALUE, /* gobject type */
                                                           WEBKIT_PARAM_READWRITE));
    g_object_class_install_property(gobjectClass,
                                    PROP_READONLY_VALUE,
                                    g_param_spec_object("readonly-value", /* name */
                                                           "test_serialized_script_value_interface_readonly-value", /* short description */
                                                           "read-only  WebKitDOMSerializedScriptValue* TestSerializedScriptValueInterface.readonly-value", /* longer - could do with some extra doc stuff here */
                                                           WEBKIT_TYPE_DOM_SERIALIZED_SCRIPT_VALUE, /* gobject type */
                                                           WEBKIT_PARAM_READABLE));
    g_object_class_install_property(gobjectClass,
                                    PROP_CACHED_VALUE,
                                    g_param_spec_object("cached-value", /* name */
                                                           "test_serialized_script_value_interface_cached-value", /* short description */
                                                           "read-write  WebKitDOMSerializedScriptValue* TestSerializedScriptValueInterface.cached-value", /* longer - could do with some extra doc stuff here */
                                                           WEBKIT_TYPE_DOM_SERIALIZED_SCRIPT_VALUE, /* gobject type */
                                                           WEBKIT_PARAM_READWRITE));
    g_object_class_install_property(gobjectClass,
                                    PROP_PORTS,
                                    g_param_spec_object("ports", /* name */
                                                           "test_serialized_script_value_interface_ports", /* short description */
                                                           "read-only  WebKitDOMMessagePortArray* TestSerializedScriptValueInterface.ports", /* longer - could do with some extra doc stuff here */
                                                           WEBKIT_TYPE_DOM_MESSAGE_PORT_ARRAY, /* gobject type */
                                                           WEBKIT_PARAM_READABLE));
    g_object_class_install_property(gobjectClass,
                                    PROP_CACHED_READONLY_VALUE,
                                    g_param_spec_object("cached-readonly-value", /* name */
                                                           "test_serialized_script_value_interface_cached-readonly-value", /* short description */
                                                           "read-only  WebKitDOMSerializedScriptValue* TestSerializedScriptValueInterface.cached-readonly-value", /* longer - could do with some extra doc stuff here */
                                                           WEBKIT_TYPE_DOM_SERIALIZED_SCRIPT_VALUE, /* gobject type */
                                                           WEBKIT_PARAM_READABLE));
}

static void webkit_dom_test_serialized_script_value_interface_init(WebKitDOMTestSerializedScriptValueInterface* request)
{
    WebKitDOMTestSerializedScriptValueInterfacePrivate* priv = WEBKIT_DOM_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE_GET_PRIVATE(request);
    new (priv) WebKitDOMTestSerializedScriptValueInterfacePrivate();
}

void
webkit_dom_test_serialized_script_value_interface_accept_transfer_list(WebKitDOMTestSerializedScriptValueInterface* self, WebKitDOMSerializedScriptValue* data, WebKitDOMArray* transferList)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(self));
    g_return_if_fail(WEBKIT_DOM_IS_SERIALIZED_SCRIPT_VALUE(data));
    g_return_if_fail(WEBKIT_DOM_IS_ARRAY(transferList));
    WebCore::TestSerializedScriptValueInterface* item = WebKit::core(self);
    WebCore::SerializedScriptValue* convertedData = WebKit::core(data);
    WebCore::Array* convertedTransferList = WebKit::core(transferList);
    item->acceptTransferList(convertedData, convertedTransferList);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void
webkit_dom_test_serialized_script_value_interface_multi_transfer_list(WebKitDOMTestSerializedScriptValueInterface* self, WebKitDOMSerializedScriptValue* first, WebKitDOMArray* tx, WebKitDOMSerializedScriptValue* second, WebKitDOMArray* txx)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(self));
    g_return_if_fail(WEBKIT_DOM_IS_SERIALIZED_SCRIPT_VALUE(first));
    g_return_if_fail(WEBKIT_DOM_IS_ARRAY(tx));
    g_return_if_fail(WEBKIT_DOM_IS_SERIALIZED_SCRIPT_VALUE(second));
    g_return_if_fail(WEBKIT_DOM_IS_ARRAY(txx));
    WebCore::TestSerializedScriptValueInterface* item = WebKit::core(self);
    WebCore::SerializedScriptValue* convertedFirst = WebKit::core(first);
    WebCore::Array* convertedTx = WebKit::core(tx);
    WebCore::SerializedScriptValue* convertedSecond = WebKit::core(second);
    WebCore::Array* convertedTxx = WebKit::core(txx);
    item->multiTransferList(convertedFirst, convertedTx, convertedSecond, convertedTxx);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMSerializedScriptValue*
webkit_dom_test_serialized_script_value_interface_get_value(WebKitDOMTestSerializedScriptValueInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(self), 0);
    WebCore::TestSerializedScriptValueInterface* item = WebKit::core(self);
    RefPtr<WebCore::SerializedScriptValue> gobjectResult = WTF::getPtr(item->value());
    return WebKit::kit(gobjectResult.get());
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void
webkit_dom_test_serialized_script_value_interface_set_value(WebKitDOMTestSerializedScriptValueInterface* self, WebKitDOMSerializedScriptValue* value)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(self));
    g_return_if_fail(WEBKIT_DOM_IS_SERIALIZED_SCRIPT_VALUE(value));
    WebCore::TestSerializedScriptValueInterface* item = WebKit::core(self);
    WebCore::SerializedScriptValue* convertedValue = WebKit::core(value);
    item->setValue(convertedValue);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMSerializedScriptValue*
webkit_dom_test_serialized_script_value_interface_get_readonly_value(WebKitDOMTestSerializedScriptValueInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(self), 0);
    WebCore::TestSerializedScriptValueInterface* item = WebKit::core(self);
    RefPtr<WebCore::SerializedScriptValue> gobjectResult = WTF::getPtr(item->readonlyValue());
    return WebKit::kit(gobjectResult.get());
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMSerializedScriptValue*
webkit_dom_test_serialized_script_value_interface_get_cached_value(WebKitDOMTestSerializedScriptValueInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(self), 0);
    WebCore::TestSerializedScriptValueInterface* item = WebKit::core(self);
    RefPtr<WebCore::SerializedScriptValue> gobjectResult = WTF::getPtr(item->cachedValue());
    return WebKit::kit(gobjectResult.get());
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void
webkit_dom_test_serialized_script_value_interface_set_cached_value(WebKitDOMTestSerializedScriptValueInterface* self, WebKitDOMSerializedScriptValue* value)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(self));
    g_return_if_fail(WEBKIT_DOM_IS_SERIALIZED_SCRIPT_VALUE(value));
    WebCore::TestSerializedScriptValueInterface* item = WebKit::core(self);
    WebCore::SerializedScriptValue* convertedValue = WebKit::core(value);
    item->setCachedValue(convertedValue);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMMessagePortArray*
webkit_dom_test_serialized_script_value_interface_get_ports(WebKitDOMTestSerializedScriptValueInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(self), 0);
    WebCore::TestSerializedScriptValueInterface* item = WebKit::core(self);
    RefPtr<WebCore::MessagePortArray> gobjectResult = WTF::getPtr(item->ports());
    return WebKit::kit(gobjectResult.get());
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMSerializedScriptValue*
webkit_dom_test_serialized_script_value_interface_get_cached_readonly_value(WebKitDOMTestSerializedScriptValueInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_SERIALIZED_SCRIPT_VALUE_INTERFACE(self), 0);
    WebCore::TestSerializedScriptValueInterface* item = WebKit::core(self);
    RefPtr<WebCore::SerializedScriptValue> gobjectResult = WTF::getPtr(item->cachedReadonlyValue());
    return WebKit::kit(gobjectResult.get());
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

