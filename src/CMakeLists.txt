# Copyright (C) 2017, Cyberhaven
# All rights reserved.
#
# Licensed under the Cyberhaven Research License Agreement.

set(S2ECORE_SOURCES S2E.cpp
                    S2EExecutionState.cpp
                    S2EExecutionStateRegisters.cpp
                    S2EExecutor.cpp
                    S2EExecutionStateMemory.cpp
                    S2EExecutionStateTlb.cpp
                    S2EDeviceState.cpp
                    AddressSpaceCache.cpp
                    MMUFunctionHandlers.cpp

                    PluginManager.cpp
                    Plugin.cpp
                    CorePluginInterface.cpp
                    CorePlugin.cpp
                    ConfigFile.cpp

                    s2e-qmp.cpp
                    SymbolicHardwareHook.cpp
                    ExprInterface.cpp
                    S2EStatsTracker.cpp
                    Synchronization.cpp
                    Utils.cpp)

if(PYTHON_PLUGINS)
    set(S2ECORE_SOURCES ${S2ECORE_SOURCES}
                        Python/Callbacks.cpp
                        Python/ConfigFile.cpp
                        Python/Interpreter.cpp
                        Python/Plugin.cpp
                        Python/Utils.cpp
                        Python/export_core.cpp
                        Python/export_expr.cpp
                        Python/export_plugins.cpp
                        Python/export_signals.cpp)
endif(PYTHON_PLUGINS)

add_library(s2ecore ${S2ECORE_SOURCES})

target_include_directories(s2ecore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(WERROR_FLAGS "-Wall -Werror -Wno-zero-length-array -Wno-c99-extensions    \
                  -Wno-gnu-anonymous-struct -Wno-nested-anon-types            \
                  -Wno-extended-offsetof -Wno-gnu-statement-expression        \
                  -Wno-gnu-zero-variadic-macro-arguments -Wno-vla-extension   \
                  -Wno-covered-switch-default -Wno-shift-negative-value       \
                  -Wno-deprecated-register -Wno-sign-compare                  \
                  -Wno-missing-field-initializers -Wno-mismatched-tags        \
                  -Wno-deprecated-declarations -Wno-initializer-overrides     \
                  -Wno-zero-length-array")

set(COMMON_FLAGS "-D__STDC_FORMAT_MACROS -D_GNU_SOURCE -DNEED_CPU_H           \
                  -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE                  \
                  -DTARGET_PHYS_ADDR_BITS=64 -fPIC -fno-strict-aliasing       \
                  -fexceptions -std=c++11")

if(PYTHON_PLUGINS)
    set(WERROR_FLAGS "${WERROR_FLAGS} -Wno-unused-local-typedefs")
    set(COMMON_FLAGS "${COMMON_FLAGS} -DS2E_PYTHON_PLUGINS")
endif(PYTHON_PLUGINS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WERROR_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WERROR_FLAGS} ${COMMON_FLAGS}")
