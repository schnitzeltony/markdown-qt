#ifndef CMarkDownQt_H
#define CMarkDownQt_H

#include "markdownqt_export.h"
#include <QtPlugin>
#include <QList>

QT_BEGIN_NAMESPACE
class QQmlEngine;
class QJSEngine;
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
  };
  Q_ENUM(DataFormat)

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
};

// plugin interface - for those not interested in QML-wrapper
class MARKDOWNQT_EXPORT MdQtPluginInterface
{
public:
    typedef struct {
        CMarkDownQt::DataFormat inFormat;
        CMarkDownQt::DataFormat outFormat;
    } ConvertType;
    virtual ~MdQtPluginInterface() {}
    virtual QList<ConvertType> availableConversion() = 0;
    virtual QString displayName() = 0;
    virtual bool convert(ConvertType convertType, const QByteArray strIn, QByteArray& strOut) = 0;
};

#define MdQtPluginInterface_iid "markdown.qt.MdQtPluginInterface"
Q_DECLARE_INTERFACE(MdQtPluginInterface, MdQtPluginInterface_iid)



#endif // CMarkDownQt_H
