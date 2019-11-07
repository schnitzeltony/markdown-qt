#ifndef CMarkDownQt_H
#define CMarkDownQt_H

#include "markdownqt_export.h"
#include <QtPlugin>
#include <QList>
#include <QMap>

// TODO PIMPL...

QT_BEGIN_NAMESPACE
class QQmlEngine;
class QJSEngine;
QT_END_NAMESPACE

// plugin interface - for those not interested in QML-wrapper
class MARKDOWNQT_EXPORT PluginInterfaceMdQt
{
public:
    enum DataFormat {
      FormatAll = 0,
      FormatMd = 1,
      FormatHtml = 2,
      FormatPdf = 3,
    };
    typedef struct {
        DataFormat inFormat;
        DataFormat outFormat;
    } ConvertType;

    virtual ~PluginInterfaceMdQt() {}
    virtual QList<ConvertType> availableConversions() = 0;
    virtual QString displayName() = 0;
    virtual bool convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut) = 0;
};

#define PluginInterfaceMdQt_iid "markdown.qt.PluginInterfaceMdQt"
Q_DECLARE_INTERFACE(PluginInterfaceMdQt, PluginInterfaceMdQt_iid)

// plugin loader
class MARKDOWNQT_EXPORT PluginLoaderMdQt
{
public:
    PluginLoaderMdQt();
private:
    typedef struct {
          QString strLibName;
          QString strDisplayName;
          QList<PluginInterfaceMdQt::ConvertType> conversionTypes;
    } PluginInfo;
    // key display string
    QMap<QString, PluginInfo> m_pluginInfoMap;
};

// our plugin wrapper for use by QML
class MARKDOWNQT_EXPORT CMarkDownQt : public QObject
{
  Q_OBJECT
public:

  enum TreatParam { // TODO remove
    AsString = 0,
    AsFilename = 1,
  };
  Q_ENUM(TreatParam)

  enum ConvertType { // TODO remove
    ConvertCmark = 0,
    ConvertSundown = 1,
  };
  Q_ENUM(ConvertType)

  enum OutputStyle {
    StyleDefault = 0,
    StyleGithub = 1,
  };
  Q_ENUM(OutputStyle)

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
  Q_INVOKABLE static QString stringToHtml(TreatParam paramAs, const QString &strIn, ConvertType convertType = ConvertCmark, OutputStyle outputStyle = StyleDefault);

signals:

public slots:

private:
  explicit CMarkDownQt(QObject *parent = nullptr);
  static QObject *getQMLInstance(QQmlEngine *t_engine, QJSEngine *t_scriptEngine);

  PluginLoaderMdQt m_PluginLoader;
};




#endif // CMarkDownQt_H
