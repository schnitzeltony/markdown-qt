#include "markdown-qt.h"
#include <QQmlEngine>
#include <QFile>
#include <QString>
#include <QDir>

static PluginLoaderMdQt sPluginLoader;
static QString sSettingsOrgName;
static QString sSettingsAppName;
static QMap<QString, QMap<QString, PluginBaseMdQt::OptionEntry>> sPluginOptionMap;

void CheckSettings(PluginBaseMdQt *plugin)
{
    if(plugin->hasOptions()) {
        // add / load plugin's options in our map
        QString pluginName = plugin->displayName();
        if(!sPluginOptionMap.contains(pluginName)) {
            // first access?
            if(!sSettingsOrgName.isEmpty() && !sSettingsAppName.isEmpty()) {
                // read from settings
                plugin->loadOptions(sSettingsOrgName, sSettingsAppName);
                // write back defaults
                plugin->storeOptions(sSettingsOrgName, sSettingsAppName);
            }
            sPluginOptionMap[pluginName] = plugin->options();
        }
        else {
            // subsequent access: ensure plugin gets our settings
            plugin->setOptions(sPluginOptionMap[pluginName]);
        }
    }
}


CMarkDownQt::CMarkDownQt(QObject *parent) : QObject(parent)
{
}

int CMarkDownQt::registerQML()
{
    return qmlRegisterSingletonType<CMarkDownQt>("MarkDownQt", 1, 0, "MarkDownQt", CMarkDownQt::getQMLInstance);
}

void CMarkDownQt::setSettingsParameters(QString strOrganisation, QString strApplicationName)
{
    sSettingsOrgName = strOrganisation;
    sSettingsAppName = strApplicationName;
}

QObject *CMarkDownQt::getQMLInstance(QQmlEngine *t_engine, QJSEngine *t_scriptEngine)
{
    Q_UNUSED(t_engine)
    Q_UNUSED(t_scriptEngine)

    return new CMarkDownQt();
}

QStringList CMarkDownQt::availableConverters(DataFormat inFormat, DataFormat outFormat)
{
    PluginBaseMdQt::ConvertType convertType = {inFormat, outFormat};
    return sPluginLoader.listAvailable(convertType);
}

QByteArray CMarkDownQt::convert(QString pluginName, DataFormat inFormat, DataFormat outFormat, QByteArray dataIn)
{
    QByteArray dataOut;
    PluginBaseMdQt *plugin = sPluginLoader.load(pluginName);
    if(plugin) {
        CheckSettings(plugin);
        PluginBaseMdQt::ConvertType convertType = {inFormat, outFormat};
        plugin->convert(convertType, dataIn, dataOut);
    }
    return dataOut;
}

bool CMarkDownQt::convertToFile(QString pluginName, DataFormat inFormat, DataFormat outFormat, QByteArray dataIn, QString strFileOut)
{
    QByteArray dataOut = convert(pluginName, inFormat, outFormat, dataIn);
    bool bFileWritten = false;
    if(!dataOut.isEmpty()) {
        strFileOut = strFileOut.replace("file://", "");
        QFile file(strFileOut);
        if (file.open(QIODevice::WriteOnly)) {
            bFileWritten = file.write(dataOut) > 0;
            file.close();
        }
        else {
            qWarning("Could not open file %s for write!", qPrintable(strFileOut));
        }
    }
    return bFileWritten;
}

QByteArray CMarkDownQt::addFraming(QString pluginName, DataFormat dataFormat, QByteArray dataIn)
{
    QByteArray dataOut;
    PluginBaseMdQt *plugin = sPluginLoader.load(pluginName);
    if(plugin) {
        CheckSettings(plugin);
        plugin->addFraming(dataFormat, dataIn, dataOut);
    }
    return dataOut;
}

