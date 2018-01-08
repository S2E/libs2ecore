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
/// \file ExecutionSignal.h
/// \brief Wrap the CorePlugin's \c ExecutionSignal to make it usable within the Python interpreter.
///

#ifndef PY_EXECUTION_SIGNAL_H_
#define PY_EXECUTION_SIGNAL_H_

#include <boost/python/object.hpp>

#include <s2e/CorePlugin.h>

#include "Callback.h"
#include "Signal.h"

namespace py = boost::python;

namespace s2e {
namespace python {

class OnExecutionCallback : public CallbackFunctor<S2EExecutionState *, uint64_t> {
public:
    virtual void operator()(S2EExecutionState *state, uint64_t pc) {
        m_callback(py::ptr(state), pc);
    }
};

///
/// \brief Python wrapper around an \c ExecutionSignal.
///
/// In contrast to the other signal wrappers (defined in Signals.h), which are simply typedefs of the \c PythonSignal
/// class, the \c ExecutionSignal wrapper instead subclasses \c PythonSignal. This is because (as discussed in
/// CorePlugin.h) \c ExecutionSignal instances are dynamically created and destroyed on demand during translation, so
/// S2E requires the ability to update the wrapped signal during runtime.
///
class PythonExecutionSignal : public PythonSignal<OnExecutionCallback, S2EExecutionState *, uint64_t> {
public:
    PythonExecutionSignal() : PythonSignal(nullptr) {
    }

    /// Update the wrapped execution signal
    void saveSignal(ExecutionSignal *sig) {
        m_signal = sig;
    }
};

///
/// \brief Base functor class for executing Python callbacks that pass an \c ExecutionSignal as an argument.
///
/// This is just a convenience class to inherit from.
///
template <typename... Ts> class CallbackFunctorWithExecutionSignal : public CallbackFunctor<ExecutionSignal *, Ts...> {
protected:
    PythonExecutionSignal m_executionSignal;
};

} // namespace python
} // namespace s2e

#endif
