/*
 * S2E Selective Symbolic Execution Framework
 *
 * Copyright (c) 2016, Dependable Systems Laboratory, EPFL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Dependable Systems Laboratory, EPFL nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE DEPENDABLE SYSTEMS LABORATORY, EPFL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

///
/// \file Callbacks.h
/// \brief Callback declarations for \c CorePlugin signals.
///
/// The arguments to each callback must *exactly* match those in CorePlugin.h.
///

#ifndef PY_CALLBACKS_H_
#define PY_CALLBACKS_H_

extern "C" {
struct TranslationBlock;
}

#include <boost/python/object.hpp>

#include <s2e/CorePlugin.h>

#include "Callback.h"
#include "ExecutionSignal.h"

namespace py = boost::python;

namespace s2e {

class S2EExecutionState;

namespace python {

class OnEngineShutdownCallback : public CallbackFunctor<> {
public:
    virtual void operator()();
};

class OnTranslateBlockStartCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t);
};

class OnTranslateBlockEndCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t, bool, uint64_t> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t, bool, uint64_t);
};

class OnTranslateBlockCompleteCallback : public CallbackFunctor<S2EExecutionState *, TranslationBlock *, uint64_t> {
public:
    virtual void operator()(S2EExecutionState *, TranslationBlock *, uint64_t);
};

class OnTranslateInstructionStartCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t);
};

class OnTranslateInstructionEndCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t);
};

class OnTranslateSpecialInstructionEndCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t,
                                                enum special_instruction_t> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t,
                            enum special_instruction_t);
};

class OnTranslateRegisterAccessEndCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t, uint64_t, uint64_t,
                                                bool> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t, uint64_t, uint64_t,
                            bool);
};

class OnTranslateJumpStartCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t, int> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t, int);
};

class OnTranslateICTIStartCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t, int, int, int> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t, int, int, int);
};

class OnTranslateLeaRipRelativeCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t, uint64_t> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t, uint64_t);
};

class OnExceptionCallback : public CallbackFunctor<S2EExecutionState *, unsigned, uint64_t> {
public:
    virtual void operator()(S2EExecutionState *, unsigned, uint64_t);
};

class OnCustomInstructionCallback : public CallbackFunctor<S2EExecutionState *, uint64_t> {
public:
    virtual void operator()(S2EExecutionState *, uint64_t);
};

class OnTranslateSoftInterruptStartCallback
    : public CallbackFunctorWithExecutionSignal<S2EExecutionState *, TranslationBlock *, uint64_t, unsigned> {
public:
    virtual void operator()(ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t, unsigned);
};

class OnBeforeSymbolicDataMemoryAccessCallback
    : public CallbackFunctor<S2EExecutionState *, klee::ref<klee::Expr>, klee::ref<klee::Expr>, bool> {
public:
    virtual void operator()(S2EExecutionState *, klee::ref<klee::Expr>, klee::ref<klee::Expr>, bool);
};

class OnAfterSymbolicDataMemoryAccessCallback
    : public CallbackFunctor<S2EExecutionState *, klee::ref<klee::Expr>, klee::ref<klee::Expr>, klee::ref<klee::Expr>,
                             unsigned> {
public:
    virtual void operator()(S2EExecutionState *, klee::ref<klee::Expr>, klee::ref<klee::Expr>, klee::ref<klee::Expr>,
                            unsigned);
};

class OnSymbolicAddressCallback : public CallbackFunctor<S2EExecutionState *, klee::ref<klee::Expr>, uint64_t, bool &,
                                                         CorePlugin::symbolicAddressReason> {
public:
    virtual void operator()(S2EExecutionState *, klee::ref<klee::Expr>, uint64_t, bool &,
                            CorePlugin::symbolicAddressReason);
};

class OnConcreteDataMemoryAccessCallback
    : public CallbackFunctor<S2EExecutionState *, uint64_t, uint64_t, uint8_t, unsigned> {
public:
    virtual void operator()(S2EExecutionState *, uint64_t, uint64_t, uint8_t, unsigned);
};

class OnPortAccessCallback
    : public CallbackFunctor<S2EExecutionState *, klee::ref<klee::Expr>, klee::ref<klee::Expr>, bool> {
public:
    virtual void operator()(S2EExecutionState *, klee::ref<klee::Expr>, klee::ref<klee::Expr>, bool);
};

class OnSymbolicVariableCreationCallback
    : public CallbackFunctor<S2EExecutionState *, const std::string &, const std::vector<klee::ref<klee::Expr>> &,
                             const klee::MemoryObject *, const klee::Array *> {
public:
    virtual void operator()(S2EExecutionState *, const std::string &, const std::vector<klee::ref<klee::Expr>> &,
                            const klee::MemoryObject *, const klee::Array *);
};

class OnTimerCallback : public CallbackFunctor<> {
public:
    virtual void operator()();
};

class OnStateForkCallback : public CallbackFunctor<S2EExecutionState *, const std::vector<S2EExecutionState *> &,
                                                   const std::vector<klee::ref<klee::Expr>> &> {
public:
    virtual void operator()(S2EExecutionState *, const std::vector<S2EExecutionState *> &,
                            const std::vector<klee::ref<klee::Expr>> &);
};

class OnStateMergeCallback : public CallbackFunctor<S2EExecutionState *, S2EExecutionState *> {
public:
    virtual void operator()(S2EExecutionState *, S2EExecutionState *);
};

class OnStateSwitchCallback : public CallbackFunctor<S2EExecutionState *, S2EExecutionState *> {
public:
    virtual void operator()(S2EExecutionState *, S2EExecutionState *);
};

class OnStateKillCallback : public CallbackFunctor<S2EExecutionState *> {
public:
    virtual void operator()(S2EExecutionState *);
};

class OnProcessForkDecideCallback : public CallbackFunctor<bool *> {
public:
    virtual void operator()(bool *);
};

class OnStateForkDecideCallback : public CallbackFunctor<S2EExecutionState *, bool *> {
public:
    virtual void operator()(S2EExecutionState *, bool *);
};

class OnProcessForkCallback : public CallbackFunctor<bool, bool, unsigned> {
public:
    virtual void operator()(bool, bool, unsigned);
};

class OnStatesSplitCallback : public CallbackFunctor<klee::StateSet &, klee::StateSet &> {
public:
    virtual void operator()(klee::StateSet &, klee::StateSet &);
};

class OnProcessForkCompleteCallback : public CallbackFunctor<bool> {
public:
    virtual void operator()(bool);
};

class OnTlbMissCallback : public CallbackFunctor<S2EExecutionState *, uint64_t, bool> {
public:
    virtual void operator()(S2EExecutionState *, uint64_t, bool);
};

class OnPageFaultCallback : public CallbackFunctor<S2EExecutionState *, uint64_t, bool> {
public:
    virtual void operator()(S2EExecutionState *, uint64_t, bool);
};

class OnPrivilegeChangeCallback : public CallbackFunctor<S2EExecutionState *, unsigned, unsigned> {
public:
    virtual void operator()(S2EExecutionState *, unsigned, unsigned);
};

class OnPageDirectoryChangeCallback : public CallbackFunctor<S2EExecutionState *, uint64_t, uint64_t> {
public:
    virtual void operator()(S2EExecutionState *, uint64_t, uint64_t);
};

class OnInitializationCompleteCallback : public CallbackFunctor<S2EExecutionState *> {
public:
    virtual void operator()(S2EExecutionState *);
};

class OnUpdateStatesCallback
    : public CallbackFunctor<S2EExecutionState *, const klee::StateSet &, const klee::StateSet &> {
public:
    virtual void operator()(S2EExecutionState *, const klee::StateSet &, const klee::StateSet &);
};

class OnAddressSpaceChangeCallback : public CallbackFunctor<S2EExecutionState *, const klee::MemoryObject *,
                                                            const klee::ObjectState *, klee::ObjectState *> {
public:
    virtual void operator()(S2EExecutionState *, const klee::MemoryObject *, const klee::ObjectState *,
                            klee::ObjectState *);
};

class OnCallReturnTranslateCallback : public CallbackFunctor<S2EExecutionState *, uint64_t, bool, bool *> {
public:
    virtual void operator()(S2EExecutionState *, uint64_t, bool, bool *);
};

} // namespace python
} // namespace s2e

#endif
