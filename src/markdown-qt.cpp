#include "markdown-qt.h"
#include <QQmlEngine>
#include <QFile>
#include <QString>

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

QString CMarkDownQt::stringToHtml(TreatParam paramAs, const QString &strIn, ConvertType convertType, OutputStyle outputStyle)
{
    QByteArray tmpData;
    switch(paramAs) {
        case AsString:
            tmpData = strIn.toUtf8();
            break;
        case AsFilename: {
            QFile cmFile(strIn);
            if(cmFile.exists() && cmFile.open(QFile::ReadOnly | QFile::Unbuffered)) {
                tmpData = cmFile.readAll();
                cmFile.close();
            }
            break;
        }
    }
    QString strHtml;
    if(!tmpData.isEmpty()) {
        switch(convertType) {
            case ConvertCmark: {
                //strHtml = convertToHtmlCmarkGfm(tmpData);
                break;
            }
            case ConvertSundown:
            {
                //strHtml = convertToHtmlSundown(tmpData);
                break;
            }
        }
    }

    // Add style / headers / footers
    QString strHeaderName, strFooterName;
    switch(outputStyle) {
        case StyleGithub:
            strHeaderName = QStringLiteral(":/styles/github-header");
            strFooterName = QStringLiteral(":/styles/common-footer");
            break;
        default:
            strHeaderName = QStringLiteral(":/styles/common-header");
            strFooterName = QStringLiteral(":/styles/common-footer");
            break;
    }
    QString strHeader;
    QFile fileHeader(strHeaderName);
    if(fileHeader.exists() && fileHeader.open(QFile::ReadOnly | QFile::Unbuffered)) {
        strHeader = fileHeader.readAll();
        fileHeader.close();
    }
    QString strFooter;
    QFile fileFooter(strFooterName);
    if(fileFooter.exists() && fileFooter.open(QFile::ReadOnly | QFile::Unbuffered)) {
        strFooter = fileFooter.readAll();
        fileFooter.close();
    }
    strHtml =
            strHeader +
            /*strStyle +
            strStyleFooter +*/
            strHtml +
            strFooter;
    return strHtml;
}

