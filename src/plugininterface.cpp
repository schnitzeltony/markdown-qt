#include "markdown-qt.h"
#include <QFile>

PluginInterfaceMdQt::~PluginInterfaceMdQt()
{
}

bool PluginInterfaceMdQt::addFraming(CMarkDownQt::DataFormat dataFormat, const QByteArray dataIn, QByteArray& dataOut)
{
    // we are the base class and implement a default implementation
    bool bConverted = false;
    if(dataFormat == CMarkDownQt::FormatHtmlUtf8) {
        QByteArray strHeader;
        QFile fileHeader(QStringLiteral(":/common-styles/header"));
        if(fileHeader.exists() && fileHeader.open(QFile::ReadOnly | QFile::Unbuffered)) {
            strHeader = fileHeader.readAll();
            fileHeader.close();
        }
        QByteArray strFooter;
        QFile fileFooter(QStringLiteral(":/common-styles/footer"));
        if(fileFooter.exists() && fileFooter.open(QFile::ReadOnly | QFile::Unbuffered)) {
            strFooter = fileFooter.readAll();
            fileFooter.close();
        }
        dataOut = strHeader + dataIn + strFooter;
        bConverted = true;
    }
    return bConverted;
}
