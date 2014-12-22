#ifndef AFFICHEDIALOG_H
#define AFFICHEDIALOG_H

#include <QDialog>
#include "ui_affichedialog.h"

class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
class DocumentWidget;


//! Classe héritant de \a QWidget et de \a Ui::AfficheDialog (ui créée dans Designer)
/*!
Cette classe gère l'affichage des pdf de preview des exercices. La méthode publique \a  loadImageFile(QString exoFile)
est appelée par la fenêtre principale à chaque fois que l'utilisateur sélectionne un nouvelle exercice dans la liste.
La classe utilise la librairie poppler-qt4 pour gérer les pdf. 
*/
class AfficheDialog : public QWidget, public Ui::AfficheDialog
{
    Q_OBJECT

public:
    //! Constructeur qui crée l'UI
    AfficheDialog();

    //! Méthode affiche sur le QLabel du Widget, le preview de l'exercice passé en argument
    /*!
*/
    void loadImageFile(QString exoFile);
    
private:
    DocumentWidget * documentWidget;
    QVector<qreal> scaleFactors;


private slots:
    void scaleDocument(int index);
};

#endif
