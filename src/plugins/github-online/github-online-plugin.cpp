#include "github-online-plugin.h"
#include "pluginbase_p.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QEventLoop>

CGithubOnlinePlugin::~CGithubOnlinePlugin()
{
}

QList<PluginBaseMdQt::ConvertType> CGithubOnlinePlugin::availableConversions()
{
    QList<PluginBaseMdQt::ConvertType> supported;
    supported.append({CMarkDownQt::FormatMdUtf8, CMarkDownQt::FormatHtmlUtf8});
    return supported;
}

QString CGithubOnlinePlugin::displayName()
{
    return QStringLiteral("github-online");
}

bool CGithubOnlinePlugin::convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut)
{
    bool bSupported = false;
    if(convertType.inFormat==CMarkDownQt::FormatMdUtf8 &&
            convertType.outFormat==CMarkDownQt::FormatHtmlUtf8) {
        dataOut = convertToHtml(dataIn);
    }
    return bSupported;
}

bool CGithubOnlinePlugin::initAvailOptions()
{
    Q_D(PluginBaseMdQt);
    d->m_optionMap[QStringLiteral("mode")] =
            OptionEntry(tr("Conversion mode \"gfm\"/\"markdown\""), QStringLiteral("gfm"));
    d->m_optionMap[QStringLiteral("context")] =
            OptionEntry(tr("Repository context e.g \"github/gollum\""), QStringLiteral(""));
    return true;
}

QByteArray CGithubOnlinePlugin::convertToHtml(QByteArray strMarkDownUtf8)
{
    Q_D(PluginBaseMdQt);
    QByteArray strHtml;
    if(strMarkDownUtf8.count() > 0) {
        QUrl serviceUrl = QUrl("https://api.github.com/markdown");
        QNetworkRequest request(serviceUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        QJsonObject json;
        json.insert("text", QString::fromUtf8(strMarkDownUtf8));
        QString strMode = QStringLiteral("gfm");
        QString strContext;
        if(hasOptions()) {
            QString strModeFromSetting = d->m_optionMap[QStringLiteral("mode")].value.toString();
            if(!strModeFromSetting.isEmpty() &&
                    (strModeFromSetting == QStringLiteral("gfm") || strModeFromSetting == QStringLiteral("markdown"))) {
                strMode = strModeFromSetting;
            }
            strContext = d->m_optionMap[QStringLiteral("context")].value.toString();
        }
        json.insert("mode", strMode);
        if(!strContext.isEmpty()) {
            json.insert("context", strContext);
        }

        QNetworkAccessManager networkManager;
        networkManager.post(request, QJsonDocument(json).toJson());
        QEventLoop loop;
        connect(&networkManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
            strHtml = reply->readAll();
            loop.quit();
        });
        loop.exec();
    }
    return strHtml;
}
