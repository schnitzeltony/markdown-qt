#include "markdown-qt.h"
#include <QQmlEngine>
#include <QFile>
#include <QString>

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

QString CMarkDownQt::convert(QString pluginName, CMarkDownQt::DataFormat inFormat, CMarkDownQt::DataFormat outFormat, QString strIn)
{
    QString strConverted;
    PluginInterfaceMdQt *interface = m_PluginLoader.load(pluginName);
    if(interface) {
        PluginInterfaceMdQt::ConvertType convertType = {inFormat, outFormat};
        QByteArray inData = strIn.toUtf8();
        QByteArray outData;
        interface->convert(convertType, inData, outData);
        strConverted = QString::fromUtf8(outData);
    }
    return strConverted;
}

bool CMarkDownQt::convertToFile(QString pluginName, DataFormat inFormat, DataFormat outFormat, QString strIn, QString strFileOut)
{
    QString strConverted = convert(pluginName, inFormat, outFormat, strIn);
    bool bFileWritten = false;
    if(!strConverted.isEmpty()) {
        QByteArray dataWrite = strConverted.toUtf8();
        strFileOut = strFileOut.replace("file://", "");
        QFile file(strFileOut);
        if (file.open(QIODevice::WriteOnly)) {
            bFileWritten = file.write(dataWrite) > 0;
            file.close();
        }
        else {
            qWarning("Could not open file %s for write!", qPrintable(strFileOut));
        }
    }
    return bFileWritten;
}

QString CMarkDownQt::addFraming(QString pluginName, DataFormat dataFormat, QString strIn)
{
    QString strConverted;
    PluginInterfaceMdQt *interface = m_PluginLoader.load(pluginName);
    if(interface) {
        QByteArray inData = strIn.toUtf8();
        QByteArray outData;
        interface->addFraming(dataFormat, inData, outData);
        strConverted = QString::fromUtf8(outData);
    }
    return strConverted;
}


