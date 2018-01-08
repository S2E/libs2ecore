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

#include <s2e/ConfigFile.h>
#include <s2e/Python/Interpreter.h>
#include <s2e/Python/Plugin.h>
#include <s2e/S2E.h>
#include <s2e/Utils.h>

#include "Utils.h"
#include "export_core.h"
#include "export_plugins.h"

namespace py = boost::python;

namespace s2e {
namespace python {

// Define the S2E Python module
static BOOST_PYTHON_MODULE(s2e) {
    // Specify that this module is actually a package
    py::object package = py::scope();
    package.attr("__path__") = "s2e";

    // Export s2e's submodules
    exportCore();    // s2e.core
    exportPlugins(); // s2e.plugins
}

/******************************************************************************
 * Python interpreter
 *****************************************************************************/

Interpreter::Interpreter(const ConfigFile *cfg) {
    g_s2e->getDebugStream() << "Initializing the Python interpreter\n";

    try {
        // Ensure that the s2e namespace is available in the interpreter
        PyImport_AppendInittab("s2e", &inits2e);

        // Initialize the interpreter
        Py_Initialize();

        // Append the Python module directories to the interpreter's search path
        py::object sysModule = py::import("sys");
        ConfigFile::string_list modulePaths = cfg->getStringList("s2e.python.modulePaths", ConfigFile::string_list());
        for (auto const &modulePath : modulePaths) {
            g_s2e->getDebugStream() << "  - Adding " << modulePath << " to the Python path\n";
            sysModule.attr("path").attr("append")(modulePath);
        }
    } catch (py::error_already_set const &) {
        pythonError(parsePythonException());
    }
}

Plugin *Interpreter::loadPlugin(const std::string &pyClass) {
    try {
        std::string description = "";
        std::vector<std::string> dependencies = std::vector<std::string>();

        // The Python class should be specified in the standard Python module format - i.e. module1.module2.Classname
        //
        // This means that the module list and class name must be separated so that the module can be imported first,
        // followed by the class object from the module
        size_t classSepPos = pyClass.find_last_of(".");
        std::string moduleName = pyClass.substr(0, classSepPos);
        std::string className = pyClass.substr(classSepPos + 1);

        // Import the plugin class so that it is available in the interpreter
        py::object pluginClass = py::import(moduleName.c_str()).attr(className.c_str());

        if (PyObject_HasAttrString(pluginClass.ptr(), "description")) {
            description = py::extract<std::string>(pluginClass.attr("description"));
        }
        if (PyObject_HasAttrString(pluginClass.ptr(), "dependencies")) {
            dependencies = toVector<std::string>(pluginClass.attr("dependencies"));
        }

        // Instantiate the plugin
        py::object pluginObj = pluginClass();

        // Save the instantiated plugin so that it can be used later
        m_plugins[className] = pluginObj;

        // Extract the C++ plugin object and configure it so that it is usable within S2E
        PythonPlugin *plugin = py::extract<PythonPlugin *>(pluginObj);

        PluginInfo pluginInfo = PluginInfo(className, description, dependencies);
        plugin->configure(pluginInfo);

        return plugin;
    } catch (py::error_already_set const &) {
        pythonError(parsePythonException());
    }

    return nullptr;
}

void Interpreter::initializePlugin(const std::string &name) {
    try {
        py::object pluginObj = m_plugins.at(name);

        if (PyObject_HasAttrString(pluginObj.ptr(), "initialize")) {
            pluginObj.attr("initialize")();
        }
    } catch (py::error_already_set const &) {
        pythonError(parsePythonException());
    }
}

py::object Interpreter::getPlugin(const std::string &name) {
    PluginMap::iterator i = m_plugins.find(name);

    if (i == m_plugins.end()) {
        // None
        return py::object();
    } else {
        // The plugin object
        return i->second;
    }
}

py::object Interpreter::getPluginState(const std::string &name) {
    py::object pluginObj = getPlugin(name);

    try {
        if (PyObject_HasAttrString(pluginObj.ptr(), "_state")) {
            return pluginObj.attr("_state");
        }
    } catch (py::error_already_set const &) {
        pythonError(parsePythonException());
    }

    // None
    return py::object();
}

} // namespace python
} // namespace s2e
