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

#include <boost/python.hpp>

extern "C" {
#include <cpu/tb.h>
}

#include <s2e/Python/Callbacks.h>
#include <s2e/Python/ExecutionSignal.h>
#include <s2e/Python/Signals.h>
#include <s2e/S2EExecutionState.h>

///
/// \file Callbacks.cpp
/// \brief Implementation for each callback operator defined in Callbacks.h.
///

namespace py = boost::python;

namespace s2e {
namespace python {

void OnEngineShutdownCallback::operator()() {
    m_callback();
}

void OnTranslateBlockStartCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state, TranslationBlock *tb,
                                               uint64_t pc) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc);
}

void OnTranslateBlockEndCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state, TranslationBlock *tb,
                                             uint64_t pc, bool validTarget, uint64_t targetPc) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc, validTarget, targetPc);
}

void OnTranslateBlockCompleteCallback::operator()(S2EExecutionState *state, TranslationBlock *tb, uint64_t pc) {
    m_callback(py::ptr(state), py::ptr(tb), pc);
}

void OnTranslateInstructionStartCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state,
                                                     TranslationBlock *tb, uint64_t pc) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc);
}

void OnTranslateInstructionEndCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state,
                                                   TranslationBlock *tb, uint64_t pc) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc);
}

void OnTranslateSpecialInstructionEndCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state,
                                                          TranslationBlock *tb, uint64_t pc,
                                                          enum special_instruction_t instType) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc, instType);
}

void OnTranslateRegisterAccessEndCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state,
                                                      TranslationBlock *tb, uint64_t pc, uint64_t regRead,
                                                      uint64_t regWrite, bool memAccess) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc, regRead, regWrite, memAccess);
}

void OnTranslateJumpStartCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state, TranslationBlock *tb,
                                              uint64_t pc, int jumpType) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc, jumpType);
}

void OnTranslateICTIStartCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state, TranslationBlock *tb,
                                              uint64_t pc, int rm, int op, int offset) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc, rm, op, offset);
}

void OnTranslateLeaRipRelativeCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state,
                                                   TranslationBlock *tb, uint64_t pc, uint64_t targetAddr) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc, targetAddr);
}

void OnExceptionCallback::operator()(S2EExecutionState *state, unsigned exceptionIndex, uint64_t pc) {
    m_callback(py::ptr(state), exceptionIndex, pc);
}

void OnCustomInstructionCallback::operator()(S2EExecutionState *state, uint64_t arg) {
    m_callback(py::ptr(state), arg);
}

void OnTranslateSoftInterruptStartCallback::operator()(ExecutionSignal *signal, S2EExecutionState *state,
                                                       TranslationBlock *tb, uint64_t pc, unsigned interruptVector) {
    // Update the wrapped execution signal
    m_executionSignal.saveSignal(signal);

    m_callback(py::ptr(&m_executionSignal), py::ptr(state), py::ptr(tb), pc, interruptVector);
}

void OnBeforeSymbolicDataMemoryAccessCallback::operator()(S2EExecutionState *state, klee::ref<klee::Expr> virtualAddr,
                                                          klee::ref<klee::Expr> value, bool isWrite) {
    // TODO KLEE wrappers
}

void OnAfterSymbolicDataMemoryAccessCallback::operator()(S2EExecutionState *state, klee::ref<klee::Expr> virtualAddr,
                                                         klee::ref<klee::Expr> hostAddress, klee::ref<klee::Expr> value,
                                                         unsigned flags) {
    // TODO KLEE wrappers
}

void OnSymbolicAddressCallback::operator()(S2EExecutionState *state, klee::ref<klee::Expr> virtualAddr,
                                           uint64_t concreteAddr, bool &concretize,
                                           CorePlugin::symbolicAddressReason reason) {
    // TODO KLEE wrappers
}

void OnConcreteDataMemoryAccessCallback::operator()(S2EExecutionState *state, uint64_t virtualAddr, uint64_t value,
                                                    uint8_t size, unsigned flags) {
    m_callback(py::ptr(state), virtualAddr, value, size, flags);
}

