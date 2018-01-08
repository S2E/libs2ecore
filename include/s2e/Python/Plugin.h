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
/// \file Plugin.h
/// \brief Base classes for Python plugins and their state.
///

#ifndef PY_PLUGIN_H_
#define PY_PLUGIN_H_

#include <boost/python/dict.hpp>
#include <boost/python/object.hpp>

#include <s2e/CorePlugin.h>
#include <s2e/Plugin.h>
#include <s2e/S2E.h>

#include <s2e/Python/ConfigFile.h>

namespace py = boost::python;

namespace s2e {
namespace python {

///
/// \brief The base Python plugin class that all S2E Python plugins must extend.
///
/// If this base Python plugin class was written in Python, it would look like the following:
///
/// \code{.py}
/// class Plugin(object):
///   # A class member that describes the plugin
///   description = 'A description of the plugin'
///
///   # A class member that lists the names of this plugin's dependencies
///   dependencies = ['PluginA', 'PluginB']
///
///   def __init__(self, plugin_info):
///     """
///     The base plugin constructor initializes the log level, etc.
///     """
///     pass
///
///   def initialize(self):
///     """
///     Equivalent to a C++ plugin's initialize method.
///     """
///     pass
///
///   @property
///   def config(self):
///     """
///     Get the S2E config.
///     """
///     pass
///
///   def debug(self, msg, state=None):
///     """
///     Writes `msg` to S2E's debug stream. Optionally prepend the message with S2E execution state details.
///     """
///     pass
///
///   def info(self, msg, state=None):
///     """
///     Writes `msg` to S2E's info stream. Optionally prepend the message with S2E execution state details.
///     """
///     pass
///
///   def warn(self, msg, state=None):
///     """
///     Write's `msg` to S2E's warning stream. Optionally prepend the message with S2E execution state details.
///     """
///     pass
///
///   def get_plugin(self, name):
///     """
///     Get a Python plugin named `name`.
///     """
///     pass
/// \endcode
///
class PythonPlugin : public Plugin {
    // The S2E_PLUGIN macro is not used because everything about a Python plugin must be determined at runtime.
    // Dependencies are handled by the Python interpreter (see s2e::python::Interpreter::loadPlugin)
public:
    virtual const PluginInfo *getPluginInfo() const {
        return m_pluginInfo;
    }

    virtual const PluginInfo *getPluginInfoStatic() {
        // There is no static plugin info for Python plugins
        return nullptr;
    }
    // End equivalent S2E_PLUGIN macro code

private:
    ///
    /// \brief A description of a specific Python plugin.
    ///
    /// Unlike C++ S2E plugins, Python plugins are instantiated at runtime and hence their \c PluginInfo must be
    /// determined dynamically. This is why the \c PluginInfo is not a static object.
    ///
    PluginInfo *m_pluginInfo;

    /// Wrapped S2E config file.
    PythonConfigFile *m_cfg;

public:
    PythonPlugin();

    virtual ~PythonPlugin();

    ///
    /// \brief Configure the Python plugin.
    ///
    /// This will initialize the \c PluginInfo and the \c PythonConfigFile wrapper.
    ///
    void configure(const PluginInfo &);

    ///
    /// Get the S2E config.
    ///
    /// \returns A wrapped version of the S2E config suitable for use in the Python interpreter.
    ///
    PythonConfigFile *getConfig() const;

    ///
    /// Write a message to the debug stream.
    ///
    /// \param msg The message to write.
    /// \param state (Optional) S2E state.
    ///
    void debug(const std::string &msg, const S2EExecutionState *state = nullptr) const;

    ///
    /// Write a message to the info stream.
    ///
    /// \param msg The message to write.
    /// \param state (Optional) S2E state.
    ///
    void info(const std::string &msg, const S2EExecutionState *state = nullptr) const;

    ///
    /// Write a message to the warning stream.
    ///
    /// \param msg The message to write.
    /// \param state (Optional) S2E state.
    ///
    void warn(const std::string &msg, const S2EExecutionState *state = nullptr) const;
};

} // namespace python
} // namespace s2e

#endif
