#include "cmark-gfm-plugin.h"
#include "cmark-gfm-source/src/cmark-gfm.h"
#include "cmark-gfm-source/extensions/cmark-gfm-core-extensions.h"
#include "cmark-gfm-source/src/html.h"
#include "parser.h"
#include <QStringList>

CMarkGfmPlugin::~CMarkGfmPlugin()
{

}

QList<PluginInterfaceMdQt::ConvertType> CMarkGfmPlugin::availableConversions()
{
    QList<PluginInterfaceMdQt::ConvertType> supported;
    supported.append({CMarkDownQt::FormatMd, CMarkDownQt::FormatHtml});
    return supported;
}

QString CMarkGfmPlugin::displayName()
{
    return QStringLiteral("cmark-gfm");
}

bool CMarkGfmPlugin::convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut)
{
    bool bSupported = false;
    if(convertType.inFormat==CMarkDownQt::FormatMd &&
            convertType.outFormat==CMarkDownQt::FormatHtml) {
        strOut = convertToHtml(strIn);
    }
    return bSupported;
}

QByteArray CMarkGfmPlugin::convertToHtml(QByteArray strMarkDownUtf8)
{
    int options = CMARK_OPT_DEFAULT;
    cmark_gfm_core_extensions_ensure_registered();
    cmark_parser *parser = cmark_parser_new_with_mem(options, cmark_get_arena_mem_allocator());
    QByteArray strHtml;
    if(parser) {
        QStringList listExtensions;
        //listExtensions.append(QStringLiteral("footnotes"));
        listExtensions.append(QStringLiteral("table"));
        listExtensions.append(QStringLiteral("strikethrough"));
        listExtensions.append(QStringLiteral("autolink"));
        listExtensions.append(QStringLiteral("tagfilter"));
        listExtensions.append(QStringLiteral("tasklist"));
        for(auto strExtension : listExtensions) {
            cmark_syntax_extension *syntax_extension = cmark_find_syntax_extension(qPrintable(strExtension));
            if(syntax_extension) {
                cmark_parser_attach_syntax_extension(parser, syntax_extension);
            }

        }
        cmark_parser_feed(parser, strMarkDownUtf8, size_t(strMarkDownUtf8.size()));
        cmark_node *document = cmark_parser_finish(parser);
        if(document) {
            strHtml = cmark_render_html_with_mem(document, options, parser->syntax_extensions, cmark_get_default_mem_allocator());
        }
        else {
            qWarning("CMARK no document node created");
        }
        cmark_arena_reset();
    }
    else {
        qWarning("CMARK parser could not be created");
    }
    return strHtml;
}