void OnPortAccessCallback::operator()(S2EExecutionState *state, klee::ref<klee::Expr> port, klee::ref<klee::Expr> value,
                                      bool isWrite) {
    // TODO KLEE wrappers
}

void OnSymbolicVariableCreationCallback::operator()(S2EExecutionState *state, const std::string &originalName,
                                                    const std::vector<klee::ref<klee::Expr>> &expr,
                                                    const klee::MemoryObject *memObj, const klee::Array *array) {
    // TODO KLEE wrappers
}

void OnTimerCallback::operator()() {
    m_callback();
}

void OnStateForkCallback::operator()(S2EExecutionState *origState, const std::vector<S2EExecutionState *> &newStates,
                                     const std::vector<klee::ref<klee::Expr>> &newConditions) {
    // TODO KLEE wrappers
}

void OnStateMergeCallback::operator()(S2EExecutionState *destination, S2EExecutionState *source) {
    m_callback(py::ptr(destination), py::ptr(source));
}

void OnStateSwitchCallback::operator()(S2EExecutionState *currentState, S2EExecutionState *nextState) {
    m_callback(py::ptr(currentState), py::ptr(nextState));
}

void OnStateKillCallback::operator()(S2EExecutionState *state) {
    m_callback(py::ptr(state));
}

void OnProcessForkDecideCallback::operator()(bool *allowFork) {
    // The Python callback must return a boolean value indicating whether the fork is allowed. This boolean value is
    // then assigned back to the allowFork pointer
    bool res = py::extract<bool>(m_callback());

    if (res) {
        *allowFork = true;
    } else {
        *allowFork = false;
    }
}

void OnStateForkDecideCallback::operator()(S2EExecutionState *state, bool *allowFork) {
    // The Python callback must return a boolean value indicating whether the fork is allowed. This boolean value is
    // then assigned back to the allowFork pointer
    bool res = py::extract<bool>(m_callback(py::ptr(state)));

    if (res) {
        *allowFork = true;
    } else {
        *allowFork = false;
    }
}

void OnProcessForkCallback::operator()(bool prefork, bool isChild, unsigned parentPid) {
    m_callback(prefork, isChild, parentPid);
}

void OnStatesSplitCallback::operator()(klee::StateSet &parent, klee::StateSet &child) {
    // TODO KLEE wrappers
}

void OnProcessForkCompleteCallback::operator()(bool isChild) {
    m_callback(isChild);
}

void OnTlbMissCallback::operator()(S2EExecutionState *state, uint64_t address, bool isWrite) {
    m_callback(py::ptr(state), address, isWrite);
}

void OnPageFaultCallback::operator()(S2EExecutionState *state, uint64_t address, bool isWrite) {
    m_callback(py::ptr(state), address, isWrite);
}

void OnPrivilegeChangeCallback::operator()(S2EExecutionState *state, unsigned prevLevel, unsigned curLevel) {
    m_callback(py::ptr(state), prevLevel, curLevel);
}

void OnPageDirectoryChangeCallback::operator()(S2EExecutionState *state, uint64_t prevPageDir, uint64_t curPageDir) {
    m_callback(py::ptr(state), prevPageDir, curPageDir);
}

void OnInitializationCompleteCallback::operator()(S2EExecutionState *state) {
    m_callback(py::ptr(state));
}

void OnUpdateStatesCallback::operator()(S2EExecutionState *state, const klee::StateSet &addedStates,
                                        const klee::StateSet &removedStates) {
    // TODO KLEE wrappers
}

void OnAddressSpaceChangeCallback::operator()(S2EExecutionState *state, const klee::MemoryObject *memObj,
                                              const klee::ObjectState *oldState, klee::ObjectState *newState) {
    // TODO KLEE wrappers
}

void OnCallReturnTranslateCallback::operator()(S2EExecutionState *state, uint64_t pc, bool isCall, bool *instrument) {
    // The Python callback must return a boolean value indicating whether to instrument or not. This boolean value is
    // then assigned back to the allowFork pointer
    bool res = py::extract<bool>(m_callback(py::ptr(state), pc, isCall));

    if (res) {
        *instrument = true;
    } else {
        *instrument = false;
    }
}

} // namespace python
} // namespace s2e
