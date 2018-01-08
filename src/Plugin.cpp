///
/// Copyright (C) 2010-2015, Dependable Systems Laboratory, EPFL
/// Copyright (C) 2016, Cyberhaven
/// All rights reserved.
///
/// Licensed under the Cyberhaven Research License Agreement.
///

#include <s2e/ConfigFile.h>
#include <s2e/Plugin.h>
#include <s2e/S2E.h>
#include <s2e/S2EExecutionState.h>
#include <s2e/Utils.h>

#ifdef S2E_PYTHON_PLUGINS
#include <s2e/Python/Interpreter.h>
#endif

#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/raw_ostream.h>

#include <algorithm>
#include <assert.h>

#include <s2e/Logging.h>

namespace s2e {

using namespace std;

CompiledPlugin::CompiledPlugins *CompiledPlugin::s_compiledPlugins = NULL;

void Plugin::initialize() {
}

///
/// Set a default log level, based on global configuration,
/// then override if needed with per-plugin level.
///
void Plugin::configureLogLevel() {
    m_nullOutput = &llvm::nulls();

    if (s2e()->hasGlobalLogLevel()) {
        m_logLevel = s2e()->getGlobalLogLevel();
    } else {
        parseLogLevel(DEFAULT_PLUGIN_LOG_LEVEL, &m_logLevel);
    }

    bool ok;
    ConfigFile *cfg = s2e()->getConfig();
    std::string logLevel = cfg->getString(getConfigKey() + ".logLevel", DEFAULT_PLUGIN_LOG_LEVEL, &ok);

    if (ok) {
        parseLogLevel(logLevel, &m_logLevel);
    }
}

PluginState *Plugin::getPluginState(S2EExecutionState *s, PluginStateFactory f) const {
    if (m_CachedPluginS2EState == s) {
        return m_CachedPluginState;
    }
    m_CachedPluginState = s->getPluginState(const_cast<Plugin *>(this), f);
    m_CachedPluginS2EState = s;
    return m_CachedPluginState;
}

llvm::raw_ostream &Plugin::getDebugStream(S2EExecutionState *state) const {
    if (m_logLevel <= LOG_DEBUG) {
        return s2e()->getDebugStream(state) << getPluginInfo()->name << ": ";
    } else {
        return *m_nullOutput;
    }
}

llvm::raw_ostream &Plugin::getInfoStream(S2EExecutionState *state) const {
    if (m_logLevel <= LOG_INFO) {
        return s2e()->getInfoStream(state) << getPluginInfo()->name << ": ";
    } else {
        return *m_nullOutput;
    }
}

llvm::raw_ostream &Plugin::getWarningsStream(S2EExecutionState *state) const {
    if (m_logLevel <= LOG_WARN) {
        return s2e()->getWarningsStream(state) << getPluginInfo()->name << ": ";
    } else {
        return *m_nullOutput;
    }
}

PluginsFactory::PluginsFactory() {
    // Register compiled (C++) plugins. We don't register Python plugins until
    // load time
    CompiledPlugin::CompiledPlugins *plugins = CompiledPlugin::getPlugins();

    foreach2 (it, plugins->begin(), plugins->end()) { registerPlugin(*it); }
}

void PluginsFactory::registerPlugin(const PluginInfo *pluginInfo) {
    assert(m_pluginsMap.find(pluginInfo->name) == m_pluginsMap.end());

    m_pluginsList.push_back(pluginInfo);
    m_pluginsMap.insert(make_pair(pluginInfo->name, pluginInfo));
}

const vector<const PluginInfo *> &PluginsFactory::getPluginInfoList() const {
    return m_pluginsList;
}

const PluginInfo *PluginsFactory::getPluginInfo(const string &name) const {
    PluginsMap::const_iterator it = m_pluginsMap.find(name);

    if (it != m_pluginsMap.end())
        return it->second;
    else
        return NULL;
}

Plugin *PluginsFactory::createPlugin(S2E *s2e, const string &name) {
    const PluginInfo *pluginInfo = getPluginInfo(name);
    s2e->getInfoStream() << "Creating plugin " << name << "\n";

    if (pluginInfo) {
        // If the PluginInfo object already exists, then it must be a compiled C++ plugin
        return pluginInfo->instanceCreator(s2e);
    } else {
#ifdef S2E_PYTHON_PLUGINS
        // If the PluginInfo object does *not* exist, it could be a Python plugin. So first check if it has the
        // PythonClass config field
        bool ok;
        string pyClass = s2e->getConfig()->getString("pluginsConfig['" + name + "'].pythonClass", "", &ok);
        if (!ok) {
            // Not a Python plugin. Display an error message and bail
            s2e->getWarningsStream() << "Plugin '" << name << "' does not exist in this S2E installation, nor is it a "
                                     << "Python plugin (it has no `pythonClass` config field)\n";
            return nullptr;
        }

        // Load and instantiate the plugin in the Python interpreter and register its details with the plugin factory
        Plugin *plugin = s2e->getPythonInterpreter()->loadPlugin(pyClass);
        registerPlugin(plugin->getPluginInfo());

        return plugin;
#else
        return nullptr;
#endif
    }
}

} // namespace s2e
