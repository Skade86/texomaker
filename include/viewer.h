#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>

#include <QDebug>
#include "ui_viewer.h"
#include "poppler-qt5.h"

class QGraphicsPixmapItem;


//! Classe héritant de \a QWidget et de \a Ui::Viewer (ui créée dans Designer)
/*!
Cette classe gère l'affichage des pdf de preview des exercices. La méthode publique \a  loadImageFile(QString exoFile)
est appelée par la fenêtre principale à chaque fois que l'utilisateur sélectionne un nouvelle exercice dans la liste.
La classe utilise la librairie poppler-qt5 pour gérer les pdf.
*/

namespace Ui {
class Viewer;
}

class Viewer : public QWidget {
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();

    //! Méthode affiche sur le QLabel du Widget, le preview de l'exercice passé en argument
    /*!
    */
    void loadImageFile(QString exoFile);
    
private:
    Ui::Viewer *ui;
    QImage image;
    qreal scaleFact;
    Poppler::Document* document;
    Poppler::Page* pdfPage;
    QGraphicsPixmapItem* myItem;
    QGraphicsScene* scene;
    int currentPage;
    int numPages;

    // Gestion de l'affichage des boutons de pages
    // suivant le nombre de pages du document
    void updatePagesButtons();

private slots:

    void onButtonInClicked();
    void onButtonOutClicked();
    void onButtonFitClicked();
    void nextPageClicked();
    void previousPageClicked();

protected:
    virtual void resizeEvent(QResizeEvent * event);
};

#endif // VIEWER_H
