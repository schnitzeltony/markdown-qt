#include "markdown-qt.h"
#include <cmark.h>
#include <markdown.h>
#include <html.h>
#include <buffer.h>
#include <QQmlEngine>
#include <QFile>
#include <QString>

CMarkDownQt::CMarkDownQt(QObject *parent) : QObject(parent)
{
}

int CMarkDownQt::registerQML()
{
    return qmlRegisterSingletonType<CMarkDownQt>("CMark", 1, 0, "CMark", CMarkDownQt::getQMLInstance);
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
    switch(convertType) {
        case ConvertCmark:
            strHtml = QString::fromUtf8(cmark_markdown_to_html(tmpData.constData(), size_t(tmpData.size()), CMARK_OPT_DEFAULT));
            break;
        case ConvertSundown:
        {
            struct sd_callbacks callbacks;
            struct html_renderopt options;
            struct sd_markdown *markdown;
            struct buf *sdInputBuf, *sdOutputBuf;
            // fill input buffer (size is constant -> realloc size is fill size)
            sdInputBuf = bufnew(size_t(tmpData.size()));
            bufput(sdInputBuf, tmpData.constData(), size_t(tmpData.size()));
            // out (realloc size taken from example)
            sdOutputBuf = bufnew(size_t(64));
            sdhtml_renderer(&callbacks, &options, 0);
            // select all extensions for now
            markdown = sd_markdown_new(
                        MKDEXT_NO_INTRA_EMPHASIS | MKDEXT_TABLES | MKDEXT_FENCED_CODE | MKDEXT_AUTOLINK | MKDEXT_STRIKETHROUGH | MKDEXT_SPACE_HEADERS | MKDEXT_SUPERSCRIPT | MKDEXT_LAX_SPACING,
                        16, &callbacks, &options);
            // convert
            sd_markdown_render(sdOutputBuf, sdInputBuf->data, sdInputBuf->size, markdown);
            sd_markdown_free(markdown);

            strHtml = QString::fromUtf8(reinterpret_cast<char*>(sdOutputBuf->data), int(sdOutputBuf->size));

            /* cleanup */
            bufrelease(sdInputBuf);
            bufrelease(sdOutputBuf);


            break;
        }
    }

    // Add style / headers / footers
    QString strHeaderName, strFooterName, strStyleName, strStyleFooterName;
    switch(outputStyle) {
    case StyleGithub:
        strHeaderName = QStringLiteral(":/styles/github-header");
        strFooterName = QStringLiteral(":/styles/common-footer");
        strStyleName = QStringLiteral(":/github-markdown-css/github-markdown.css");
        strStyleFooterName = QStringLiteral(":/styles/github-footer");
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
    QString strStyle;
    QFile fileStyle(strStyleName);
    if(fileStyle.exists() && fileStyle.open(QFile::ReadOnly | QFile::Unbuffered)) {
        strStyle = fileStyle.readAll();
        fileStyle.close();
    }
    QString strStyleFooter;
    QFile fileStyleFooter(strStyleFooterName);
    if(fileStyleFooter.exists() && fileStyleFooter.open(QFile::ReadOnly | QFile::Unbuffered)) {
        strStyleFooter = fileStyleFooter.readAll();
        fileStyleFooter.close();
    }

    strHtml =
            strHeader +
            strStyle +
            strStyleFooter +
            strHtml +
            strFooter;

    return strHtml;
}

