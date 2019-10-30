#ifndef CMarkDownQt_H
#define CMarkDownQt_H

#include <QObject>
#include "markdownqt_export.h"

QT_BEGIN_NAMESPACE
class QQmlEngine;
class QJSEngine;
QT_END_NAMESPACE

class MARKDOWNQT_EXPORT CMarkDownQt : public QObject
{
  Q_OBJECT
public:
  enum TreatParam {
    AsString = 0,
    AsFilename = 1,
  };
  Q_ENUM(TreatParam)

  enum ConvertType {
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

#endif // CMarkDownQt_H
