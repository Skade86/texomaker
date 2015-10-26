#include <QtCore>
#include <QtWidgets>

#include "importthread.h"
#include "preferences.h"

ImportThread::ImportThread(QList<QMap<QString,QString> > list, QObject *parent) : QThread(parent)
{
	fileList = list;
}

void ImportThread::run()
{	
	QProcess compileProcess;
	QString processBin = QString();
	
	if ((Preferences::p_getCompiler()=="latex")||(Preferences::p_getCompiler()=="tex"))
	{
		processBin=Preferences::p_getLtx2pdf();
	}
	else processBin=Preferences::p_getBin(Preferences::p_getCompiler());
	
#ifndef Q_OS_WIN
	QStringList env = QProcess::systemEnvironment();
	int j = env.indexOf(QRegExp("^PATH=(.*)"));
	int limit = env.at(j).indexOf("=");
	QString value = env.at(j).right(env.at(j).size()-limit-1).trimmed();
	value = "PATH=" + value + ":" + QFileInfo(processBin).path() + ":" + QFileInfo(Preferences::p_getBin("latex")).path() + ":" + QFileInfo(Preferences::p_getBin("dvips")).path() + ":" + QFileInfo(Preferences::p_getBin("ps2pdf")).path();
        env.replace(j,value);
	compileProcess.setEnvironment(env);
#endif
	
	int listLen = fileList.size();
	
	QString exoFile;
	QMap<QString,QString> * exoMap = new QMap<QString,QString>;
	
	for (int i=0; i<listLen; i++)
		{	
			exoMap = new QMap<QString,QString>;
			*exoMap = fileList.at(i);
			exoFile = exoMap->value("filepath");
			compileProcess.setWorkingDirectory(QFileInfo(exoFile).path());
			QStringList args;
			QString tmpFileName = QFileInfo(exoFile).baseName() + "-preview.tex";
			
			if ((Preferences::p_getCompiler()=="latex")||(Preferences::p_getCompiler()=="tex"))
			{ 
#ifdef Q_OS_WIN
				args << QFileInfo(exoFile).baseName() + "-preview" << Preferences::p_getBin("latex") << Preferences::p_getBin("dvips") << Preferences::p_getBin("ps2pdf") << Preferences::p_getCompilationOptions();
#else
                args << "-c" << tmpFileName << Preferences::p_getBin("latex") << Preferences::p_getBin("dvips") << Preferences::p_getBin("ps2pdf") << Preferences::p_getCompilationOptions();
                //             args << "-c" << tmpFileName << "latex" << "dvips" << "ps2pdf" << Preferences::p_getCompilationOptions();
#endif
			}
			else args << Preferences::p_getCompilationOptions() << tmpFileName;

			// On exécute la compilation
			compileProcess.start(processBin,args);
			compileProcess.waitForFinished(-1);
			QString errorOutput = compileProcess.readAll();
			QString outName=QString();
			outName = QFileInfo(exoFile).path() + QDir::separator()+ QFileInfo(exoFile).baseName()+"-preview.pdf";
			
			QFile outFile;
			if (outFile.exists(outName)) {	emit exoTreated(exoMap);		} 
				else emit errorOccured(exoFile,errorOutput);
			exoMap->~QMap();
		}
}
