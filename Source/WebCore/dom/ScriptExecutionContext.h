/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 * Copyright (C) 2012 Google Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef ScriptExecutionContext_h
#define ScriptExecutionContext_h

#include "ActiveDOMObject.h"
#include "ConsoleTypes.h"
#include "KURL.h"
#include "ScriptCallStack.h"
#include "ScriptState.h"
#include "SecurityContext.h"
#include "Supplementable.h"
#include <wtf/Forward.h>
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/Noncopyable.h>
#include <wtf/OwnPtr.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/Threading.h>
#include <wtf/text/StringHash.h>

#if USE(JSC)
#include <runtime/JSGlobalData.h>
#endif

namespace WebCore {

class CachedScript;
class DOMTimer;
class EventListener;
class EventQueue;
class EventTarget;
class MessagePort;

#if ENABLE(BLOB)
class PublicURLManager;
#endif

#if ENABLE(BLOB)
class FileThread;
#endif

class ScriptExecutionContext : public SecurityContext, public Supplementable<ScriptExecutionContext> {
public:
    ScriptExecutionContext();
    virtual ~ScriptExecutionContext();

    virtual bool isDocument() const { return false; }
    virtual bool isWorkerContext() const { return false; }

    virtual bool isContextThread() const { return true; }
    virtual bool isJSExecutionForbidden() const = 0;

    const KURL& url() const { return virtualURL(); }
    KURL completeURL(const String& url) const { return virtualCompleteURL(url); }

    virtual String userAgent(const KURL&) const = 0;

    virtual void disableEval(const String& errorMessage) = 0;

    bool sanitizeScriptError(String& errorMessage, int& lineNumber, String& sourceURL, CachedScript* = 0);
    void reportException(const String& errorMessage, int lineNumber, const String& sourceURL, PassRefPtr<ScriptCallStack>, CachedScript* = 0);

    void addConsoleMessage(MessageSource, MessageType, MessageLevel, const String& message, const String& sourceURL, unsigned lineNumber, ScriptState* = 0, unsigned long requestIdentifier = 0);
    virtual void addConsoleMessage(MessageSource, MessageType, MessageLevel, const String& message, unsigned long requestIdentifier = 0) = 0;

#if ENABLE(BLOB)
    PublicURLManager& publicURLManager();
#endif
    // Active objects are not garbage collected even if inaccessible, e.g. because their activity may result in callbacks being invoked.
    bool canSuspendActiveDOMObjects();
    // Active objects can be asked to suspend even if canSuspendActiveDOMObjects() returns 'false' -
    // step-by-step JS debugging is one example.
    virtual void suspendActiveDOMObjects(ActiveDOMObject::ReasonForSuspension);
    virtual void resumeActiveDOMObjects();
    virtual void stopActiveDOMObjects();

    bool activeDOMObjectsAreSuspended() const { return m_activeDOMObjectsAreSuspended; }
    bool activeDOMObjectsAreStopped() const { return m_activeDOMObjectsAreStopped; }

    // Called from the constructor and destructors of ActiveDOMObject.
    void didCreateActiveDOMObject(ActiveDOMObject*, void* upcastPointer);
    void willDestroyActiveDOMObject(ActiveDOMObject*);

    // Called after the construction of an ActiveDOMObject to synchronize suspend state.
    void suspendActiveDOMObjectIfNeeded(ActiveDOMObject*);

    typedef const HashMap<ActiveDOMObject*, void*> ActiveDOMObjectsMap;
    ActiveDOMObjectsMap& activeDOMObjects() const { return m_activeDOMObjects; }

    void didCreateDestructionObserver(ContextDestructionObserver*);
    void willDestroyDestructionObserver(ContextDestructionObserver*);

    virtual void suspendScriptedAnimationControllerCallbacks() { }
    virtual void resumeScriptedAnimationControllerCallbacks() { }

    // MessagePort is conceptually a kind of ActiveDOMObject, but it needs to be tracked separately for message dispatch.
    void processMessagePortMessagesSoon();
    void dispatchMessagePortEvents();
    void createdMessagePort(MessagePort*);
    void destroyedMessagePort(MessagePort*);
    const HashSet<MessagePort*>& messagePorts() const { return m_messagePorts; }

