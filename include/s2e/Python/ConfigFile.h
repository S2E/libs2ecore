// TODO add copyright

///
/// \file ConfigFile.h
/// \brief Wrap the \c ConfigFile class to make it usable within the Python interpreter.
///

#ifndef PY_CONFIG_H_
#define PY_CONFIG_H_

#include <boost/python/list.hpp>
#include <boost/python/object.hpp>

#include <s2e/ConfigFile.h>

namespace py = boost::python;

namespace s2e {

struct PluginInfo;

namespace python {

///
/// \brief A Python wrapper around a \c ConfigFile.
///
/// The wrapped config file is read-only. Each wrapped config file is specific to a particular plugin. This is done so
/// that access to table values is more "Pythonic".
///
class PythonConfigFile {
private:
    /// The wrapped config file
    ConfigFile *const m_cfg;

    ///
    /// \brief The prefix for accessing a plugin-specific config value.
    ///
    /// This will be of the form:
    ///
    /// \code
    ///   pluginsConfig['PLUGIN_CONFIG_KEY'].
    /// \endcode
    ///
    /// where \c PLUGIN_CONFIG_KEY is the plugin's config key.
    ///
    const std::string m_keyPrefix;

public:
    /// Wrap a \c ConfigFile for a specific plugin.
    PythonConfigFile(ConfigFile *const cfg, const PluginInfo *pi);

    ///
    /// \brief Return a value from the config file.
    ///
    /// Returns \c None if the given key does not exist.
    ///
    py::object get(const std::string &name);

    ///
    /// \brief Return a list of string keys for a given table.
    ///
    /// Returns \c None if \c name is invalid.
    ///
    py::object keys(const std::string &name) const;
};

} // namespace python
} // namespace s2e

#endif
