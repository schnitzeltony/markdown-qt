#include "github-markdown-css-plugin.h"
#include <QFile>

CGithubMarkdownCssPlugin::CGithubMarkdownCssPlugin()
{
    Q_INIT_RESOURCE(resources);
}

QList<PluginInterfaceMdQt::ConvertType> CGithubMarkdownCssPlugin::availableConversions()
{
    QList<PluginInterfaceMdQt::ConvertType> supported;
    supported.append({FormatHtml, FormatHtml});
    return supported;
}

QString CGithubMarkdownCssPlugin::displayName()
{
    return QStringLiteral("github-markdown-css");
}

bool CGithubMarkdownCssPlugin::convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut)
{
    bool bSupported = false;
    if(convertType.inFormat==FormatHtml && convertType.inFormat==FormatHtml) {
        strOut = cssHtml(strIn);
    }
    return bSupported;
}

QByteArray CGithubMarkdownCssPlugin::cssHtml(QByteArray strHtmlIn)
{
    QByteArray strHtmlOut;
    QString strStyleName = QStringLiteral(":/github-markdown-css/github-markdown.css");
    QString strStyleFooterName = QStringLiteral(":/styles/github-footer");
    QByteArray strStyle;
    QFile fileStyle(strStyleName);
    if(fileStyle.exists() && fileStyle.open(QFile::ReadOnly | QFile::Unbuffered)) {
        strStyle = fileStyle.readAll();
        fileStyle.close();
    }
    QByteArray strStyleFooter;
    QFile fileStyleFooter(strStyleFooterName);
    if(fileStyleFooter.exists() && fileStyleFooter.open(QFile::ReadOnly | QFile::Unbuffered)) {
        strStyleFooter = fileStyleFooter.readAll();
        fileStyleFooter.close();
    }
    strHtmlOut =
            strStyle +
            strStyleFooter +
            strHtmlIn;
    return strHtmlOut;
}
