#include "dummy-plugin.h"

CDummyPlugin::~CDummyPlugin()
{

}

QList<PluginInterfaceMdQt::ConvertType> CDummyPlugin::availableConversions()
{
    QList<PluginInterfaceMdQt::ConvertType> supported;
    for(auto inFormat : validDataFormats) {
        for(auto outFormat : validDataFormats) {
            supported.append({inFormat, outFormat});
        }
    }
    return supported;
}

QString CDummyPlugin::displayName()
{
    return QStringLiteral("dummy");
}

bool CDummyPlugin::convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut)
{
    Q_UNUSED(convertType)
    strOut = strIn;
    return true;
}
