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
/// \file Interpreter.h
/// \brief The core Python interpreter interface.
///

#ifndef PY_INTERPRETER_H_
#define PY_INTERPRETER_H_

#include <map>

#include <boost/python/object.hpp>

namespace py = boost::python;

namespace s2e {

class ConfigFile;
class Plugin;

namespace python {

///
/// S2E's Python interpreter.
///
/// This class initializes the Python interpreter, loads and maintains Python modules/plugins.
///
class Interpreter {
private:
    using PluginMap = std::map<std::string, py::object>;

    /// A map of plugin names to their Python objects
    PluginMap m_plugins;

public:
    ///
    /// \brief Construct a new Python interpreter based on the given configuration file.
    ///
    /// The constructor will read the following fields in the config file:
    ///
    /// - \c s2e.python.moduleDirs : A list of directories that contain Python modules. These directories will be
    /// appended to the Python interpreter's search path (i.e. \c sys.path) so that it can load Python plugins from
    /// arbitrary locations.
    ///
    Interpreter(const ConfigFile *);

    ///
    /// \brief Load and instantiate a Python plugin in the interpreter.
    ///
    /// \param pyClass The name of a Python class that extends the base S2E \c Plugin class. This class name should
    /// be given in the standard Python module format - i.e. \c module1.module2.ClassName. This module path should
    /// be relative to one of the directories listed in the \c s2e.python.moduleDirs configuration file entry.
    /// \return The instantiated plugin.
    ///
    Plugin *loadPlugin(const std::string &pyClass);

    ///
    /// \brief Call a Python plugin's \c initialize method in the interpreter.
    ///
    /// \param name The name of the plugin to initialize.
    ///
    void initializePlugin(const std::string &name);

    ///
    /// \brief Retrieve a previously-loaded and initialized plugin from the Python interpreter.
    ///
    /// \param name The plugin's name.
    /// \return A Python object containing the (instantiated and initialized) plugin. Returns \c None if the plugin
    /// does not exist.
    ///
    py::object getPlugin(const std::string &name);

    ///
    /// \brief Retrieve the state for a given plugin.
    ///
    /// In Python plugins, state is stored in a private attribute named \c _state.
    ///
    /// \param name The plugin's name
    /// \return A Python object containing the plugin's state
    ///
    py::object getPluginState(const std::string &name);
};

} // namespace python
} // namespace s2e

#endif
