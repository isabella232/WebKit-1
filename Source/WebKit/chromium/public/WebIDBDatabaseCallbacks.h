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

#ifndef WebIDBDatabaseCallbacks_h
#define WebIDBDatabaseCallbacks_h

#include "WebIDBDatabaseError.h"
#include "platform/WebCommon.h"
#include "platform/WebString.h"

namespace WebKit {

class WebIDBDatabaseCallbacks {
public:
    virtual ~WebIDBDatabaseCallbacks() { }

    virtual void onForcedClose() { WEBKIT_ASSERT_NOT_REACHED(); }
    virtual void onVersionChange(long long oldVersion, long long newVersion) { WEBKIT_ASSERT_NOT_REACHED(); }
    virtual void onVersionChange(const WebString& requestedVersion) { WEBKIT_ASSERT_NOT_REACHED(); }

    virtual void onAbort(long long transactionId, const WebIDBDatabaseError&) { WEBKIT_ASSERT_NOT_REACHED(); }
    virtual void onComplete(long long transactionId) { WEBKIT_ASSERT_NOT_REACHED(); }
};

} // namespace WebKit

#endif // WebIDBDatabaseCallbacks_h
