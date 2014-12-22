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
  //      searchLocation = rect;
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

    if (!searchLocation.isEmpty()) {
        QRect highlightRect = matrix().mapRect(searchLocation).toRect();
        highlightRect.adjust(-2, -2, 2, 2);
        QImage highlight = image.copy(highlightRect);
        QPainter painter;
        painter.begin(&image);
        painter.fillRect(image.rect(), QColor(0, 0, 0, 32));
        painter.drawImage(highlightRect, highlight);
        painter.end();
    }

    setPixmap(QPixmap::fromImage(image));
}

QRectF DocumentWidget::searchBackwards(const QString &text)
{
    QRectF oldLocation = searchLocation;

    int page = currentPage;
    if (oldLocation.isNull())
        page -= 1;

    while (page > -1) {

        QList<QRectF> locations;
        searchLocation = QRectF();

//        while (doc->page(page)->search(text, searchLocation,
//            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

//            if (searchLocation != oldLocation)
//                locations.append(searchLocation);
//            else
//                break;
//        }

        int index = locations.indexOf(oldLocation);
        if (index == -1 && !locations.isEmpty()) {
            searchLocation = locations.last();
            showPage(page + 1);
            return searchLocation;
        } else if (index > 0) {
            searchLocation = locations[index - 1];
            showPage(page + 1);
            return searchLocation;
        }

        oldLocation = QRectF();
        page -= 1;
    }

    if (currentPage == doc->numPages() - 1)
        return QRectF();

    oldLocation = QRectF();
    page = doc->numPages() - 1;

    while (page > currentPage) {

        QList<QRectF> locations;
        searchLocation = QRectF();

//        while (doc->page(page)->search(text, searchLocation,
//            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

//            locations.append(searchLocation);
//        }

        if (!locations.isEmpty()) {
            searchLocation = locations.last();
            showPage(page + 1);
            return searchLocation;
        }
        page -= 1;
    }

    return QRectF();
}

QRectF DocumentWidget::searchForwards(const QString &text)
{
    int page = currentPage;
    while (page < doc->numPages()) {

//        if (doc->page(page)->search(text, searchLocation,
//            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {
//            if (!searchLocation.isNull()) {
//                showPage(page + 1);
//                return searchLocation;
//            }
//        }
        page += 1;
        searchLocation = QRectF();
    }

    page = 0;

    while (page < currentPage) {

        searchLocation = QRectF();

//        if (doc->page(page)->search(text, searchLocation,
//            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {
//            if (!searchLocation.isNull()) {
//                showPage(page + 1);
//                return searchLocation;
//            }
//        }
        page += 1;
    }

    return QRectF();
}

void DocumentWidget::selectedText(const QRectF &rect)
{
    QRectF selectedRect = matrix().inverted().mapRect(rect);
    // QString text = doc->page(currentPage)->text(selectedRect);

    QString text;
    bool hadSpace = false;
    QPointF center;
    foreach (Poppler::TextBox *box, doc->page(currentPage)->textList()) {
        if (selectedRect.intersects(box->boundingBox())) {
            if (hadSpace)
                text += " ";
            if (!text.isEmpty() && box->boundingBox().top() > center.y())
                text += "\n";
            text += box->text();
            hadSpace = box->hasSpaceAfter();
            center = box->boundingBox().center();
        }
    }

    if (!text.isEmpty())
        emit textSelected(text);
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
        searchLocation = QRectF();
        currentPage = -1;
        setPage(1);
    }
    return doc != 0;
}

void DocumentWidget::setPage(int page)
{
    if (page != currentPage + 1) {
        searchLocation = QRectF();
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