    void ref() { refScriptExecutionContext(); }
    void deref() { derefScriptExecutionContext(); }

    class Task {
        WTF_MAKE_NONCOPYABLE(Task);
        WTF_MAKE_FAST_ALLOCATED;
    public:
        Task() { }
        virtual ~Task();
        virtual void performTask(ScriptExecutionContext*) = 0;
        // Certain tasks get marked specially so that they aren't discarded, and are executed, when the context is shutting down its message queue.
        virtual bool isCleanupTask() const { return false; }
    };

    virtual void postTask(PassOwnPtr<Task>) = 0; // Executes the task on context's thread asynchronously.

    // Creates a unique id for setTimeout, setInterval or navigator.geolocation.watchPosition.
    int newUniqueID();

    void addTimeout(int timeoutId, DOMTimer* timer) { ASSERT(!m_timeouts.contains(timeoutId)); m_timeouts.set(timeoutId, timer); }
    void removeTimeout(int timeoutId) { m_timeouts.remove(timeoutId); }
    DOMTimer* findTimeout(int timeoutId) { return m_timeouts.get(timeoutId); }

#if USE(JSC)
    JSC::JSGlobalData* globalData();
#endif

#if ENABLE(BLOB)
    FileThread* fileThread();
    void stopFileThread();
#endif

    // Interval is in seconds.
    void adjustMinimumTimerInterval(double oldMinimumTimerInterval);
    virtual double minimumTimerInterval() const;

    void didChangeTimerAlignmentInterval();
    virtual double timerAlignmentInterval() const;

    virtual EventQueue* eventQueue() const = 0;

    virtual void reportMemoryUsage(MemoryObjectInfo*) const OVERRIDE;

protected:
    class AddConsoleMessageTask : public Task {
    public:
        static PassOwnPtr<AddConsoleMessageTask> create(MessageSource source, MessageType type, MessageLevel level, const String& message)
        {
            return adoptPtr(new AddConsoleMessageTask(source, type, level, message));
        }
        virtual void performTask(ScriptExecutionContext*);
    private:
        AddConsoleMessageTask(MessageSource source, MessageType type, MessageLevel level, const String& message)
            : m_source(source)
            , m_type(type)
            , m_level(level)
            , m_message(message.isolatedCopy())
        {
        }
        MessageSource m_source;
        MessageType m_type;
        MessageLevel m_level;
        String m_message;
    };

private:
    virtual const KURL& virtualURL() const = 0;
    virtual KURL virtualCompleteURL(const String&) const = 0;

    virtual void addMessage(MessageSource, MessageType, MessageLevel, const String& message, const String& sourceURL, unsigned lineNumber, PassRefPtr<ScriptCallStack>, ScriptState* = 0, unsigned long requestIdentifier = 0) = 0;
    virtual EventTarget* errorEventTarget() = 0;
    virtual void logExceptionToConsole(const String& errorMessage, const String& sourceURL, int lineNumber, PassRefPtr<ScriptCallStack>) = 0;
    bool dispatchErrorEvent(const String& errorMessage, int lineNumber, const String& sourceURL, CachedScript*);

    void closeMessagePorts();

    virtual void refScriptExecutionContext() = 0;
    virtual void derefScriptExecutionContext() = 0;

    HashSet<MessagePort*> m_messagePorts;
    HashSet<ContextDestructionObserver*> m_destructionObservers;
    HashMap<ActiveDOMObject*, void*> m_activeDOMObjects;
    bool m_iteratingActiveDOMObjects;
    bool m_inDestructor;

    int m_sequentialID;
    typedef HashMap<int, DOMTimer*> TimeoutMap;
    TimeoutMap m_timeouts;

    bool m_inDispatchErrorEvent;
    class PendingException;
    OwnPtr<Vector<OwnPtr<PendingException> > > m_pendingExceptions;

    bool m_activeDOMObjectsAreSuspended;
    ActiveDOMObject::ReasonForSuspension m_reasonForSuspendingActiveDOMObjects;
    bool m_activeDOMObjectsAreStopped;

#if ENABLE(BLOB)
    OwnPtr<PublicURLManager> m_publicURLManager;
    RefPtr<FileThread> m_fileThread;
#endif
};

} // namespace WebCore

#endif // ScriptExecutionContext_h
