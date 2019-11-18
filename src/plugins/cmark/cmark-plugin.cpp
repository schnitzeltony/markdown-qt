#include "cmark-plugin.h"
#include "cmark.h"
#include <QStringList>

CMarkPlugin::~CMarkPlugin()
{

}

QList<PluginBaseMdQt::ConvertType> CMarkPlugin::availableConversions()
{
    QList<PluginBaseMdQt::ConvertType> supported;
    supported.append({CMarkDownQt::FormatMdUtf8, CMarkDownQt::FormatHtmlUtf8});
    return supported;
}

QString CMarkPlugin::displayName()
{
    return QStringLiteral("cmark");
}

bool CMarkPlugin::convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut)
{
    bool bSupported = false;
    if(convertType.inFormat==CMarkDownQt::FormatMdUtf8 &&
            convertType.outFormat==CMarkDownQt::FormatHtmlUtf8) {
        dataOut = convertToHtml(dataIn);
    }
    return bSupported;
}

QByteArray CMarkPlugin::convertToHtml(QByteArray strMarkDownUtf8)
{
    return cmark_markdown_to_html(strMarkDownUtf8, size_t(strMarkDownUtf8.size()), CMARK_OPT_DEFAULT);
}
