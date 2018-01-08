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
/// \file Utils.h
/// \brief A collection of utility functions and macros for working with S2E's Python interpreter.
///

#ifndef PY_UTILS_H_
#define PY_UTILS_H_

#include <vector>

#include <boost/python/iterator.hpp>
#include <boost/python/list.hpp>
#include <boost/python/stl_iterator.hpp>

namespace py = boost::python;

namespace s2e {
namespace python {

///
/// \brief Convert an iterable Python object (e.g. a list) to an STL \c vector.
///
/// For this conversion to be successful, the Python object must be (a) iterable and (b) a homogeneous collection. None
/// of these things are checked.
///
/// \param o An iterable, homogeneous Python object
/// \return An STL vector
///
template <typename T> static inline std::vector<T> toVector(const py::object &o) {
    return std::vector<T>(py::stl_input_iterator<T>(o), py::stl_input_iterator<T>());
}

///
/// \brief Convert an STL \c vector to a Python list.
///
template <typename T> static inline py::list toList(const std::vector<T> &v) {
    py::object iter = py::iterator<std::vector<T>>()(v);

    return py::list(iter);
}

///
/// Parse a Python exception (that occurred within the interpreter).
///
/// \return A \c string describing the Python exception
///
std::string parsePythonException();

///
/// Write a message to the warning stream and exit with return code -1.
///
/// \param msg The message to write.
///
void pythonError(std::string msg);

} // namespace python
} // namespace s2e

#endif
