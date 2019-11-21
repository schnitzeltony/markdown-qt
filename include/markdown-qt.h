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

// our plugin wrapper for use by QML
class MARKDOWNQT_EXPORT CMarkDownQt : public QObject
{
  Q_OBJECT
public:
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

// plugin interface - for those not interested in QML-wrapper
// followed https://techbase.kde.org/Policies/Library_Code_Policy/Shared_D-Pointer_Example
class PluginBaseMdQtPrivate;
class MARKDOWNQT_EXPORT PluginBaseMdQt : public QObject
{
public:
    typedef struct _ConvertType{
        CMarkDownQt::DataFormat inFormat;
        CMarkDownQt::DataFormat outFormat;
        bool operator==(const _ConvertType& other) const {
            return inFormat == other.inFormat && outFormat == other.outFormat;
        }
    } ConvertType;
    explicit PluginBaseMdQt(QObject *parent = nullptr);
    virtual ~PluginBaseMdQt();
    virtual QList<ConvertType> availableConversions()= 0;
    virtual QString displayName() = 0;
    virtual bool convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut) = 0;
    virtual bool addFraming(CMarkDownQt::DataFormat dataFormat, const QByteArray dataIn, QByteArray& dataOut);

    // Options
    typedef struct _OptionEntry{
        _OptionEntry();
        _OptionEntry(QString _displayText, QVariant defaultValue);
        QString displayText;
        QVariant value;
    } OptionEntry;
    bool hasOptions();
    QMap<QString, OptionEntry> options();
    void setOptions(QMap<QString, OptionEntry> newOptions);
    bool loadOptions(const QString organizationName, const QString applicationName);
    void storeOptions(const QString organizationName, const QString applicationName);
protected:
    virtual bool initAvailOptions() { return false; }

    const QScopedPointer<PluginBaseMdQtPrivate> d_ptr;
    PluginBaseMdQt(PluginBaseMdQtPrivate &dd, QObject *parent);
    Q_DECLARE_PRIVATE(PluginBaseMdQt)
};

#define PluginBaseMdQt_iid "markdown.qt.PluginBaseMdQt"
Q_DECLARE_INTERFACE(PluginBaseMdQt, PluginBaseMdQt_iid)

// plugin loader
class PluginLoaderMdQtPrivate;
class MARKDOWNQT_EXPORT PluginLoaderMdQt
{
public:
    PluginLoaderMdQt();
    ~PluginLoaderMdQt();

    QStringList listAvailable(const PluginBaseMdQt::ConvertType convertType);
    PluginBaseMdQt *load(const QString strDisplayName);
    bool unload(const QString strDisplayName);
private:
    PluginLoaderMdQtPrivate *d_ptr;
    Q_DECLARE_PRIVATE(PluginLoaderMdQt)
};

QT_END_NAMESPACE

#endif // CMarkDownQt_H
