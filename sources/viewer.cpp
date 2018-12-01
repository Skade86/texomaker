#include "viewer.h"
#include "preferences.h"
#include "ui_viewer.h"
#include <QFileInfo>
#include <QDir>

Viewer::Viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Viewer)
{
    // Initialisations diverses
    ui->setupUi(this);
    webView = new QWebEngineView(this);
    ui->layout->addWidget(webView);
}

void Viewer::loadImageFile(QString exoFile)
{
    if (exoFile.isEmpty()) {
        return;
    }

    QString pdfFile;
    if (QFileInfo(exoFile).suffix() == "pdf")  pdfFile = exoFile;
    else {
        pdfFile = QFileInfo(exoFile).path() + QDir::separator()
                  + QFileInfo(exoFile).baseName()+"-preview.pdf";
    }

    if (QFileInfo(pdfFile).exists()) {
        QString pdfFileURL = Preferences::p_getPdfJSDir() + QString("?file=")+pdfFile;
        webView->load(QUrl::fromUserInput(pdfFileURL));
    }
    else {
        webView->setHtml("");
    }
}

Viewer::~Viewer()
{
    delete ui;
}
