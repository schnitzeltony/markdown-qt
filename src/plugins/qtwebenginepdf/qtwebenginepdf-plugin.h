#ifndef CQTWEBENGINEPDF_PLUGIN_H
#define CQTWEBENGINEPDF_PLUGIN_H

#include <QtPlugin>
#include "markdown-qt.h"

class CQtWebEnginePdfPlugin : public PluginBaseMdQt
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginBaseMdQt_iid)
    Q_INTERFACES(PluginBaseMdQt)

public:
    CQtWebEnginePdfPlugin();
    ~CQtWebEnginePdfPlugin() override;
    virtual QList<ConvertType> availableConversions() override;
    virtual QString displayName() override;
    virtual bool convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut) override;
private:
    QByteArray convertToPdfOrPrinter(QByteArray strHtmlUtf8, CMarkDownQt::DataFormat outFormat);
};

#endif // CQTWEBENGINEPDF_PLUGIN_H
