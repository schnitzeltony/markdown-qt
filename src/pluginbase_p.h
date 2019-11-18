#ifndef PLUGININTERFACE_P_H
#define PLUGININTERFACE_P_H

#include "markdown-qt.h"

class PluginBaseMdQtPrivate
{
     Q_DECLARE_PUBLIC(PluginBaseMdQt)
public:
    PluginBaseMdQtPrivate(PluginBaseMdQt* pPublic);
    virtual ~PluginBaseMdQtPrivate();

    PluginBaseMdQt *q_ptr;
    // key: option name stored in settings
    QMap<QString, PluginBaseMdQt::OptionEntry> m_optionMap;
};

#endif // PLUGININTERFACE_P_H
