#include "cmark-gfm-plugin.h"
#include "pluginbase_p.h"
#include "cmark-gfm-source/src/cmark-gfm.h"
#include "cmark-gfm-source/extensions/cmark-gfm-core-extensions.h"
#include "cmark-gfm-source/src/html.h"
#include "parser.h"
#include "htmlhelper.h"
#include <QMapIterator>

CMarkGfmPlugin::~CMarkGfmPlugin()
{

}

QList<PluginBaseMdQt::ConvertType> CMarkGfmPlugin::availableConversions()
{
    QList<PluginBaseMdQt::ConvertType> supported;
    supported.append({CMarkDownQt::FormatMdUtf8, CMarkDownQt::FormatHtmlUtf8});
    return supported;
}

QString CMarkGfmPlugin::displayName()
{
    return QStringLiteral("cmark-gfm");
}

bool CMarkGfmPlugin::convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut)
{
    bool bSupported = false;
    if(convertType.inFormat==CMarkDownQt::FormatMdUtf8 &&
            convertType.outFormat==CMarkDownQt::FormatHtmlUtf8) {
        dataOut = convertToHtml(dataIn);
    }
    return bSupported;
}

bool CMarkGfmPlugin::initAvailOptions()
{
    Q_D(PluginBaseMdQt);
    d->m_optionMap[QStringLiteral("CMARK_OPT_UNSAFE")] =
            OptionEntry(tr("Allow unsafe HTML"), true);
    d->m_optionMap[QStringLiteral("CMARK_OPT_LIBERAL_HTML_TAG")] =
            OptionEntry(tr("Liberal HTML tag"), true);
    d->m_optionMap[QStringLiteral("CMARK_OPT_GITHUB_PRE_LANG")] =
            OptionEntry(tr("Use GitHub-style <pre lang=\"x\"> tags"), false);
    d->m_optionMap[QStringLiteral("CMARK_OPT_FULL_INFO_STRING")] =
            OptionEntry(tr("Include remainder of info string in code blocks as separate attribute"), false);

    d->m_optionMap[QStringLiteral("CMARK_EXT_FOOTNOTES")] =
            OptionEntry(tr("Enable footnotes extension"), false);
    d->m_optionMap[QStringLiteral("CMARK_EXT_TABLE")] =
            OptionEntry(tr("Enable table extension"), true);
    d->m_optionMap[QStringLiteral("CMARK_EXT_STRIKETHROUGH")] =
            OptionEntry(tr("Enable strikethrough extension"), true);
    d->m_optionMap[QStringLiteral("CMARK_EXT_AUTOLINK")] =
            OptionEntry(tr("Enable autolink extension"), true);
    d->m_optionMap[QStringLiteral("CMARK_EXT_TAGFILTER")] =
            OptionEntry(tr("Enable tagfilter extension"), true);
    d->m_optionMap[QStringLiteral("CMARK_EXT_TASKLIST")] =
            OptionEntry(tr("Enable tasklist extension"), true);
    // TODO add more options
    return true;
}

QByteArray CMarkGfmPlugin::convertToHtml(QByteArray strMarkDownUtf8)
{
    Q_D(PluginBaseMdQt);
    int options = 0;
    if(hasOptions()) {
        if(d->m_optionMap[QStringLiteral("CMARK_OPT_UNSAFE")].value.toBool()) {
            options |= CMARK_OPT_UNSAFE;
        }
        if(d->m_optionMap[QStringLiteral("CMARK_OPT_LIBERAL_HTML_TAG")].value.toBool()) {
            options |= CMARK_OPT_LIBERAL_HTML_TAG;
        }
        if(d->m_optionMap[QStringLiteral("CMARK_OPT_GITHUB_PRE_LANG")].value.toBool()) {
            options |= CMARK_OPT_GITHUB_PRE_LANG;
        }
        if(d->m_optionMap[QStringLiteral("CMARK_OPT_FULL_INFO_STRING")].value.toBool()) {
            options |= CMARK_OPT_FULL_INFO_STRING;
        }
    }
    cmark_gfm_core_extensions_ensure_registered();
    cmark_parser *parser = cmark_parser_new_with_mem(options, cmark_get_arena_mem_allocator());
    QByteArray strHtml;
    if(parser) {
        if(hasOptions()) {
            QStringList listExtensions;
            if(d->m_optionMap[QStringLiteral("CMARK_EXT_FOOTNOTES")].value.toBool()) {
                listExtensions.append(QStringLiteral("footnotes"));
            }
            if(d->m_optionMap[QStringLiteral("CMARK_EXT_TABLE")].value.toBool()) {
                listExtensions.append(QStringLiteral("table"));
            }
            if(d->m_optionMap[QStringLiteral("CMARK_EXT_STRIKETHROUGH")].value.toBool()) {
                listExtensions.append(QStringLiteral("strikethrough"));
            }
            if(d->m_optionMap[QStringLiteral("CMARK_EXT_AUTOLINK")].value.toBool()) {
                listExtensions.append(QStringLiteral("autolink"));
            }
            if(d->m_optionMap[QStringLiteral("CMARK_EXT_TAGFILTER")].value.toBool()) {
                listExtensions.append(QStringLiteral("tagfilter"));
            }
            if(d->m_optionMap[QStringLiteral("CMARK_EXT_TASKLIST")].value.toBool()) {
                listExtensions.append(QStringLiteral("tasklist"));
            }
            for(auto strExtension : listExtensions) {
                cmark_syntax_extension *syntax_extension = cmark_find_syntax_extension(qPrintable(strExtension));
                if(syntax_extension) {
                    cmark_parser_attach_syntax_extension(parser, syntax_extension);
                }

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
            HtmlHelper htmlHelper;
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
