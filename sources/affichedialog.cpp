#include <QtWidgets>
#include "affichedialog.h"
#include <poppler-qt5.h>
#include "documentwidget.h"

AfficheDialog::AfficheDialog() 
    : QWidget()
{
    setupUi(this);
    scaleFactors << 0.25 << 0.5 << 0.75 << 1. << 1.25 << 1.5 << 2. << 3. << 4.;
    documentWidget = new DocumentWidget();
    scrollArea->setWidget(documentWidget);

    connect(pageSpinBox, SIGNAL(valueChanged(int)),
            documentWidget, SLOT(setPage(int)));
    connect(documentWidget, SIGNAL(pageChanged(int)),
            pageSpinBox, SLOT(setValue(int)));
    connect(scaleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(scaleDocument(int)));
}

void AfficheDialog::loadImageFile(QString exoFile)
{
    if (exoFile.isEmpty()) {
        documentWidget->setDocument(QString());
        return;
    }

    QString pdfFile;
    if (QFileInfo(exoFile).suffix() == "pdf")  pdfFile = exoFile;
    else {
        pdfFile = QFileInfo(exoFile).path() + QDir::separator()
                  + QFileInfo(exoFile).baseName()+"-preview.pdf";
    }

    if (QFileInfo(pdfFile).exists()) documentWidget->setDocument(pdfFile);
    else {
        documentWidget->setDocument(QString());
        return;
    }

    pageSpinBox->setEnabled(true);
    pageSpinBox->setMinimum(1);
    pageSpinBox->setMaximum(documentWidget->document()->numPages());
    pageSpinBox->setValue(1);
    scaleComboBox->setEnabled(true);
    documentWidget->setScale(scaleFactors[scaleComboBox->currentIndex()]);
}

void AfficheDialog::scaleDocument(int index)
{
    documentWidget->setScale(scaleFactors[index]);
}

