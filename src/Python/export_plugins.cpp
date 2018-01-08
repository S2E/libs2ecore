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

#include <s2e/Python/Plugin.h>
#include <s2e/S2E.h>
#include <s2e/S2EExecutionState.h>

#include "Utils.h"
#include "export_plugins.h"

///
/// \file export_plugins.cpp
/// \brief Exports plugin-related classes to the \c s2e.core.plugins Python module.
///

namespace py = boost::python;

namespace s2e {
namespace python {

///
/// Generate wrappers for the overloaded \c PythonPlugin::debug method. This allows the method to be used without the
/// S2E execution state
///
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(debug_overloads, PythonPlugin::debug, 1, 2)

///
/// Generate wrappers for the overloaded \c PythonPlugin::info method. This allows the method to be used without the
/// S2E execution state
///
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(info_overloads, PythonPlugin::info, 1, 2)

///
/// Generate wrappers for the overloaded \c PythonPlugin::warn method. This allows the method to be used without the
/// S2E execution state
///
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(warn_overloads, PythonPlugin::warn, 1, 2)

/// Export \c s2e.plugins.PluginInfo
static void exportPluginInfo() {
    py::class_<PluginInfo>("PluginInfo", py::no_init)
        .def_readonly("name", &PluginInfo::name)
        .def_readonly("description", &PluginInfo::description)
        .def_readonly("config_key", &PluginInfo::configKey)
        // TODO add dependencies vector as read only member
        ;
}

/// Export \c s2e.plugins.Plugin
static void exportPlugin() {
    py::class_<PythonPlugin>("Plugin")
        .add_property("plugin_info", py::make_function(&PythonPlugin::getPluginInfo, py::return_internal_reference<>()))
        .add_property("config", py::make_function(&PythonPlugin::getConfig, py::return_internal_reference<>()))
        .def("initialize", &PythonPlugin::initialize)
        .def("debug", &PythonPlugin::debug, debug_overloads())
        .def("info", &PythonPlugin::info, info_overloads())
        .def("warn", &PythonPlugin::warn, warn_overloads())
        .add_property("s2e", py::make_function(static_cast<const S2E *(PythonPlugin::*) () const>(&PythonPlugin::s2e),
                                               py::return_internal_reference<>()));
}

/// Export \c s2e.plugins
void exportPlugins() {
    try {
        // Make "from s2e.core.plugins import *" work
        py::object module(py::handle<>(py::borrowed(PyImport_AddModule("s2e.core.plugins"))));

        // Make "from s2e.core import plugins" work
        py::scope().attr("plugins") = module;

        // Set the current scope to the s2e.core.plugins module
        py::scope submoduleScope = module;

        exportPluginInfo();
        exportPlugin();
    } catch (py::error_already_set const &) {
        pythonError(parsePythonException());
    }
}

} // namespace python
} // namespace s2e
