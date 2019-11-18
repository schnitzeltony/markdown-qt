#ifndef CSUNDOWN_PLUGIN_H
#define CSUNDOWN_PLUGIN_H

#include <QtPlugin>
#include "markdown-qt.h"

class CDummyPlugin : public PluginBaseMdQt
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginBaseMdQt_iid)
    Q_INTERFACES(PluginBaseMdQt)

public:
    ~CDummyPlugin() override;
    virtual QList<ConvertType> availableConversions() override;
    virtual QString displayName() override;
    virtual bool convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut) override;
};

#endif // CSUNDOWN_PLUGIN_H
