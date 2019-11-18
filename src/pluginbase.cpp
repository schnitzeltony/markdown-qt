#include "markdown-qt.h"
#include "pluginbase_p.h"
#include <QString>
#include <QFile>
#include <QSettings>

PluginBaseMdQtPrivate::PluginBaseMdQtPrivate(PluginBaseMdQt *pPublic) : q_ptr(pPublic)
{
}

PluginBaseMdQtPrivate::~PluginBaseMdQtPrivate()
{
}


PluginBaseMdQt::_OptionEntry::_OptionEntry(QString _displayText, QVariant defaultValue)
{
    displayText = _displayText;
    value = defaultValue;
}

PluginBaseMdQt::_OptionEntry::_OptionEntry()
{
    displayText = QStringLiteral("Unkown");
    value = QVariant();
}


PluginBaseMdQt::PluginBaseMdQt(QObject *parent) : QObject(parent), d_ptr(new PluginBaseMdQtPrivate(this))
{
}

PluginBaseMdQt::PluginBaseMdQt(PluginBaseMdQtPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

PluginBaseMdQt::~PluginBaseMdQt()
{
}

bool PluginBaseMdQt::addFraming(CMarkDownQt::DataFormat dataFormat, const QByteArray dataIn, QByteArray& dataOut)
{
    // we are the base class and implement a default implementation
    bool bConverted = false;
    if(dataFormat == CMarkDownQt::FormatHtmlUtf8) {
        QByteArray strHeader;
        QFile fileHeader(QStringLiteral(":/common-styles/header"));
        if(fileHeader.exists() && fileHeader.open(QFile::ReadOnly | QFile::Unbuffered)) {
            strHeader = fileHeader.readAll();
            fileHeader.close();
        }
        QByteArray strFooter;
        QFile fileFooter(QStringLiteral(":/common-styles/footer"));
        if(fileFooter.exists() && fileFooter.open(QFile::ReadOnly | QFile::Unbuffered)) {
            strFooter = fileFooter.readAll();
            fileFooter.close();
        }
        dataOut = strHeader + dataIn + strFooter;
        bConverted = true;
    }
    return bConverted;
}

bool PluginBaseMdQt::hasOptions()
{
    Q_D(PluginBaseMdQt);
    return !d->m_optionMap.isEmpty() || initAvailOptions();
}

QMap<QString, PluginBaseMdQt::OptionEntry> PluginBaseMdQt::options()
{
    Q_D(PluginBaseMdQt);
    hasOptions();
    return d->m_optionMap;
}

void PluginBaseMdQt::setOptions(QMap<QString, OptionEntry> newOptions)
{
    // no signalling required
    Q_D(PluginBaseMdQt);
    if(hasOptions()) {
        QMapIterator<QString, OptionEntry> iter(newOptions);
        while (iter.hasNext()) {
            iter.next();
            QString strKey = iter.key();
            if(d->m_optionMap.contains(strKey)) {
                if(d->m_optionMap[strKey].value.type() == iter.value().value.type()) {
                    d->m_optionMap[strKey].value = iter.value().value;
                }
                else {
                    qWarning("PluginBaseMdQt::setOptions option \"%s\": cannot convert %s to %s",
                             qPrintable(iter.key()), iter.value().value.typeName(), d->m_optionMap[strKey].value.typeName());
                }
            }
        }
    }
}

bool PluginBaseMdQt::loadOptions(const QString organizationName, const QString applicationName)
{
    Q_D(PluginBaseMdQt);
    if(hasOptions() && !organizationName.isEmpty() && !applicationName.isEmpty()) {
        QMapIterator<QString, OptionEntry> iter(d->m_optionMap);
        QSettings storedSettings(organizationName, applicationName);
        storedSettings.beginGroup(displayName());
        while (iter.hasNext()) {
            iter.next();
            QVariant value = storedSettings.value(iter.key(), iter.value().value);
            d->m_optionMap[iter.key()].value = value;
        }
        storedSettings.endGroup();
    }
    return d->m_optionMap.count() > 0;
}

void PluginBaseMdQt::storeOptions(const QString organizationName, const QString applicationName)
{
    Q_D(PluginBaseMdQt);
    if(hasOptions() && !organizationName.isEmpty() && !applicationName.isEmpty()) {
        QMapIterator<QString, OptionEntry> iter(d->m_optionMap);
        QSettings storedSettings(organizationName, applicationName);
        storedSettings.beginGroup(displayName());
        while (iter.hasNext()) {
            iter.next();
            storedSettings.setValue(iter.key(), iter.value().value);
        }
        storedSettings.endGroup();
    }
}
