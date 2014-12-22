#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include <QLabel>
#include <QRectF>
#include <poppler-qt5.h>

class QRubberBand;

class DocumentWidget : public QLabel
{
    Q_OBJECT

public:
    DocumentWidget(QWidget *parent = 0);
    ~DocumentWidget();
    Poppler::Document *document();
    QMatrix matrix() const;
    qreal scale() const;

public slots:
    QRectF searchBackwards(const QString &text);
    QRectF searchForwards(const QString &text);
    bool setDocument(const QString &filePath);
    void setPage(int page = -1);
    void setScale(qreal scale);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void pageChanged(int currentPage);
    void textSelected(const QString &text);

private:
    void selectedText(const QRectF &rect);
    void showPage(int page = -1);

    Poppler::Document *doc;
    int currentPage;
    QPoint dragPosition;
    QRubberBand *rubberBand;
    QRectF searchLocation;
    qreal scaleFactor;
};

#endif
