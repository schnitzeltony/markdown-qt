#ifndef CMARKGFM_PLUGIN_H
#define CMARKGFM_PLUGIN_H

#include <QtPlugin>
#include "markdown-qt.h"

class CMarkGfmPlugin : public QObject, public PluginInterfaceMdQt
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterfaceMdQt_iid)
    Q_INTERFACES(PluginInterfaceMdQt)

public:
    ~CMarkGfmPlugin() override;
    virtual QList<ConvertType> availableConversions() override;
    virtual QString displayName() override;
    virtual bool convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut) override;
private:
    QByteArray convertToHtml(QByteArray strMarkDownUtf8);
};

#endif // CMARKGFM_PLUGIN_H
