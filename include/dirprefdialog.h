#ifndef DIRPREFDIALOG_H
#define DIRPREFDIALOG_H

#include <QDialog>
#include "ui_dirprefdialog.h"

class XmlDomHandler;

//! Classe de la fenêtre des préférences
/*! 
        C'est dans cette fenêtre de dialogue que l'utilisateur peut modifier à sa guise le préambule LateX qui
	sera utilisé d'une part dans la feuille d'exercice finale, mais également pour créer les aperçus des exercices.
*/
class DirPrefDialog : public QDialog, public Ui::DirPrefDialog
{
Q_OBJECT
	
public:

//! Constructeur
/*! 
	Prend comme arguments le Widget parent, ainsi qu'un pointeur sur le prembule Latex courant
*/
    DirPrefDialog( QWidget * parent = 0,XmlDomHandler * domHandler=0,int rows=0, bool creation = false);
	
//! Liste de pointeurs sur des QRadioButton pour la sélection des métadonnées
	QList<QRadioButton *> metaRadio;	
	
//! Liste de pointeurs sur des QLineEdit pour le choix des metadonnées
	QList<QLineEdit *> metaLine;

//! Liste de pointeurs sur des QCheckBox pour le choix des métadonnées à afficher
	QList<QCheckBox *> metaCheck;

//! Liste de pointeurs sur des QSpinBox pour le choix de l'ordre des métadonnées à afficher
	QList<QSpinBox *> metaSpin;
	
signals:
	
	void aboutToClose();
	
private slots:

//! Acceptation des (éventuelles) modifications
	void on_okButton_clicked();
	void on_addButton_clicked();
	void on_removeButton_clicked();
    void on_addMacroFileButton_clicked();
    void on_delMacroFileButton_clicked();
	void updateMax(bool checked);
	
private:

    XmlDomHandler * domHandler;
	
	void setMetaTab();
	
	bool updateMetaDatas();

    void updateMetaSpin(int toRemove);
	
	int rows;
	
	bool creation;
	
	int metaSize;
	
	int viewChecked;

    int metaAdded;
};
#endif
