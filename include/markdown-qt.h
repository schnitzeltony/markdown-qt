#ifndef CMarkDownQt_H
#define CMarkDownQt_H

#include "markdownqt_export.h"
#include <QtPlugin>
#include <QPluginLoader>
#include <QList>
#include <QMap>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QQmlEngine;
class QJSEngine;
class PluginLoaderMdQt;


// QML plugin wrapper
class MARKDOWNQT_EXPORT CMarkDownQt : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Enum with data in/out formats
     */
    enum DataFormat {
        FormatMdUtf8 = 0,
        FormatHtmlUtf8 = 1,
        FormatPdfBin = 2,
        FormatPrinterOut = 3,

        FormatUnknown // don't forget to update validDataFormats below
    };
    Q_ENUM(DataFormat)

    /**
     * @brief Register CMarkDownQt 1.0 to Qml
     * @return return value of qmlRegisterSingletonType
     */
    static int registerQML();

    /**
     * @brief setSettingsParameters Pass application's QSettings to the library so it will load store in same file
     * @param strOrganisation Applications Organisation (see QSettings for further details - by default subfolder in $HOME/.config)
     * @param strApplicationName Applicartion name (see QSettings for further details)
     */
    Q_INVOKABLE static void setSettingsParameters(QString strOrganisation, QString strApplicationName);

    /**
     * @brief Ask for available converters matching in/out data types
     * @param inFormat in-data format
     * @param outFormat out-data format
     * @return string-list of matching converters
     */
    Q_INVOKABLE static QStringList availableConverters(DataFormat inFormat, DataFormat outFormat);

    /**
     * @brief Convert in -> out
     * @param pluginName plugin name of plugin to perform conversion
     * @param inFormat in-data format
     * @param outFormat out-data format
     * @param dataIn data to convert
     * @return converted data / empty on error
     */
    Q_INVOKABLE static QByteArray convert(QString pluginName, DataFormat inFormat, DataFormat outFormat, QByteArray dataIn);

    /**
     * @brief Convert in -> file
     * @param pluginName plugin name of plugin to perform conversion
     * @param inFormat in-data format
     * @param outFormat out-data format
     * @param dataIn data to convert
     * @param strFileOut filename converted output is written to
     * @return true if file was written
     */
    Q_INVOKABLE static bool convertToFile(QString pluginName, DataFormat inFormat, DataFormat outFormat, QByteArray dataIn, QString strFileOut);

    /**
     * @brief Add frames e.g HTML headers/footers/css
     * @param dataIn data to add framing
     * @param pluginName plugin name of plugin to perform conversion
     * @param dataFormat data format of input and output
     * @return converted data / empty on error
     */
    Q_INVOKABLE static QByteArray addFraming(QString pluginName, DataFormat dataFormat, QByteArray dataIn);

signals:

public slots:

private:
    explicit CMarkDownQt(QObject *parent = nullptr);
    static QObject *getQMLInstance(QQmlEngine *t_engine, QJSEngine *t_scriptEngine);
};

// hmm should be member of CMarkDownQt.. / printer is not a data format
constexpr std::initializer_list<CMarkDownQt::DataFormat> validDataFormats =
{
    CMarkDownQt::FormatMdUtf8,
    CMarkDownQt::FormatHtmlUtf8,
    CMarkDownQt::FormatPdfBin
};

// clang: be quiet
QT_WARNING_DISABLE_CLANG("-Wshadow-field")


