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
/// \file Callback.h
/// \brief Defines the base callback wrapper class.
///
/// A callback class wraps a Python \c function object and provides a function call operator that can be connected to
/// an S2E/sigc signal. This allows us to bridge the gap between signals being emitted in C++ code and writing
/// callbacks in Python code.
///

#ifndef PY_CALLBACK_H_
#define PY_CALLBACK_H_

#include <boost/python/object.hpp>

namespace py = boost::python;

namespace s2e {
namespace python {

///
/// \brief Base functor class for executing function callbacks in the Python interpreter.
///
/// \c CallbackFunctor wraps Python \c function objects rather than calling them directly because callback parameters
/// may need to be manipulated before they are passed to the Python callback function, e.g. transform it into a Python
/// object, handle pointers, etc. This parameter manipulation is performed in the \c CallbackFunctor's function call
/// operator before calling the Python \c function object.
///
/// \tparam Ts Types that the callback functor accepts when a signal is emitted by S2E
///
template <typename... Ts> class CallbackFunctor {
public:
    virtual ~CallbackFunctor() {
    }

    /// Store a Python callback function
    void assignCallback(const py::object &func) {
        m_callback = func;
    }

    /// Invokes the Python callback function with the appropriate arguments
    virtual void operator()(Ts... args) = 0;

protected:
    /// Stores the Python callback function
    py::object m_callback;
};

} // namespace python
} // namespace s2e

#endif
