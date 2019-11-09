#include "cmark-plugin.h"
#include "cmark.h"
#include <QStringList>

CMarkPlugin::~CMarkPlugin()
{

}

QList<PluginInterfaceMdQt::ConvertType> CMarkPlugin::availableConversions()
{
    QList<PluginInterfaceMdQt::ConvertType> supported;
    supported.append({CMarkDownQt::FormatMd, CMarkDownQt::FormatHtml});
    return supported;
}

QString CMarkPlugin::displayName()
{
    return QStringLiteral("cmark");
}

bool CMarkPlugin::convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut)
{
    bool bSupported = false;
    if(convertType.inFormat==CMarkDownQt::FormatMd &&
            convertType.outFormat==CMarkDownQt::FormatHtml) {
        strOut = convertToHtml(strIn);
    }
    return bSupported;
}

QByteArray CMarkPlugin::convertToHtml(QByteArray strMarkDownUtf8)
{
    return cmark_markdown_to_html(strMarkDownUtf8, size_t(strMarkDownUtf8.size()), CMARK_OPT_DEFAULT);
}