// Abstract Qt/C++ plugin interface
// pimpl followed https://techbase.kde.org/Policies/Library_Code_Policy/Shared_D-Pointer_Example
class PluginBaseMdQtPrivate;
class MARKDOWNQT_EXPORT PluginBaseMdQt : public QObject
{
public:
    /* A conversion type is defined by a pair of inFormat -> outFormat */
    typedef struct _ConvertType{
        CMarkDownQt::DataFormat inFormat;
        CMarkDownQt::DataFormat outFormat;
        bool operator==(const _ConvertType& other) const {
            return inFormat == other.inFormat && outFormat == other.outFormat;
        }
    } ConvertType;
    explicit PluginBaseMdQt(QObject *parent = nullptr);
    virtual ~PluginBaseMdQt();
    /**
     * @brief availableConversions To be implemented by plugin
     * @return plugin's list of available conversions
     */
    virtual QList<ConvertType> availableConversions() = 0;
    /**
     * @brief displayName To be implemented by plugin
     * @return Display name
     */
    virtual QString displayName() = 0;
    /**
     * @brief convert Do the conversion - to be implemented by the plugin
     * @param convertType Type of conversion (for plugins supporting more than one conversion type)
     * @param dataIn In QByteArray
     * @param dataOut Out QByteArry
     * @return tru on sucess (e.g conversion type supported)
     */
    virtual bool convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut) = 0;
    /**
     * @brief addFraming Add framing (HTML default implementation on base class ) to pure HTML
     * @param dataFormat all implementations currently support CMarkDownQt::FormatHtmlUtf8 only
     * @param dataIn pure HTML wothout header / footer
     * @param dataOut framed out data
     * @return
     */
    virtual bool addFraming(CMarkDownQt::DataFormat dataFormat, const QByteArray dataIn, QByteArray& dataOut);

    /* Options: Plugins can load/store from/to configuration file to make options persistent */
    typedef struct _OptionEntry{
        _OptionEntry();
        _OptionEntry(QString _displayText, QVariant defaultValue);
        QString displayText;
        QVariant value;
    } OptionEntry;
    /**
     * @brief hasOptions Check options map / on empty try initAvailOptions()
     * @return
     */
    bool hasOptions();
    /**
     * @brief options Get plugin's option map
     * @return option map
     */
    QMap<QString, OptionEntry> options();
    /**
     * @brief setOptions overwrite plugin's option map (partially)
     * @param newOptions
     */
    void setOptions(QMap<QString, OptionEntry> newOptions);
    /**
     * @brief loadOptions Load options from config file ($HOME/.config/<organization>/<application>.conf)
     * @param organizationName organization name for QSettings
     * @param applicationName application name for QSettings
     * @return true: option map is not empty
     */
    bool loadOptions(const QString organizationName, const QString applicationName);
    /**
     * @brief storeOptions Store plugin's opton map to config file ($HOME/.config/<organization>/<application>.conf)
     * @param organizationName organization name for QSettings
     * @param applicationName application name for QSettings
     */
    void storeOptions(const QString organizationName, const QString applicationName);
protected:
    /**
     * @brief initAvailOptions Plugins override this to register their options in optons map
     * @return true: Plugin has options
     */
    virtual bool initAvailOptions() { return false; }

    const QScopedPointer<PluginBaseMdQtPrivate> d_ptr;
    PluginBaseMdQt(PluginBaseMdQtPrivate &dd, QObject *parent);
    Q_DECLARE_PRIVATE(PluginBaseMdQt)
};

// 'register' plugin id/interface
#define PluginBaseMdQt_iid "markdown.qt.PluginBaseMdQt"
Q_DECLARE_INTERFACE(PluginBaseMdQt, PluginBaseMdQt_iid)


// The plugin loader
class PluginLoaderMdQtPrivate;
class MARKDOWNQT_EXPORT PluginLoaderMdQt
{
public:
    PluginLoaderMdQt();
    ~PluginLoaderMdQt();
    /**
     * @brief listAvailable List all plugins in FULL_PLUGIN_PATH (see CMakeLists.txt how to change)
     * @param convertType Return only those with matchong convert-type
     * @return String list of plugin's display names found
     */
    QStringList listAvailable(const PluginBaseMdQt::ConvertType convertType);
    /**
     * @brief load Load a plugin
     * @param strDisplayName Display name of the plugin
     * @return pointer to plugin
     */
    PluginBaseMdQt *load(const QString strDisplayName);
    /**
     * @brief unload Unload a plugin
     * @param strDisplayName Display name of the plugin
     * @return plugin was unloaded: do not use it's ponter anymore
     */
    bool unload(const QString strDisplayName);
private:
    PluginLoaderMdQtPrivate *d_ptr;
    Q_DECLARE_PRIVATE(PluginLoaderMdQt)
};

QT_END_NAMESPACE

#endif // CMarkDownQt_H
