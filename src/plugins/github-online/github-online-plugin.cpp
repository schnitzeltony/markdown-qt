#include "github-online-plugin.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QEventLoop>

CGithubOnlinePlugin::~CGithubOnlinePlugin()
{
}

QList<PluginInterfaceMdQt::ConvertType> CGithubOnlinePlugin::availableConversions()
{
    QList<PluginInterfaceMdQt::ConvertType> supported;
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

QByteArray CGithubOnlinePlugin::convertToHtml(QByteArray strMarkDownUtf8)
{
    QByteArray strHtml;
    if(strMarkDownUtf8.count() > 0) {
        QUrl serviceUrl = QUrl("https://api.github.com/markdown");
        QNetworkRequest request(serviceUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        QJsonObject json;
        json.insert("text", QString::fromUtf8(strMarkDownUtf8));
        json.insert("mode", QStringLiteral("gfm"));
        // TODO context -> yes we need an option system

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
