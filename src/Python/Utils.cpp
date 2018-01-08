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

#include <s2e/S2E.h>

#include "Utils.h"

namespace py = boost::python;

namespace s2e {
namespace python {

// Adapted from https://github.com/josephturnerjr/boost-python-tutorial/blob/master/part4/handle_error.cpp
std::string parsePythonException() {
    PyObject *typePtr = NULL;
    PyObject *valuePtr = NULL;
    PyObject *tracebackPtr = NULL;

    // Fetch the exception info from the Python C API
    PyErr_Fetch(&typePtr, &valuePtr, &tracebackPtr);

    // Fallback error string
    std::string err = "Unfetchable Python error";

    // If PyErr_Fetch got a type pointer, parse the type into the exception
    // string
    if (typePtr) {
        py::handle<> typeHandle(typePtr);
        py::str typePyStr(typeHandle);
        py::extract<std::string> typeStr(typePyStr);

        // If a valid string extraction is evailable, use it. Otherwise use
        // a fallback message
        if (typeStr.check()) {
            err = typeStr();
        } else {
            err = "Unknown exception type";
        }

        // Do the same for the exception value
        if (valuePtr) {
            py::handle<> valHandle(valuePtr);
            py::str valPyStr(valHandle);
            py::extract<std::string> valStr(valPyStr);

            if (valStr.check()) {
                err += ": " + valStr();
            } else {
                err += ": Unparseable Python error: ";
            }
        }
    }

    return err;
}

void pythonError(std::string msg) {
    g_s2e->getWarningsStream() << "Python error: " << msg << '\n';
    ::exit(-1);
}

} // namespace python
} // namespace s2e
