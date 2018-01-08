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

#include <s2e/Python/Signals.h>
#include <s2e/S2E.h>
#include <s2e/S2EExecutionState.h>

#include "Utils.h"
#include "export_signals.h"

namespace py = boost::python;

///
/// \file export_signals.cpp
/// \brief Exports signal-related classes to the \c s2e.core.signals Python module.
///

///
/// \brief Generates a Python class for the given signal and exposes a module-level object.
///
/// The Python class includes \c connect and \c disconnect methods.
///
/// \param C Name of the signal. E.g. if C = TranslateBlockStart, then the signal type \c TranslateBlockStart will be
/// exposed to the Python interpreter.
/// \param EXPORTED_NAME The name of the module-level object that users can connect callbacks to. To conform with PEP8,
/// this will typically be a snake-case version of the \c C parameter.
///
#define EXPORT_CORE_SIGNAL(C, EXPORTED_NAME)                \
    py::class_<Python##C##Signal>(#C "Signal", py::no_init) \
        .def("connect", &Python##C##Signal::connect)        \
        .def("disconnect", &Python##C##Signal::disconnect); \
    py::scope().attr("on_" #EXPORTED_NAME) = Python##C##Signal(&g_s2e->getCorePlugin()->on##C)

namespace s2e {
namespace python {

/// Export \c s2e.core.signals
static void exportCorePluginSignals() {
    // Expose the CorePlugin signals
    EXPORT_CORE_SIGNAL(EngineShutdown, engine_shutdown);
    EXPORT_CORE_SIGNAL(TranslateBlockStart, translate_block_start);
    EXPORT_CORE_SIGNAL(TranslateBlockEnd, translate_block_end);
    EXPORT_CORE_SIGNAL(TranslateBlockComplete, translate_block_complete);
    EXPORT_CORE_SIGNAL(TranslateInstructionStart, translate_instruction_start);
    EXPORT_CORE_SIGNAL(TranslateInstructionEnd, translate_instruction_end);
    EXPORT_CORE_SIGNAL(TranslateSpecialInstructionEnd, translate_special_instruction_end);
    EXPORT_CORE_SIGNAL(TranslateRegisterAccessEnd, translate_register_access_end);
    EXPORT_CORE_SIGNAL(TranslateJumpStart, translate_jump_start);
    EXPORT_CORE_SIGNAL(TranslateICTIStart, translate_icti_start);
    EXPORT_CORE_SIGNAL(TranslateLeaRipRelative, translate_lea_rip_relative);
    EXPORT_CORE_SIGNAL(Exception, exception);
    EXPORT_CORE_SIGNAL(CustomInstruction, custom_instruction);
    EXPORT_CORE_SIGNAL(TranslateSoftInterruptStart, translate_soft_interrupt_start);
    EXPORT_CORE_SIGNAL(BeforeSymbolicDataMemoryAccess, before_symbolic_data_memory_access);
    EXPORT_CORE_SIGNAL(AfterSymbolicDataMemoryAccess, after_symbolic_data_memory_access);
    EXPORT_CORE_SIGNAL(SymbolicAddress, symbolic_address);
    EXPORT_CORE_SIGNAL(ConcreteDataMemoryAccess, concrete_data_memory_access);
    EXPORT_CORE_SIGNAL(PortAccess, port_access);
    EXPORT_CORE_SIGNAL(SymbolicVariableCreation, symbolic_variable_creation);
    EXPORT_CORE_SIGNAL(Timer, timer);
    EXPORT_CORE_SIGNAL(StateFork, state_fork);
    EXPORT_CORE_SIGNAL(StateMerge, state_merge);
    EXPORT_CORE_SIGNAL(StateSwitch, state_switch);
    EXPORT_CORE_SIGNAL(StateKill, state_kill);
    EXPORT_CORE_SIGNAL(ProcessForkDecide, process_fork_decide);
    EXPORT_CORE_SIGNAL(StateForkDecide, state_fork_decide);
    EXPORT_CORE_SIGNAL(ProcessFork, process_fork);
    EXPORT_CORE_SIGNAL(StatesSplit, states_split);
    EXPORT_CORE_SIGNAL(ProcessForkComplete, process_fork_complete);
    EXPORT_CORE_SIGNAL(TlbMiss, tlb_miss);
    EXPORT_CORE_SIGNAL(PageFault, page_fault);
    EXPORT_CORE_SIGNAL(PrivilegeChange, privilege_change);
    EXPORT_CORE_SIGNAL(PageDirectoryChange, page_directory_change);
    EXPORT_CORE_SIGNAL(InitializationComplete, initialization_complete);
    EXPORT_CORE_SIGNAL(UpdateStates, update_states);
    EXPORT_CORE_SIGNAL(AddressSpaceChange, address_space_change);
    EXPORT_CORE_SIGNAL(CallReturnTranslate, call_return_translate);
}

/// Export \c s2e.core.signals.ExecutionSignal
static void exportExecutionSignal() {
    py::class_<PythonExecutionSignal>("ExecutionSignal", py::no_init)
        .def("connect", &PythonExecutionSignal::connect)
        .def("disconnect", &PythonExecutionSignal::disconnect);
}

/// Export \c s2e.core.signals
void exportSignals() {
    try {
        // Make "from s2e.core.signals import *" work
        py::object module(py::handle<>(py::borrowed(PyImport_AddModule("s2e.core.signals"))));

        // Make "from s2e.core import signals" work
        py::scope().attr("signals") = module;

        // Set the current scope to the s2e.core.signals module
        py::scope submoduleScope = module;

        exportCorePluginSignals();
        exportExecutionSignal();
    } catch (py::error_already_set const &) {
        pythonError(parsePythonException());
    }
}

} // namespace python
} // namespace s2e
