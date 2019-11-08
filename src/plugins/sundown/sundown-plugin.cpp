#include "sundown-plugin.h"
#include "sundown-source/html/html.h"
#include <QStringList>

CSundownPlugin::~CSundownPlugin()
{

}

QList<PluginInterfaceMdQt::ConvertType> CSundownPlugin::availableConversions()
{
    QList<PluginInterfaceMdQt::ConvertType> supported;
    supported.append({CMarkDownQt::FormatMd, CMarkDownQt::FormatHtml});
    return supported;
}

QString CSundownPlugin::displayName()
{
    return QStringLiteral("sundown");
}

bool CSundownPlugin::convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut)
{
    bool bSupported = false;
    if(convertType.inFormat==CMarkDownQt::FormatMd &&
            convertType.outFormat==CMarkDownQt::FormatHtml) {
        strOut = convertToHtml(strIn);
    }
    return bSupported;
}

QByteArray CSundownPlugin::convertToHtml(QByteArray strMarkDownUtf8)
{
    QByteArray strHtml;
    struct sd_callbacks callbacks;
    struct html_renderopt options;
    struct sd_markdown *markdown;
    struct buf *sdInputBuf, *sdOutputBuf;
    // fill input buffer (size is constant -> realloc size is fill size)
    sdInputBuf = bufnew(size_t(strMarkDownUtf8.size()));
    bufput(sdInputBuf, strMarkDownUtf8, size_t(strMarkDownUtf8.size()));
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

    strHtml = QByteArray(reinterpret_cast<char*>(sdOutputBuf->data), int(sdOutputBuf->size));

    /* cleanup */
    bufrelease(sdInputBuf);
    bufrelease(sdOutputBuf);

    return strHtml;
}
