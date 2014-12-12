#ifndef IMPORTTHREAD_H
#define IMPORTTHREAD_H

#include <QThread>
#include <QProcess>
#include <QMap>
#include <QList>

//! Classe du thread qui compile les fichiers tex
/*!
Cette classe héritant de \a QThread et réimplémentant naturellement la méthode \a run(), exécutée lors du lancement
du thread par start().
*/
class ImportThread : public QThread
{
	Q_OBJECT
	
	signals:
	
	//! Signal émis lors d'une création réussi de *-preview.pdf par pdflatex
	/*! 
	Ce signal transmet les métadonnées de l'exercice en question.
	*/
	void exoTreated(const QMap<QString,QString> * fileMap);
	
	//! Signal émis si pdflatex a échoué, c-à-d n'a pas donné de fichier pdf
	/*! 
	Ce signal transmet les métadonnées de l'exercice en question ainsi que la sortie standard de pdflatex
	*/
	void errorOccured(const QString &file,const QString &error);
	
	public:
	
	//! Constructeur qui reçoit la liste des noms des exercices à compiler.
	ImportThread(QList<QMap<QString,QString> > list, QObject *parent);
		
	//! Méthode run() qui fait le travail en créant un \a QProcess, et en testant le succès de la compilation.
	void run();
		
		
	private:
	
	//! Liste locale des exercices à traiter
	QList<QMap<QString,QString> > fileList;
	
};
#endif
