#include "dummy-plugin.h"

CDummyPlugin::~CDummyPlugin()
{

}

QList<PluginBaseMdQt::ConvertType> CDummyPlugin::availableConversions()
{
    QList<PluginBaseMdQt::ConvertType> supported;
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

bool CDummyPlugin::convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut)
{
    Q_UNUSED(convertType)
    dataOut = dataIn;
    return true;
}
