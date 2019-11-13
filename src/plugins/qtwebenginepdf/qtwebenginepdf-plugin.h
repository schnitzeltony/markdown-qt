#ifndef CQTWEBENGINEPDF_PLUGIN_H
#define CQTWEBENGINEPDF_PLUGIN_H

#include <QtPlugin>
#include "markdown-qt.h"

class CQtWebEnginePdfPlugin : public QObject, public PluginInterfaceMdQt
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterfaceMdQt_iid)
    Q_INTERFACES(PluginInterfaceMdQt)

public:
    CQtWebEnginePdfPlugin();
    ~CQtWebEnginePdfPlugin() override;
    virtual QList<ConvertType> availableConversions() override;
    virtual QString displayName() override;
    virtual bool convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut) override;
private:
    QByteArray convertToPdf(QByteArray strHtmlUtf8);
};

#endif // CQTWEBENGINEPDF_PLUGIN_H
