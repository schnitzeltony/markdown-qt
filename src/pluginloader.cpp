#include "markdown-qt.h"
#include "markdown-qt-config.h"
#include <QDir>
#include <QPluginLoader>

constexpr auto ABSOLUTE_PATH_TO_PLUGIN = FULL_PLUGIN_PATH;

PluginLoaderMdQt::PluginLoaderMdQt()
{
    // Check available plugins and fill m_pluginInfoMap
    QDir dir(ABSOLUTE_PATH_TO_PLUGIN);
    QStringList availablePluginLibs = dir.entryList(QStringList(QStringLiteral("*.so")));
    if(availablePluginLibs.count() > 0) {
        for(auto strLibName : availablePluginLibs) {
            QString strFullLibPath = dir.absoluteFilePath(strLibName);
            PluginInfo pluginInfo;
            pluginInfo.strLibName = strLibName;
            QPluginLoader loader(strFullLibPath);
            if (auto instance = loader.instance()) {
                if (auto plugin = qobject_cast<PluginInterfaceMdQt* >(instance)){
                    QString strDisplayName = plugin->displayName();
                    pluginInfo.conversionTypes = plugin->availableConversions();
                    pluginInfo.strDisplayName = strDisplayName;
                    if(!m_pluginInfoMap.contains(strDisplayName)) {
                        m_pluginInfoMap[strDisplayName] = pluginInfo;
                    }
                    else {
                        PluginInfo pluginInfoFirst = m_pluginInfoMap[strDisplayName];
                        qWarning("Plugins %s and %s have same displayName: %s. Name must be unique!!",
                                 qPrintable(pluginInfoFirst.strLibName),
                                 qPrintable(strLibName),
                                 qPrintable(strDisplayName));
                    }
                }
                else {
                    qWarning("Could not cast plugin %s!", qPrintable(strLibName));
                }
                loader.unload();
            }
            else {
                qWarning("Could not load plugin %s!", qPrintable(strLibName));
            }
        }
        for(auto infoEntry : m_pluginInfoMap) {
            qInfo("Plugin %s found", qPrintable(infoEntry.strDisplayName));
        }
    }
    else {
        qWarning("No plugins found in %s!", ABSOLUTE_PATH_TO_PLUGIN);
    }
}
