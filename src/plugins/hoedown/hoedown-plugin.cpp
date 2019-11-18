#include "hoedown-plugin.h"
#include "hoedown-source/src/html.h"
#include <QStringList>

CHoedownPlugin::~CHoedownPlugin()
{

}

QList<PluginBaseMdQt::ConvertType> CHoedownPlugin::availableConversions()
{
    QList<PluginBaseMdQt::ConvertType> supported;
    supported.append({CMarkDownQt::FormatMdUtf8, CMarkDownQt::FormatHtmlUtf8});
    return supported;
}

QString CHoedownPlugin::displayName()
{
    return QStringLiteral("hoedown");
}

bool CHoedownPlugin::convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut)
{
    bool bSupported = false;
    if(convertType.inFormat==CMarkDownQt::FormatMdUtf8 &&
            convertType.outFormat==CMarkDownQt::FormatHtmlUtf8) {
        dataOut = convertToHtml(dataIn);
    }
    return bSupported;
}

QByteArray CHoedownPlugin::convertToHtml(QByteArray strMarkDownUtf8)
{
    QByteArray strHtml;
    if(strMarkDownUtf8.size() > 0) {
        hoedown_buffer *ib, *ob;
        hoedown_renderer *renderer = nullptr;
        void (*renderer_free)(hoedown_renderer *) = nullptr;
        hoedown_document *document;
        hoedown_html_flags html_flags = static_cast<hoedown_html_flags>(0);
        /* Read everything */
        ib = hoedown_buffer_new(1024);
        hoedown_buffer_put(ib, reinterpret_cast<uint8_t*>(strMarkDownUtf8.data()), size_t(strMarkDownUtf8.size()));
        renderer = hoedown_html_renderer_new(html_flags, 0);
        renderer_free = hoedown_html_renderer_free;

        ob = hoedown_buffer_new(64);
        document = hoedown_document_new(
                    renderer,
                    static_cast<hoedown_extensions>(HOEDOWN_EXT_BLOCK | HOEDOWN_EXT_SPAN | HOEDOWN_EXT_FLAGS), 16);

        hoedown_document_render(document, ob, ib->data, ib->size);

        /* Cleanup */
        hoedown_buffer_free(ib);
        hoedown_document_free(document);
        renderer_free(renderer);

        /* Write the result our output */
        strHtml.append(reinterpret_cast<char*>(ob->data), static_cast<int>(ob->size));
        hoedown_buffer_free(ob);
    }
    return strHtml;
}
