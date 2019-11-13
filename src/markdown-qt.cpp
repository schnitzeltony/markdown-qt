#include "markdown-qt.h"
#include <QQmlEngine>
#include <QFile>
#include <QString>
#include <QDir>

PluginLoaderMdQt CMarkDownQt::m_PluginLoader = PluginLoaderMdQt();

CMarkDownQt::CMarkDownQt(QObject *parent) : QObject(parent)
{
}

int CMarkDownQt::registerQML()
{
    return qmlRegisterSingletonType<CMarkDownQt>("MarkDownQt", 1, 0, "MarkDownQt", CMarkDownQt::getQMLInstance);
}

QObject *CMarkDownQt::getQMLInstance(QQmlEngine *t_engine, QJSEngine *t_scriptEngine)
{
    Q_UNUSED(t_engine)
    Q_UNUSED(t_scriptEngine)

    return new CMarkDownQt();
}

QStringList CMarkDownQt::availableConverters(DataFormat inFormat, DataFormat outFormat)
{
    PluginInterfaceMdQt::ConvertType convertType = {inFormat, outFormat};
    return m_PluginLoader.listAvailable(convertType);
}

QByteArray CMarkDownQt::convert(QString pluginName, DataFormat inFormat, DataFormat outFormat, QByteArray dataIn)
{
    QByteArray dataOut;
    PluginInterfaceMdQt *interface = m_PluginLoader.load(pluginName);
    if(interface) {
        PluginInterfaceMdQt::ConvertType convertType = {inFormat, outFormat};
        interface->convert(convertType, dataIn, dataOut);
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
    PluginInterfaceMdQt *interface = m_PluginLoader.load(pluginName);
    if(interface) {
        interface->addFraming(dataFormat, dataIn, dataOut);
    }
    return dataOut;
}

