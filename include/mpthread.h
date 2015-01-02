#ifndef MPTHREAD_H
#define MPTHREAD_H

#include <QThread>
#include <QProcess>
#include <QMap>
#include <QList>

//! Classe du thread qui compile les fichiers metapost
/*!
Cette classe héritant de \a QThread et réimplémentant naturellement la méthode \a run(), exécutée lors du lancement
du thread par start().
*/
class MpThread : public QThread
{
	Q_OBJECT
	
	signals:
	
	//! Signal émis lors d'une compilation réussie
	/*! 
	Ce signal transmet les métadonnées de l'exercice en question.
	*/
	void mpTreated();
	
	//! Signal émis si mpost a échoué (erreur de compilation ou absence d'u fichier .mp)
	/*! 
	Ce signal transmet l'indice dans newExosList de l'exo posant problème ainsi que le message d'erreur
	*/
	void errorOccured(const int &fileRef,const QString &error);
	
	public:
	
	//! Constructeur
	/*!
	Reçoit la liste des métadonnées des exercices, ainsi que la liste des indices de cette liste
	correspondant à des exercices ayant des figures metapost à compiler.
	*/
	MpThread(QList<QMap<QString,QString> > file, QList<int> mpFilesRef, QObject *parent);
		
	//! Méthode run() qui fait le travail en créant un \a QProcess, et en testant le succès de la compilation.
	void run();
		
		
	private:
	
	//! Liste locale des exercices à traiter
	QList<QMap<QString,QString> > fileList;
	
	
	//! Liste des numéros d'exos contenant du mp
	QList<int> mpFilesRef;
};
#endif
