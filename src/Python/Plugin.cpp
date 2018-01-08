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

#include <s2e/Python/Interpreter.h>
#include <s2e/Python/Plugin.h>
#include <s2e/S2EExecutionState.h>

#include "Utils.h"

namespace py = boost::python;

namespace s2e {
namespace python {

/******************************************************************************
 * PythonPluginState
 *****************************************************************************/

///
/// \brief a plugin's state within the Python interpreter.
///
/// In a Python plugin, a plugin's state can be an object of any type (which is stored in the plugin's \c _state
/// attribute). However, the Python object must still be wrapped to make sure S2E can correctly copy the plugin state
/// when forking occurs. This is done using the \c deepcopy routine, as documented at
/// https://docs.python.org/2/library/copy.html.
///
struct PythonPluginState : public PluginState {
    /// The Python object that encapsulates a plugin's state
    py::object m_stateObj;

    /// The Python \c deepcopy function
    static py::object s_deepCopy;

    PythonPluginState(const py::object &obj) : m_stateObj(obj) {
    }

    virtual ~PythonPluginState() {
    }

    virtual PythonPluginState *clone() const {
        try {
            if (s_deepCopy.is_none()) {
                s_deepCopy = py::import("copy").attr("deepcopy");
            }
            // Perform a deep copy of the plugin's state object.
            py::object copiedStateObj = s_deepCopy(m_stateObj);

            return new PythonPluginState(copiedStateObj);
        } catch (py::error_already_set const &) {
            pythonError(parsePythonException());
        }

        // Control should never reach here
        return nullptr;
    }

    static PluginState *factory(Plugin *p, S2EExecutionState *s) {
        py::object stateObj = g_s2e->getPythonInterpreter()->getPluginState(p->getPluginInfo()->name);

        return new PythonPluginState(stateObj);
    }
};

// The deep copy function object must be initialized to None, because static members will be initialized before the
// Python interpreter is ready (causing an error). The deepcopy function will be lazily assigned
static py::object s_deepCopy = py::object();

/******************************************************************************
 * PythonPlugin
 *****************************************************************************/

PythonPlugin::PythonPlugin() : Plugin(g_s2e) {
}

PythonPlugin::~PythonPlugin() {
    delete m_pluginInfo;
    delete m_cfg;
}

void PythonPlugin::configure(const PluginInfo &pi) {
    m_pluginInfo = new PluginInfo(pi);
    m_cfg = new PythonConfigFile(s2e()->getConfig(), &pi);
}

PythonConfigFile *PythonPlugin::getConfig() const {
    return m_cfg;
}

void PythonPlugin::debug(const std::string &msg, const S2EExecutionState *state) const {
    if (m_pluginInfo && getLogLevel() <= LOG_DEBUG) {
        s2e()->getDebugStream(state) << m_pluginInfo->name << ": " << msg << '\n';
    }
}

void PythonPlugin::info(const std::string &msg, const S2EExecutionState *state) const {
    if (m_pluginInfo && getLogLevel() <= LOG_INFO) {
        s2e()->getInfoStream(state) << m_pluginInfo->name << ": " << msg << '\n';
    }
}

void PythonPlugin::warn(const std::string &msg, const S2EExecutionState *state) const {
    if (m_pluginInfo && getLogLevel() <= LOG_WARN) {
        s2e()->getWarningsStream(state) << m_pluginInfo->name << ": " << msg << '\n';
    }
}

} // namespace python
} // namespace s2e
