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

#include <s2e/CorePlugin.h>
#include <s2e/Python/ConfigFile.h>
#include <s2e/S2E.h>
#include <s2e/S2EExecutionState.h>
#include <s2e/S2EExecutor.h>

#include "Utils.h"
#include "export_core.h"
#include "export_expr.h"
#include "export_signals.h"

///
/// \file export_core.cpp
/// \brief Exports core S2E classes to the \c s2e.core Python module.
///

namespace py = boost::python;

namespace s2e {
namespace python {

/// Export \c s2e.core.S2E
static void exportS2E() {
    py::class_<S2E>("S2E", py::no_init)
        .add_property("output_directory",
                      py::make_function(&S2E::getOutputDirectory, py::return_internal_reference<>()))
        .add_property("output_directory_base",
                      py::make_function(&S2E::getOutputDirectoryBase, py::return_internal_reference<>()))
        .add_property("executor", py::make_function(&S2E::getExecutor, py::return_internal_reference<>()))
        .def("fork", &S2E::fork)
        .def("fetch_and_increment_state_id", &S2E::fetchAndIncrementStateId)
        .def("fetch_next_state_id", &S2E::fetchNextStateId)
        .add_property("max_processes", &S2E::getMaxProcesses)
        .add_property("current_process_id", &S2E::getCurrentProcessId)
        .add_property("current_process_index", &S2E::getCurrentProcessIndex)
        .add_property("current_process_count", &S2E::getCurrentProcessCount)
        .add_property("start_time", &S2E::getStartTime);
}

/// Export \c s2e.core.Config
static void exportConfig() {
    py::class_<PythonConfigFile>("Config", py::no_init)
        .def("__getitem__", &PythonConfigFile::get)
        .def("keys", &PythonConfigFile::keys);
}

///
/// Generate wrappers for the overloaded \c S2EExecutionStateMemory::getHostAddress method. This allows the method to
/// be used without the address type
///
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getHostAddress_overloads, S2EExecutionStateMemory::getHostAddress, 1, 2)

/// Export \c s2e.core.TranslationBlock
static void exportTranslationBlock() {
    py::class_<TranslationBlock>("TranslationBlock", py::no_init)
        .add_property("pc", &TranslationBlock::pc)
        .add_property("cs_base", &TranslationBlock::cs_base)
        .add_property("flags", &TranslationBlock::flags)
        .add_property("size", &TranslationBlock::size)
        .add_property("cflags", &TranslationBlock::cflags)
        .add_property("tc_size", &TranslationBlock::tc_size)
        .add_property("icount", &TranslationBlock::icount)
#ifdef CONFIG_SYMBEX
        .add_property("reg_rmask", &TranslationBlock::reg_rmask)
        .add_property("reg_wmask", &TranslationBlock::reg_wmask)
        .add_property("helper_accesses_mem", &TranslationBlock::helper_accesses_mem)
        .add_property("se_tb_call_eip", &TranslationBlock::se_tb_call_eip)
        .add_property("pc_of_last_instr", &TranslationBlock::pcOfLastInstr)
        .add_property("precise_entries", &TranslationBlock::precise_entries)
        .add_property("instrumented", &TranslationBlock::instrumented)
#ifdef TCG_KEEP_OPC
        .add_property("gen_opc_count", &TranslationBlock::gen_opc_count)
        .add_property("tcg_nb_globals", &TranslationBlock::tcg_nb_globals)
        .add_property("tcg_nb_temps", &TranslationBlock::tcg_nb_temps)
#endif
#ifdef STATIC_TRANSLATOR
        .add_property("last_pc", &TranslationBlock::last_pc)
#endif
#endif
        ;
}

/// Export \cs2e.core.AddressType
static void exportAddressType() {
    py::enum_<AddressType>("AddressType")
        .value("virtual_address", VirtualAddress)
        .value("physical_address", PhysicalAddress)
        .value("host_address", HostAddress);
}

/// Export \c s2e.core.Memory
static void exportMemory() {
    py::class_<S2EExecutionStateMemory>("Memory", py::no_init)
        .def("get_physical_address", &S2EExecutionStateMemory::getPhysicalAddress)
        .def("get_host_address", &S2EExecutionStateMemory::getHostAddress, getHostAddress_overloads())
        .def("read_dirty_mask", &S2EExecutionStateMemory::readDirtyMask)
        .def("write_dirty_mask", &S2EExecutionStateMemory::writeDirtyMask);
}

/// Export \c s2e.core.SpecialInstruction
static void exportSpecialInstruction() {
    py::enum_<special_instruction_t>("SpecialInstruction")
        .value("rdtsc", RDTSC)
        .value("sysenter", SYSENTER)
        .value("syscall", SYSCALL)
        .value("pushim", PUSHIM);
}

/// Export \c s2e.core.Registers
static void exportRegisters() {
    py::class_<S2EExecutionStateRegisters>("Registers", py::no_init)
        .def("copy_symb_regs", &S2EExecutionStateRegisters::copySymbRegs)
        .def("compare_architectural_concrete_state", &S2EExecutionStateRegisters::compareArchitecturalConcreteState)
        .def("save_concrete_state", &S2EExecutionStateRegisters::saveConcreteState)
        .def("restore_concrete_state", &S2EExecutionStateRegisters::restoreConcreteState)
        .def("all_concrete", &S2EExecutionStateRegisters::allConcrete)
        .add_property("symbolic_register_mask", &S2EExecutionStateRegisters::getSymbolicRegistersMask)
        .add_property("flags_are_symbolic", &S2EExecutionStateRegisters::flagsRegistersAreSymbolic)
        .add_property("initialized", &S2EExecutionStateRegisters::initialized)
        .add_property("pc", &S2EExecutionStateRegisters::getPc, &S2EExecutionStateRegisters::setPc)
        .add_property("page_dir", &S2EExecutionStateRegisters::getPageDir)
        .add_property("sp", &S2EExecutionStateRegisters::getSp, &S2EExecutionStateRegisters::setSp)
        .add_property("bp", &S2EExecutionStateRegisters::getBp, &S2EExecutionStateRegisters::setBp)
        .add_property("flags", &S2EExecutionStateRegisters::getFlags);
}

/// Export \c s2e.core.Executor
static void exportExecutor() {
    py::class_<S2EExecutor>("Executor", py::no_init)
        // From klee::Executor
        .add_property("states_count", &S2EExecutor::getStatesCount)
        .add_property("states", py::make_function(&S2EExecutor::getStates, py::return_internal_reference<>()))

        // From s2e::S2Executor
        .add_property("load_balancing", &S2EExecutor::isLoadBalancing)
        .def("reset_state_switch_timer", &S2EExecutor::resetStateSwitchTimer);
}

/// Export \c s2e.core.ExecutionState
static void exportExecutionState() {
    py::class_<S2EExecutionState>("ExecutionState", py::no_init)
        .add_property("id", &S2EExecutionState::getID)
        .def("is_active", &S2EExecutionState::isActive)
        .add_property("zombie", &S2EExecutionState::isZombie, &S2EExecutionState::zombify)
        .add_property("yielded", &S2EExecutionState::isYielded, &S2EExecutionState::yield)
        .add_property("pinned", &S2EExecutionState::isPinned, &S2EExecutionState::setPinned)
        .add_property("state_switch_forbidden", &S2EExecutionState::isStateSwitchForbidden,
                      &S2EExecutionState::setStateSwitchForbidden)
        .add_property("running_concrete", &S2EExecutionState::isRunningConcrete)
        .def("bypass_function", &S2EExecutionState::bypassFunction)
        .def("jump_to_symbolic_cpp", &S2EExecutionState::jumpToSymbolicCpp)
        .def("undo_call_and_jump_to_symbolic", &S2EExecutionState::undoCallAndJumpToSymbolic)
        .add_property("forking_enabled", &S2EExecutionState::isForkingEnabled, &S2EExecutionState::setForking)
        .add_property("symbolic_execution_enabled", &S2EExecutionState::isSymbolicExecutionEnabled)
        .add_property("running_exception_emulation_code", &S2EExecutionState::isRunningExceptionEmulationCode,
                      &S2EExecutionState::setRunningExceptionEmulationCode)
        .def("enable_symbolic_execution", &S2EExecutionState::enableSymbolicExecution)
        .def("disable_symbolic_execution", &S2EExecutionState::disableSymbolicExecution)
        .def("add_constraint", &S2EExecutionState::addConstraint)
        .def("test_constraints", &S2EExecutionState::testConstraints)
        .def("apply_constraints", &S2EExecutionState::applyConstraints)
        .def("create_symbolic_value", &S2EExecutionState::createSymbolicValue)
        .def("create_symbolic_array", &S2EExecutionState::createSymbolicArray)
        .def("create_concolic_array", &S2EExecutionState::createConcolicArray)
        .def("merge", &S2EExecutionState::merge)
        .def("write_pointer", &S2EExecutionState::writePointer)
        .add_property("pointer_size", &S2EExecutionState::getPointerSize);
}

/// Export \c s2e.core
void exportCore() {
    try {
        // Make "from s2e.core import *" work
        py::object module(py::handle<>(py::borrowed(PyImport_AddModule("s2e.core"))));

        // Make "from s2e import core" work
        py::scope().attr("core") = module;

        // Set the current scope to the s2e.core module
        py::scope submoduleScope = module;

        exportS2E();
        exportConfig();
        exportTranslationBlock();
        exportAddressType();
        exportMemory();
        exportSpecialInstruction();
        exportRegisters();
        exportExecutor();
        exportExecutionState();

        exportSignals();
        exportExpr();
    } catch (py::error_already_set const &) {
        pythonError(parsePythonException());
    }
}

} // namespace python
} // namespace s2e
