#ifndef EXOSMODEL_H
#define EXOSMODEL_H

#include <QtWidgets>
#include <QAbstractTableModel>

//! Classe de modèle personnalisée, héritant de \a QAbstractTableModel
/*!
	La classe \a QAbstractTableModel est une classe abstraite. On réimplémente ici des fonctions de cette classe.
	La plupart de ces méthodes ne seront pas appelées directement par nous, mais le seront via des méthodes
	de plus haut niveau (comme setModel() ...).
*/

class ExosModel : public QAbstractTableModel
{
	
Q_OBJECT

public:

//! Constructeur
/*! 
	Prend comme arguments le Widget parent, ainsi qu'un QStringList contenant l'ensemble des champs de tous les
	exercices à la suite.
*/
	ExosModel(const QList<QMap<QString,QString> > & exos, QObject * parent);
	
	//! Méthode réimplémentée renvoyant le nombre de lignes
	int rowCount(const QModelIndex& parent) const;
	
	//! Méthode réimplémentée renvoyant le nombre de colonnes
	int columnCount(const QModelIndex& parent) const;
	
	//! Méthode réimplémentée gérant le header des données (Titre - ...)
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	
	//! Méthode réimplémentée gérant les données du modèle
	QVariant data(const QModelIndex &index, int role) const;
	
	//! Méthode réimplémentée gérant l'écriture de nouvelles données
	bool setData(const QModelIndex &index,  const QVariant &value, int role);
	
	//! Méthode réimplémentée gérant l'ajout de nouvelles lignes
	bool insertRows(int row, int count, const QModelIndex &parent);
	
	//! Méthode réimplémentée la suppression de lignes
	bool removeRows(int row, int count, const QModelIndex &parent);
	
	//! Méthode personnelle qui met à jour le modèle lors de l'insertion d'un nouvel exercice
	void updateModel(QMap<QString,QString> newRowMap, bool update);
	
private:

	//! C'est une liste QStringList.
	/*!
	Cette liste contient des QStringList, autant que d'exercices dans la base; chaque QStringList
	contient les données d'un exercice.
	*/
	QList<QStringList> exosList;
	
};
#endif
