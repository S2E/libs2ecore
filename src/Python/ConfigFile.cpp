// TODO add copyright

#include <s2e/Plugin.h>
#include <s2e/Python/ConfigFile.h>

#include "Utils.h"

namespace s2e {
namespace python {

PythonConfigFile::PythonConfigFile(ConfigFile *const cfg, const PluginInfo *pi)
    : m_cfg(cfg), m_keyPrefix("pluginsConfig['" + pi->configKey + "'].") {
}

py::object PythonConfigFile::get(const std::string &name) {
    // Be silent
    bool wasSilent = m_cfg->isSilent();
    m_cfg->setSilent(true);

    bool ok;

    // XXX is string concat the most efficient way to do this?
    std::string key = m_keyPrefix + name;

    // Because Python is dynamically typed, this method can return any value. This method will simply try all the
    // different types until a valid return value is created. If a valid return value cannot be found, return None

    // Try to get a bool
    bool boolRes = m_cfg->getBool(key, false, &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return py::object(boolRes);
    }

    // Try to get an int
    int64_t intRes = m_cfg->getInt(key, 0, &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return py::object(intRes);
    }

    // Try to get a double
    double doubleRes = m_cfg->getDouble(key, 0, &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return py::object(doubleRes);
    }

    // Try to get a string
    std::string stringRes = m_cfg->getString(key, "", &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);

        return py::object(stringRes);
    }

    // Try to get a string list
    ConfigFile::string_list stringListRes = m_cfg->getStringList(key, ConfigFile::string_list(), &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return toList(stringListRes);
    }

    // Try to get an integer list
    ConfigFile::integer_list intListRes = m_cfg->getIntegerList(key, ConfigFile::integer_list(), &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return toList(intListRes);
    }

    m_cfg->setSilent(wasSilent);

    // Return None
    return py::object();
}

py::object PythonConfigFile::keys(const std::string &name) const {
    bool ok;

    ConfigFile::string_list keyList = m_cfg->getListKeys(name, &ok);
    if (ok) {
        return toList(keyList);
    }

    // Return None
    return py::object();
}

} // namespace python
} // namespace s2e
