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
/// \file signals.h
/// \brief Signal wrappers for \c CorePlugin signals.
///
/// Each signal listed in CorePlugin.h should have a wrapper here. The signal parameters should match exactly,
/// otherwise compilation errors will occur.
///
/// TODO generate this file automatically using a Python script - e.g. using libclang
///

#ifndef PY_SIGNALS_H_
#define PY_SIGNALS_H_

#include "Callbacks.h"
#include "Signal.h"

namespace s2e {
namespace python {

using PythonEngineShutdownSignal = PythonSignal<OnEngineShutdownCallback>;

using PythonTranslateBlockStartSignal =
    PythonSignal<OnTranslateBlockStartCallback, ExecutionSignal *, S2EExecutionState *, TranslationBlock *, uint64_t>;

using PythonTranslateBlockEndSignal = PythonSignal<OnTranslateBlockEndCallback, ExecutionSignal *, S2EExecutionState *,
                                                   TranslationBlock *, uint64_t, bool, uint64_t>;

using PythonTranslateBlockCompleteSignal =
    PythonSignal<OnTranslateBlockCompleteCallback, S2EExecutionState *, TranslationBlock *, uint64_t>;

using PythonTranslateInstructionStartSignal = PythonSignal<OnTranslateInstructionStartCallback, ExecutionSignal *,
                                                           S2EExecutionState *, TranslationBlock *, uint64_t>;

using PythonTranslateInstructionEndSignal = PythonSignal<OnTranslateInstructionEndCallback, ExecutionSignal *,
                                                         S2EExecutionState *, TranslationBlock *, uint64_t>;

using PythonTranslateSpecialInstructionEndSignal =
    PythonSignal<OnTranslateSpecialInstructionEndCallback, ExecutionSignal *, S2EExecutionState *, TranslationBlock *,
                 uint64_t, enum special_instruction_t>;

using PythonTranslateRegisterAccessEndSignal =
    PythonSignal<OnTranslateRegisterAccessEndCallback, ExecutionSignal *, S2EExecutionState *, TranslationBlock *,
                 uint64_t, uint64_t, uint64_t, bool>;

using PythonTranslateJumpStartSignal = PythonSignal<OnTranslateJumpStartCallback, ExecutionSignal *,
                                                    S2EExecutionState *, TranslationBlock *, uint64_t, int>;

using PythonTranslateICTIStartSignal = PythonSignal<OnTranslateICTIStartCallback, ExecutionSignal *,
                                                    S2EExecutionState *, TranslationBlock *, uint64_t, int, int, int>;

using PythonTranslateLeaRipRelativeSignal = PythonSignal<OnTranslateLeaRipRelativeCallback, ExecutionSignal *,
                                                         S2EExecutionState *, TranslationBlock *, uint64_t, uint64_t>;

using PythonExceptionSignal = PythonSignal<OnExceptionCallback, S2EExecutionState *, unsigned, uint64_t>;

using PythonCustomInstructionSignal = PythonSignal<OnCustomInstructionCallback, S2EExecutionState *, uint64_t>;

using PythonTranslateSoftInterruptStartSignal =
    PythonSignal<OnTranslateSoftInterruptStartCallback, ExecutionSignal *, S2EExecutionState *, TranslationBlock *,
                 uint64_t, unsigned>;

using PythonBeforeSymbolicDataMemoryAccessSignal =
    PythonSignal<OnBeforeSymbolicDataMemoryAccessCallback, S2EExecutionState *, klee::ref<klee::Expr>,
                 klee::ref<klee::Expr>, bool>;

using PythonAfterSymbolicDataMemoryAccessSignal =
    PythonSignal<OnAfterSymbolicDataMemoryAccessCallback, S2EExecutionState *, klee::ref<klee::Expr>,
                 klee::ref<klee::Expr>, klee::ref<klee::Expr>, unsigned>;

using PythonSymbolicAddressSignal = PythonSignal<OnSymbolicAddressCallback, S2EExecutionState *, klee::ref<klee::Expr>,
                                                 uint64_t, bool &, CorePlugin::symbolicAddressReason>;

using PythonConcreteDataMemoryAccessSignal =
    PythonSignal<OnConcreteDataMemoryAccessCallback, S2EExecutionState *, uint64_t, uint64_t, uint8_t, unsigned>;

using PythonPortAccessSignal =
    PythonSignal<OnPortAccessCallback, S2EExecutionState *, klee::ref<klee::Expr>, klee::ref<klee::Expr>, bool>;

using PythonSymbolicVariableCreationSignal =
    PythonSignal<OnSymbolicVariableCreationCallback, S2EExecutionState *, const std::string &,
                 const std::vector<klee::ref<klee::Expr>> &, const klee::MemoryObject *, const klee::Array *>;

using PythonTimerSignal = PythonSignal<OnTimerCallback>;

using PythonStateForkSignal =
    PythonSignal<OnStateForkCallback, S2EExecutionState *, const std::vector<S2EExecutionState *> &,
                 const std::vector<klee::ref<klee::Expr>> &>;

using PythonStateMergeSignal = PythonSignal<OnStateMergeCallback, S2EExecutionState *, S2EExecutionState *>;

using PythonStateSwitchSignal = PythonSignal<OnStateSwitchCallback, S2EExecutionState *, S2EExecutionState *>;

using PythonStateKillSignal = PythonSignal<OnStateKillCallback, S2EExecutionState *>;

using PythonProcessForkDecideSignal = PythonSignal<OnProcessForkDecideCallback, bool *>;

using PythonStateForkDecideSignal = PythonSignal<OnStateForkDecideCallback, S2EExecutionState *, bool *>;

using PythonProcessForkSignal = PythonSignal<OnProcessForkCallback, bool, bool, unsigned>;

using PythonStatesSplitSignal = PythonSignal<OnStatesSplitCallback, klee::StateSet &, klee::StateSet &>;

using PythonProcessForkCompleteSignal = PythonSignal<OnProcessForkCompleteCallback, bool>;

using PythonTlbMissSignal = PythonSignal<OnTlbMissCallback, S2EExecutionState *, uint64_t, bool>;

using PythonPageFaultSignal = PythonSignal<OnPageFaultCallback, S2EExecutionState *, uint64_t, bool>;

using PythonPrivilegeChangeSignal = PythonSignal<OnPrivilegeChangeCallback, S2EExecutionState *, unsigned, unsigned>;

using PythonPageDirectoryChangeSignal =
    PythonSignal<OnPageDirectoryChangeCallback, S2EExecutionState *, uint64_t, uint64_t>;

using PythonInitializationCompleteSignal = PythonSignal<OnInitializationCompleteCallback, S2EExecutionState *>;

using PythonUpdateStatesSignal =
    PythonSignal<OnUpdateStatesCallback, S2EExecutionState *, const klee::StateSet &, const klee::StateSet &>;

using PythonAddressSpaceChangeSignal =
    PythonSignal<OnAddressSpaceChangeCallback, S2EExecutionState *, const klee::MemoryObject *,
                 const klee::ObjectState *, klee::ObjectState *>;

using PythonCallReturnTranslateSignal =
    PythonSignal<OnCallReturnTranslateCallback, S2EExecutionState *, uint64_t, bool, bool *>;

} // namespace python
} // namespace s2e

#endif
