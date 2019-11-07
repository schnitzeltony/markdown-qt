#ifndef CGITHUBMARKCSS_PLUGIN_H
#define CGITHUBMARKCSS_PLUGIN_H

#include <QtPlugin>
#include "markdown-qt.h"

class CGithubMarkdownCssPlugin : public QObject, public MdQtPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID MdQtPluginInterface_iid)
    Q_INTERFACES(MdQtPluginInterface)

public:
    CGithubMarkdownCssPlugin();
    ~CGithubMarkdownCssPlugin() override {}
    virtual QList<ConvertType> availableConversion() override;
    virtual QString displayName() override;
    virtual bool convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut) override;
private:
    QByteArray cssHtml(QByteArray strHtmlIn);
};

#endif // CGITHUBMARKCSS_PLUGIN_H
