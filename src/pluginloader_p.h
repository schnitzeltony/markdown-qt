#ifndef PLUGINLOADER_P_H
#define PLUGINLOADER_P_H

#include "markdown-qt.h"

typedef struct _PluginInfo{
    QString strDisplayName;
    QList<PluginBaseMdQt::ConvertType> conversionTypes;
    QPluginLoader *pluginLoader;
} PluginInfo;

class PluginLoaderMdQtPrivate
{
     Q_DECLARE_PUBLIC(PluginLoaderMdQt)
public:
    PluginLoaderMdQtPrivate(PluginLoaderMdQt* pPublic);
    ~PluginLoaderMdQtPrivate();

    bool ScanPlugins();
private:
    PluginLoaderMdQt *q_ptr;
    // key display string
    QMap<QString, PluginInfo> m_pluginInfoMap;
};

#endif // PLUGINLOADER_P_H
