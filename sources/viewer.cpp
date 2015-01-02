#include "viewer.h"
#include "ui_viewer.h"
#include "poppler-qt5.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFileInfo>
#include <QDir>

Viewer::Viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Viewer)
{
    // Initialisations diverses
    ui->setupUi(this);
    numPages=1;
    currentPage=0;
    scaleFact=1.0;
    updatePagesButtons();

    // Connection des boutons Zoom/Fit
    connect(ui->inButton,SIGNAL(clicked()),this,SLOT(onButtonInClicked()));
    connect(ui->outButton,SIGNAL(clicked()),this,SLOT(onButtonOutClicked()));
    connect(ui->fitButton,SIGNAL(clicked()),this,SLOT(onButtonFitClicked()));
    connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(nextPageClicked()));
    connect(ui->previousButton,SIGNAL(clicked()),this,SLOT(previousPageClicked()));

    // Création de la QGraphicsView avec une image vide
    image = QImage();
    scene = new QGraphicsScene(this);
    ui->pdfView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->pdfView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->pdfView->setScene(scene);
    ui->pdfView->grabGesture(Qt::PanGesture);
    ui->pdfView->grabGesture(Qt::PinchGesture);
    myItem= new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(myItem);

    ui->pdfView->show();
}

void Viewer::loadImageFile(QString exoFile)
{
    if (exoFile.isEmpty()) {
        image=QImage();
        myItem->setPixmap(QPixmap::fromImage(image));
        return;
    }

    QString pdfFile;
    if (QFileInfo(exoFile).suffix() == "pdf")  pdfFile = exoFile;
    else {
        pdfFile = QFileInfo(exoFile).path() + QDir::separator()
                  + QFileInfo(exoFile).baseName()+"-preview.pdf";
    }

    if (QFileInfo(pdfFile).exists()) {
        document = Poppler::Document::load(pdfFile);
        document->setRenderHint(Poppler::Document::Antialiasing,true);
        document->setRenderHint(Poppler::Document::TextAntialiasing,true);
        numPages=document->numPages();
        currentPage=0;
        updatePagesButtons();
        pdfPage = document->page(currentPage);
        image = pdfPage->renderToImage(scaleFact*physicalDpiX(),scaleFact*physicalDpiY());
        myItem->setPixmap(QPixmap::fromImage(image));
    }
    else {
        image=QImage();
        myItem->setPixmap(QPixmap::fromImage(image));
        return;
    }
}

void Viewer::nextPageClicked() {
    if (currentPage==numPages-1) return;
    currentPage++;
    pdfPage = document->page(currentPage);
    image = pdfPage->renderToImage(scaleFact*physicalDpiX(),scaleFact*physicalDpiY());
    myItem->setPixmap(QPixmap::fromImage(image));
}

void Viewer::previousPageClicked() {
    if (currentPage==0) return;
    currentPage--;
    pdfPage = document->page(currentPage);
    image = pdfPage->renderToImage(scaleFact*physicalDpiX(),scaleFact*physicalDpiY());
    myItem->setPixmap(QPixmap::fromImage(image));
}

void Viewer::onButtonInClicked() {
    if (image.width()==0) return;
    scaleFact+=0.1;
    image = pdfPage->renderToImage(scaleFact*physicalDpiX(),scaleFact*physicalDpiY());
    myItem->setPixmap(QPixmap::fromImage(image));
}

void Viewer::onButtonOutClicked() {
    if (image.width()==0) return;
    scaleFact-=0.1;
    image = pdfPage->renderToImage(scaleFact*physicalDpiX(),scaleFact*physicalDpiY());
    myItem->setPixmap(QPixmap::fromImage(image));
}

void Viewer::onButtonFitClicked() {
    if (image.width()==0) return;
    scaleFact=scaleFact*(this->width())/(image.width());
    image = pdfPage->renderToImage(scaleFact*physicalDpiX(),scaleFact*physicalDpiY());
    myItem->setPixmap(QPixmap::fromImage(image));
}

void Viewer::resizeEvent(QResizeEvent *event) {
    onButtonFitClicked();
}

void Viewer::updatePagesButtons() {
    if (numPages==1) {
        ui->nextButton->setVisible(false);
        ui->previousButton->setVisible(false);
    }
    else {
        ui->nextButton->setVisible(true);
        ui->previousButton->setVisible(true);
    }
}

Viewer::~Viewer()
{
    delete ui;
}
