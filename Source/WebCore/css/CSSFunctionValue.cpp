/*
 * Copyright (C) 2008, 2010 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "CSSFunctionValue.h"

#include "CSSParserValues.h"
#include "CSSValueList.h"
#include "WebCoreMemoryInstrumentation.h"
#include <wtf/PassOwnPtr.h>
#include <wtf/text/StringBuilder.h>

namespace WebCore {

CSSFunctionValue::CSSFunctionValue(CSSParserFunction* function)
    : CSSValue(FunctionClass)
    , m_name(function->name)
{
    if (function->args)
        m_args = CSSValueList::createFromParserValueList(function->args.get());
}

CSSFunctionValue::CSSFunctionValue(String name, PassRefPtr<CSSValueList> args)
    : CSSValue(FunctionClass)
    , m_name(name)
    , m_args(args)
{
}

String CSSFunctionValue::customCssText() const
{
    StringBuilder result;
    result.append(m_name); // Includes the '('
    if (m_args)
        result.append(m_args->cssText());
    result.append(')');
    return result.toString();
}

void CSSFunctionValue::reportDescendantMemoryUsage(MemoryObjectInfo* memoryObjectInfo) const
{
    MemoryClassInfo info(memoryObjectInfo, this, WebCoreMemoryTypes::CSS);
    info.addMember(m_name);
    info.addMember(m_args);
}

}
