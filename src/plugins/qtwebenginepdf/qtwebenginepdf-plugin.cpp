#include "qtwebenginepdf-plugin.h"
#include <QtWebEngine>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QPageLayout>
#include <QPageSize>
#include <QApplication>
#include <QPrinter>
#include <QPrintDialog>

CQtWebEnginePdfPlugin::CQtWebEnginePdfPlugin()
{
    QApplication* app = qobject_cast<QApplication*>(QApplication::instance());
    if(!app) {
        qFatal("You application must have a QApplication object - QGuiApplication is not enough");
    }
}

CQtWebEnginePdfPlugin::~CQtWebEnginePdfPlugin()
{
}

QList<PluginBaseMdQt::ConvertType> CQtWebEnginePdfPlugin::availableConversions()
{
    QList<PluginBaseMdQt::ConvertType> supported;
    supported.append({CMarkDownQt::FormatHtmlUtf8, CMarkDownQt::FormatPdfBin});
    supported.append({CMarkDownQt::FormatHtmlUtf8, CMarkDownQt::FormatPrinterOut});
    return supported;
}

QString CQtWebEnginePdfPlugin::displayName()
{
    return QStringLiteral("qtwebenginepdf");
}

bool CQtWebEnginePdfPlugin::convert(ConvertType convertType, const QByteArray dataIn, QByteArray& dataOut)
{
    bool bSupported = false;
    if(convertType.inFormat==CMarkDownQt::FormatHtmlUtf8 &&
            (convertType.outFormat==CMarkDownQt::FormatPdfBin ||
            convertType.outFormat==CMarkDownQt::FormatPrinterOut)) {
        dataOut = convertToPdfOrPrinter(dataIn, convertType.outFormat);
    }
    return bSupported;
}

QByteArray CQtWebEnginePdfPlugin::convertToPdfOrPrinter(QByteArray strHtmlUtf8 , CMarkDownQt::DataFormat outFormat)
{
    QByteArray strPdf;
    if(strHtmlUtf8.size() > 0) {
        // yes - we need options for plugins
        QUrl baseUrl = QUrl();
        QMarginsF minMargins;
        minMargins.setLeft(5.0);
        minMargins.setRight(5.0);
        minMargins.setTop(15.0);
        minMargins.setBottom(15.0);
        QPageLayout pageLayout =
                QPageLayout(
                    QPageSize(QPageSize::A4),
                    QPageLayout::Portrait,
                    QMarginsF(),
                    QPageLayout::Millimeter,
                    minMargins);

        QWebEnginePage webPage;
        // load
        webPage.setHtml(strHtmlUtf8, baseUrl);
        bool bLoadOK = false;
        QEventLoop loop;
        connect(&webPage, &QWebEnginePage::loadFinished, [&](bool bOk) {
            bLoadOK = bOk;
            loop.quit();
        });
        loop.exec();

        if(bLoadOK) {
            // -> pdf
            if(outFormat == CMarkDownQt::FormatPdfBin) {
                webPage.printToPdf([&](const QByteArray &strPdfCreated) {
                        strPdf = strPdfCreated;
                        loop.quit();
                    },
                    pageLayout);
                loop.exec();
            }
            // -> printer
            else {
                QPrinter printer;
                printer.setPageLayout(pageLayout);
                QPrintDialog printDialog(&printer);
                if (printDialog.exec() == QDialog::Accepted) {
                    webPage.print(&printer, [&](bool bPrinted) {
                        if(!bPrinted) {
                            qWarning("Webpage print: page was not printed!");
                        }
                        loop.quit();
                    });

                loop.exec();

                }
            }
        }
        else {
            qWarning("Webpage print/export: page was not loaded!");
        }
    }
    return strPdf;
}
