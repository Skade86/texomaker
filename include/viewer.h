#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QtWebEngineWidgets/QWebEngineView>
#include "ui_viewer.h"

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
    explicit Viewer(QWidget *parent = nullptr);
    ~Viewer();

    //! Méthode affiche sur le QLabel du Widget, le preview de l'exercice passé en argument
    /*!
    */
    void loadImageFile(QString exoFile);
    
private:
    Ui::Viewer *ui;
    QWebEngineView *webView;
};

#endif // VIEWER_H
