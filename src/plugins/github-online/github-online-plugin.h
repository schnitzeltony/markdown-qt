#ifndef CGITHUBONLINE_PLUGIN_H
#define CGITHUBONLINE_PLUGIN_H

#include <QtPlugin>
#include "markdown-qt.h"

class CGithubOnlinePlugin : public PluginBaseMdQt
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginBaseMdQt_iid)
    Q_INTERFACES(PluginBaseMdQt)

public:
    ~CGithubOnlinePlugin() override;
    virtual QList<ConvertType> availableConversions() override;
    virtual QString displayName() override;
    virtual bool convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut) override;
protected:
    virtual bool initAvailOptions() override;
private:
    QByteArray convertToHtml(QByteArray strMarkDownUtf8);
};

#endif // CGITHUBONLINE_PLUGIN_H
