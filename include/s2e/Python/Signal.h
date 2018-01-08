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
/// \file Signal.h
/// \brief Wrap an S2E/sigc signal to make it usable within the Python interpreter.
///

#ifndef PY_SIGNAL_H_
#define PY_SIGNAL_H_

#include <boost/python/object.hpp>

#include <fsigc++/fsigc++.h>

namespace py = boost::python;

namespace s2e {
namespace python {

///
/// \brief A Python wrapper around an S2E signal.
///
/// This wrapper allows Python functions to be used as callbacks when a signal is emitted. The callback function is
/// stored within the class.
///
/// \tparam C Callback class
/// \tparam As Types that the signal emits
///
template <typename C, typename... As>
class PythonSignal {
protected:
    /// The wrapped signal
    sigc::signal<void, As...> *m_signal;

    /// Wrapped signal connection. Used for safe disconnection
    sigc::connection m_connection;

    /// The callback functor called when the signal is emitted
    C m_callback;

public:
    ///
    /// Wrap a sigc \c signal.
    ///
    /// \param sig The \c signal to wrap
    ///
    PythonSignal(sigc::signal<void, As...> *sig) : m_signal(sig) {
    }

    ///
    /// \brief Connect a Python function to the wrapped signal.
    ///
    /// The signal's connection is saved so that we can safely disconnect the signal from the callback if required.
    ///
    /// \param func A Python function.
    ///
    void connect(const py::object &func) {
        m_callback.assignCallback(func);
        m_connection = m_signal->connect(sigc::mem_fun(m_callback, &C::operator()));
    }

    ///
    /// Safely disconnect a Python callback from a wrapped signal.
    ///
    void disconnect() {
        m_connection.disconnect();
    }
};

} // namespace python
} // namespace s2e

#endif
