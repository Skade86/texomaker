#include <QtCore>
#include <QtWidgets>

#include "mpthread.h"
#include "preferences.h"

MpThread::MpThread(QList<QMap<QString,QString> > file, QList<int> mpFilesRef,QObject *parent) : QThread(parent), mpFilesRef(mpFilesRef)
{
	fileList=file;
}

void MpThread::run()
{	
	QProcess mpostProcess;
	
	QString mpostBin=Preferences::p_getBin("mpost");
	
	// On ajoute à l'environnement du processus le répertoire contenant tous les binaires liés à LaTeX et metapost (en particulier
	// makempx, nécessaire à mpost.
	int j;
	
#ifndef Q_WS_WIN
	QStringList env = QProcess::systemEnvironment();
	j = env.indexOf(QRegExp("^PATH=(.*)"));
	int limit = env.at(j).indexOf("=");
	QString value = env.at(j).right(env.at(j).size()-limit-1).trimmed();
	value = "PATH=" + value + ":" + QFileInfo(mpostBin).path();
	env.replace(j,value);
	mpostProcess.setEnvironment(env);
#endif
	
	
	// Nombre d'exos de la liste fileList qui contiennent du metapost
	int listLen = mpFilesRef.size();
	QMap<QString,QString> exoMap;
	QString exoFile;
	QStringList mpFigFileList;
	
	// On boucle sur tous les exos qi contiennent du mp
	for (int i=0; i<listLen; i++)
		{	
			j = mpFilesRef.at(i);	// j est l'indice dans fileList d'un exo contenant du mp
			exoMap = fileList.at(j);	// Liste des métadonnées de l'exo en question
			exoFile = exoMap.value("filepath");
			QStringList mpFigFileList = exoMap.values("metapost");	// Liste des fichiers mp à compiler
			QString mpFile=QString();
			
			// On boucle sur cette liste
			for (int k=0;k<mpFigFileList.size();k++)
			{
				mpFile = mpFigFileList.at(k);
				QString completeMpFile = QFileInfo(exoFile).path() + QDir::separator()+ mpFile;
				QString errorOutput;
				// On vérifie que le fichier métapost requis existe
				if (QFileInfo(completeMpFile).exists())
				{
				mpostProcess.setWorkingDirectory(QFileInfo(exoFile).path());
				QStringList args;
				args << "-halt-on-error" << mpFile;
	
				// On exécute mpost
				mpostProcess.start(mpostBin,args);
				mpostProcess.waitForFinished(-1);
				errorOutput = mpostProcess.readAll();
				}
				else errorOutput = QString(tr("The required metapost file %1 doesn't exists !").arg(mpFile));
				
				QString compiledFile = QFileInfo(exoFile).path() + QDir::separator()+ QFileInfo(mpFile).baseName()+".1";
				QFile outFile;
				if (!outFile.exists(compiledFile)) {	 emit errorOccured(j,errorOutput);	} 
			}
		}
	emit mpTreated();	
}
