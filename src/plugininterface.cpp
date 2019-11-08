#include "markdown-qt.h"
#include <QFile>

PluginInterfaceMdQt::~PluginInterfaceMdQt()
{
}

bool PluginInterfaceMdQt::addFraming(CMarkDownQt::DataFormat dataFormat, const QByteArray strIn, QByteArray& strOut)
{
    // we are the base class and implement a default implementation
    bool bConverted = false;
    if(dataFormat == CMarkDownQt::FormatHtml) {
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
        strOut = strHeader + strIn + strFooter;
        bConverted = true;
    }
    return bConverted;
}
