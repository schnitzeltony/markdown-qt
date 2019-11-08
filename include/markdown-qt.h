#ifndef CMarkDownQt_H
#define CMarkDownQt_H

#include "markdownqt_export.h"
#include <QtPlugin>
#include <QPluginLoader>
#include <QList>
#include <QMap>

// TODO PIMPL !!!!!!

QT_BEGIN_NAMESPACE
class QQmlEngine;
class QJSEngine;
class PluginLoaderMdQt;
QT_END_NAMESPACE



// our plugin wrapper for use by QML
class MARKDOWNQT_EXPORT CMarkDownQt : public QObject
{
  Q_OBJECT
public:
  enum DataFormat {
    FormatMd = 0,
    FormatHtml = 1,
    FormatPdf = 2,

    FormatUnknown
  };
  Q_ENUM(DataFormat)
  /**
   * @brief Register CMarkDownQt 1.0 to Qml
   * @return return value of qmlRegisterSingletonType
   */
  static int registerQML();

  /**
   * @brief Transforms Commonmark to HTML
   * @param paramAs How to treat str parameter see TreatParam
   * @param strIn Either Commonmark formatted text or filename or..
   * @return Text in HTML format
   */
  //Q_INVOKABLE QString stringToHtml(TreatParam paramAs, const QString &strIn, OutputStyle outputStyle = StyleDefault);

  Q_INVOKABLE static QStringList availableConverters(DataFormat inFormat, DataFormat outFormat);

  Q_INVOKABLE static QString doConvert(QString strIn, QString strPlugin, DataFormat inFormat, DataFormat outFormat);

  Q_INVOKABLE static QString addFraming(QString strIn, QString strPlugin, DataFormat dataFormat);


signals:

public slots:

private:
  explicit CMarkDownQt(QObject *parent = nullptr);
  static QObject *getQMLInstance(QQmlEngine *t_engine, QJSEngine *t_scriptEngine);

  static PluginLoaderMdQt m_PluginLoader;
};

// plugin interface - for those not interested in QML-wrapper
class MARKDOWNQT_EXPORT PluginInterfaceMdQt
{
public:
    typedef struct _ConvertType{
        CMarkDownQt::DataFormat inFormat;
        CMarkDownQt::DataFormat outFormat;
        bool operator==(const _ConvertType& other) const {
            return inFormat == other.inFormat && outFormat == other.outFormat;
        }
    } ConvertType;

    virtual ~PluginInterfaceMdQt();
    virtual QList<ConvertType> availableConversions() = 0;
    virtual QString displayName() = 0;
    virtual bool convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut) = 0;
    virtual bool addFraming(CMarkDownQt::DataFormat dataFormat, const QByteArray strIn, QByteArray& strOut);
};

#define PluginInterfaceMdQt_iid "markdown.qt.PluginInterfaceMdQt"
Q_DECLARE_INTERFACE(PluginInterfaceMdQt, PluginInterfaceMdQt_iid)

// plugin loader
class MARKDOWNQT_EXPORT PluginLoaderMdQt
{
public:
    PluginLoaderMdQt();
    ~PluginLoaderMdQt();
    QStringList listAvailable(const PluginInterfaceMdQt::ConvertType convertType);
    PluginInterfaceMdQt *load(const QString strDisplayName);
    bool unload(const QString strDisplayName);
private:
    typedef struct _PluginInfo{
        QString strDisplayName;
        QList<PluginInterfaceMdQt::ConvertType> conversionTypes;
        QPluginLoader *pluginLoader;
    } PluginInfo;
    // key display string
    QMap<QString, PluginInfo> m_pluginInfoMap;
};





#endif // CMarkDownQt_H
