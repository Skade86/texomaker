#include <QtWidgets>
#include <poppler-qt5.h>
#include "documentwidget.h"

DocumentWidget::DocumentWidget(QWidget *parent)
    : QLabel(parent)
{
    currentPage = -1;
    doc = 0;
    rubberBand = 0;
    scaleFactor = 1.0;
    setAlignment(Qt::AlignCenter);
}

DocumentWidget::~DocumentWidget()
{
    delete doc;
}

Poppler::Document *DocumentWidget::document()
{
    return doc;
}

QMatrix DocumentWidget::matrix() const
{
    return QMatrix(scaleFactor * physicalDpiX() / 72.0, 0,
                   0, scaleFactor * physicalDpiY() / 72.0,
                   0, 0);
}

void DocumentWidget::mousePressEvent(QMouseEvent *event)
{
    if (!doc)
        return;

    dragPosition = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(dragPosition, QSize()));
    rubberBand->show();
}

void DocumentWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!doc)
        return;

    rubberBand->setGeometry(QRect(dragPosition, event->pos()).normalized());
}

void DocumentWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (!doc)
        return;

    if (!rubberBand->size().isEmpty()) {
        // Correct for the margin around the image in the label.
        QRectF rect = QRectF(rubberBand->pos(), rubberBand->size());
        rect.moveLeft(rect.left() - (width() - pixmap()->width()) / 2.0);
        rect.moveTop(rect.top() - (height() - pixmap()->height()) / 2.0);
  //      setPage();
    }

    rubberBand->hide();
}

qreal DocumentWidget::scale() const
{
    return scaleFactor;
}

void DocumentWidget::showPage(int page)
{
    if (page != -1 && page != currentPage + 1) {
        currentPage = page - 1;
        emit pageChanged(page);
    }

    QImage image = doc->page(currentPage)
                      ->renderToImage(scaleFactor * physicalDpiX(), scaleFactor * physicalDpiY());

    setPixmap(QPixmap::fromImage(image));
}


bool DocumentWidget::setDocument(const QString &filePath)
{
    if (filePath.isEmpty()) {
        setPixmap(QPixmap::fromImage(QImage()));
        return false;
    }

    Poppler::Document *oldDocument = doc;

    doc = Poppler::Document::load(filePath);
    if (doc) {
        delete oldDocument;
        doc->setRenderHint(Poppler::Document::Antialiasing);
        doc->setRenderHint(Poppler::Document::TextAntialiasing);
//        searchLocation = QRectF();
        currentPage = -1;
        setPage(1);
    }
    return doc != 0;
}

void DocumentWidget::setPage(int page)
{
    if (page != currentPage + 1) {
        showPage(page);
    }
}

void DocumentWidget::setScale(qreal scale)
{
    if (scaleFactor != scale) {
        scaleFactor = scale;
        showPage();
    }
}
