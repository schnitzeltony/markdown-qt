#include "cmark-gfm-plugin.h"
#include "cmark-gfm-source/src/cmark-gfm.h"
#include "cmark-gfm-source/extensions/cmark-gfm-core-extensions.h"
#include "cmark-gfm-source/src/html.h"
#include "parser.h"
#include "chtmlhelper.h"
#include <QMapIterator>

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
    int options = CMARK_OPT_UNSAFE | CMARK_OPT_LIBERAL_HTML_TAG | CMARK_OPT_GITHUB_PRE_LANG | CMARK_OPT_FULL_INFO_STRING; /* TODO options for plugins */
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
            // some github specific post-processing
            // Tasklist first lines
            strHtml = strHtml.replace(
                        "<ul>\n<li><input type=\"checkbox\"",
                        "<ul class=\"contains-task-list\">\n<li class=\"task-list-item\"><input type=\"checkbox\"");
            // Tasklist other lines
            strHtml = strHtml.replace(
                        "<li><input type=\"checkbox\"",
                        "<li class=\"task-list-item\"><input type=\"checkbox\"");
            // re-sort <p> for checklist text
            // <li class="task-list-item"><input type="checkbox" checked="" disabled="" /><p>foo</p>
            // turns into
            // <p><li class="task-list-item"><input type="checkbox" checked="" disabled="" />foo</p>
            CHtmlHelper htmlHelper;
            if(htmlHelper.parse(strHtml)) {
                const QMap<int, TagInfo>& tagMap = htmlHelper.GetTagMap();
                int currSearchPos = 0;
                do {
                    currSearchPos = strHtml.indexOf("<input type=\"checkbox\"", currSearchPos);
                    if(currSearchPos >= 0) {
                        if(tagMap.contains(currSearchPos)) {
                            auto iter = tagMap.find(currSearchPos);
                            const TagInfo& tagInfo = iter.value();
                            currSearchPos = tagInfo.endPos+1;
                            iter++;
                            if(iter != tagMap.constEnd()) {
                                const TagInfo& tagInfoNext = iter.value();
                                QByteArray nextTag = strHtml.mid(tagInfoNext.startPos, tagInfoNext.endPos+1 - tagInfoNext.startPos);
                                if(nextTag == "<p>") {
                                    currSearchPos = tagInfoNext.endPos+1;
                                    QByteArray leftPart = strHtml.left(tagInfo.startPos);
                                    QByteArray midPart = QByteArrayLiteral("<p>") + strHtml.mid(tagInfo.startPos, tagInfo.endPos+1 - tagInfo.startPos);
                                    QByteArray rightPart = strHtml.right(strHtml.size()-tagInfoNext.endPos-1);
                                    strHtml = leftPart + midPart + QByteArrayLiteral(" ") + rightPart;
                                    // by adding a space all following tags are not matching anymore
                                    htmlHelper.parse(strHtml, leftPart.size()+midPart.size());
                                }
                            }
                        }
                        else {
                            // should not happen - but avoids deadlock
                            currSearchPos++;
                        }
                    }
                } while(currSearchPos >= 0 && currSearchPos < strHtml.count());
            }
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
