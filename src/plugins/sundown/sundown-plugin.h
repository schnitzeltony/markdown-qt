#ifndef CSUNDOWN_PLUGIN_H
#define CSUNDOWN_PLUGIN_H

#include <QtPlugin>
#include "markdown-qt.h"

class CSundownPlugin : public QObject, public PluginInterfaceMdQt
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterfaceMdQt_iid)
    Q_INTERFACES(PluginInterfaceMdQt)

public:
    ~CSundownPlugin() override;
    virtual QList<ConvertType> availableConversions() override;
    virtual QString displayName() override;
    virtual bool convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut) override;
private:
    QByteArray convertToHtml(QByteArray strMarkDownUtf8);
};

#endif // CSUNDOWN_PLUGIN_H
