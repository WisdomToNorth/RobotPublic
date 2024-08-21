#pragma once
#ifndef RF_SIGNAL_H
#define RF_SIGNAL_H

// 'emit' macro might have been defined by Qt as a pseudo additional keyword
// It's temporarily undefined in this header for the sake of KBindings::Signal which provides member
// function emit()
#ifdef emit
#undef emit
#define RF_QT_EMIT_TO_BE_RESTORED
#endif

#include <any>
#include <memory>
#include <thread>

#include <kdbindings/signal.h>

namespace rfapp
{

using SignalConnectionHandle = KDBindings::ConnectionHandle;

// Provides an interface to deal with signal/slot thread mismatch
//
// Typical example is to connect a signal to a slot in main(UI) thread but later on the signal is
// emitted from within a worker thread. In such case slots would be executed by the worker thread
// which might cause troubles
// ISignalThreadHelper is used by Signal::connectSlot() to make sure the slots are executed in the
// thread where the initial connection was created.
class ISignalThreadHelper
{
public:
    virtual ~ISignalThreadHelper() = default;

    // Get implementation-specific context object for the current thread
    virtual std::any getCurrentThreadContext() = 0;

    // Executes function 'fn' in target thread referred by 'context'
    // If current thread is different from target thread then implementation has to ensure 'fn' is
    // executed in target thread(eg by enqueuing 'fn' in some event loop)
    // 'context' is the implementation-specific thread context object identified in a previous call
    // to getCurrentThreadContext()
    virtual void execInThread(const std::any &context, const std::function<void()> &fn) = 0;
};

// Getter/setter functions of the global ISignalThreadHelper object used by Signal::connectSlot()
// ISignalThreadHelper is optional(by default getGlobalSignalThreadHelper() returns nullptr) in that
// case expect signal/slot thread mismatches(which might be fine with user client-code)
ISignalThreadHelper *getGlobalSignalThreadHelper();
void setGlobalSignalThreadHelper(std::unique_ptr<ISignalThreadHelper> helper);

// Provides a mechanism for communication between objects
// Based on KDBindings::Signal<>
//
// Prefer connectSlot() in client-code instead of KDBindings::Signal::connect() because it
// uses ISignalThreadHelper to handle signal/slot thread mismatch
// On signal emission KDBindings::Signal makes direct call to the connected slot functions which is
// cause of problems
template <typename... Args>
class RfSignal : public KDBindings::Signal<Args...>
{
public:
    RfSignal() = default;
    RfSignal(const RfSignal &) = delete;
    RfSignal &operator=(RfSignal const &other) = delete;
    RfSignal(RfSignal &&other) noexcept = default;
    RfSignal &operator=(RfSignal &&other) noexcept = default;

    // Emits the Signal, which causes all connected slots to be called, as long as they are not
    // blocked.
    // The arguments provided to emit will be passed to each slot by copy, therefore consider
    // using (const) references as the Args to the Signal wherever possible.
    void send(Args... p) const
    {
        this->emit(p...);
    }

    // Connects 'fnSlot' to the signal
    // When send() is called, the functions will be executed with the arguments provided to send()
    SignalConnectionHandle connectSlot(const std::function<void(Args...)> &fnSlot)
    {
        if (getGlobalSignalThreadHelper())
        {
            auto threadContext = getGlobalSignalThreadHelper()->getCurrentThreadContext();
            auto connectThreadId = std::this_thread::get_id();
            auto fnWrap = [=](Args... args)
            {
                auto emitThreadId = std::this_thread::get_id();
                if (emitThreadId == connectThreadId)
                    fnSlot(args...);
                else
                    getGlobalSignalThreadHelper()->execInThread(threadContext,
                                                                [=] { fnSlot(args...); });
            };
            return this->connect(fnWrap);
        }
        else
        {
            return this->connect(fnSlot);
        }
    }

    template <typename FunctionSlot, typename... FunctionSlotArgs>

    typename std::enable_if<
        std::disjunction_v<
            std::negation<std::is_convertible<FunctionSlot, std::function<void(Args...)>>>,
            std::integral_constant<bool, sizeof...(FunctionSlotArgs)>>,
        SignalConnectionHandle>::type

    connectSlot(FunctionSlot &&fnSlot, FunctionSlotArgs &&...args)
    {
        std::function<void(Args...)> bound = KDBindings::Private::bind_first(
            std::forward<FunctionSlot>(fnSlot), std::forward<FunctionSlotArgs>(args)...);
        return this->connectSlot(bound);
    }
};

} // namespace rfapp

// Restore 'emit' Qt macro if it was undefined at the beginning of this header
#ifdef RF_QT_EMIT_TO_BE_RESTORED
#define emit
#endif
#undef RF_QT_EMIT_TO_BE_RESTORED

#endif
