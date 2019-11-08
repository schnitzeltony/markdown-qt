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

QString CMarkDownQt::doConvert(QString strIn, QString strPlugin, CMarkDownQt::DataFormat inFormat, CMarkDownQt::DataFormat outFormat)
{
    QString strConverted;
    PluginInterfaceMdQt *interface = m_PluginLoader.load(strPlugin);
    if(interface) {
        PluginInterfaceMdQt::ConvertType convertType = {inFormat, outFormat};
        QByteArray inData = strIn.toUtf8();
        QByteArray outData;
        interface->convert(convertType, inData, outData);
        strConverted = QString::fromUtf8(outData);
    }
    return strConverted;
}

QString CMarkDownQt::addFraming(QString strIn, QString strPlugin, DataFormat dataFormat)
{
    QString strConverted;
    PluginInterfaceMdQt *interface = m_PluginLoader.load(strPlugin);
    if(interface) {
        QByteArray inData = strIn.toUtf8();
        QByteArray outData;
        interface->addFraming(dataFormat, inData, outData);
        strConverted = QString::fromUtf8(outData);
    }
    return strConverted;
}


