/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
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

#ifndef IDBDatabaseBackendProxy_h
#define IDBDatabaseBackendProxy_h

#if ENABLE(INDEXED_DATABASE)

#include "IDBDatabaseBackendInterface.h"
#include <wtf/OwnPtr.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/PassRefPtr.h>

namespace WebKit {

class WebIDBDatabase;

class IDBDatabaseBackendProxy : public WebCore::IDBDatabaseBackendInterface {
public:
    static PassRefPtr<WebCore::IDBDatabaseBackendInterface> create(PassOwnPtr<WebIDBDatabase>);
    virtual ~IDBDatabaseBackendProxy();

    virtual WebCore::IDBDatabaseMetadata metadata() const;

    virtual PassRefPtr<WebCore::IDBObjectStoreBackendInterface> createObjectStore(int64_t, const String& name, const WebCore::IDBKeyPath&, bool autoIncrement, WebCore::IDBTransactionBackendInterface*, WebCore::ExceptionCode&);
    virtual void deleteObjectStore(int64_t, WebCore::IDBTransactionBackendInterface*, WebCore::ExceptionCode&);
    // FIXME: Remove this method in https://bugs.webkit.org/show_bug.cgi?id=103923.
    virtual PassRefPtr<WebCore::IDBTransactionBackendInterface> createTransaction(int64_t, const Vector<int64_t>&, WebCore::IDBTransaction::Mode);
    virtual void createTransaction(int64_t, PassRefPtr<WebCore::IDBDatabaseCallbacks>, const Vector<int64_t>&, unsigned short mode);
    virtual void close(PassRefPtr<WebCore::IDBDatabaseCallbacks>);

    virtual void commit(int64_t);
    virtual void abort(int64_t);

private:
    IDBDatabaseBackendProxy(PassOwnPtr<WebIDBDatabase>);

    OwnPtr<WebIDBDatabase> m_webIDBDatabase;
};

} // namespace WebKit

#endif

#endif // IDBDatabaseBackendProxy_h
