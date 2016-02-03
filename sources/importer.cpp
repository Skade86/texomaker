#include <QtWidgets>
#include <QProcess>
#include <QDebug>

#include "importer.h"
#include "xmldomhandler.h"
#include "pdflatexlogdialog.h"
#include "importthread.h"
#include "mpthread.h"
#include "exosmodel.h"
#include "preferences.h"
#include "viewer.h"

Importer::Importer( QStringList texFiles,bool iso,QDomDocument *doc,ExosModel *model,Viewer *affiche,bool update) : QWidget()
{
	fileList = texFiles;
	useIso = iso;
	localDoc = doc;
	domHandler = new XmlDomHandler(localDoc);
	localModel = model;
	fileUpdate = update;
	localAffiche = affiche;
	QString processBin=Preferences::p_getLtx2pdf();
}

void Importer::importFiles()
{
	
	QStringList::Iterator it = fileList.begin();	// Itérateur sur la liste des fichiers d'exos
	// On boucle sur tous les exos
     while(it != fileList.end())
     	{
     	QString fileBaseName = QFileInfo(*it).baseName() + ".tex";
     	
		int testExist = domHandler->searchExoPath(*it);
     	if ((testExist==0) ||  (fileUpdate == true)) // L'importation ne se fait que si l'exo n'existe pas déjà ou qu'il s'agit d'un update
		{
			
		// Si l'aperçu existe déjà on le renomme avec "-old"
		QString pdfFileName = QFileInfo(*it).path() + QDir::separator() + QFileInfo(*it).baseName() + "-preview.pdf";
		QString pdfFileNameOld = QFileInfo(*it).path() + QDir::separator() + QFileInfo(*it).baseName() + "-preview-old.pdf";
		if (QFileInfo(pdfFileName).exists()) QFile(pdfFileName).rename(pdfFileNameOld);
			
		// Quelques initialisations
        fileContent.clear();
		containsMpFiles.clear();
		metapostFigs=false;
     	
		exoMetaDatas = readFileContent(*it);	//  On récupére un tableau associatif contenant le texte  et les métadonnées de l'utilisateur
		fileContent = exoMetaDatas.value("exoText");		// QString contenant le contenu du fichier
		exoMetaDatas.remove("exoText");
		//******* On ajoute quelques métadonnées à exoMetaDatas
		
		// Ajout du chemin du fichier
		exoMetaDatas.insert("filepath",*it);
			
		// Ajout de la date et heure
		QString curDate = date.currentDateTime().toString("dd-MM-yyyy hh:mm");
		exoMetaDatas.insert("importdate",curDate);
		
		newExosList.append(exoMetaDatas);
			
		if (!exoMetaDatas.value("metapost").isEmpty()) 
		{
			metapostFigs=true;
			containsMpFiles.append(newExosList.size()-1);
		}
			
		//****************** Fin metadonnées

		// Création du fichier .tex à compiler pour créer le preview
		createLatexFile(fileContent,*it);
		
		exoDatas.clear();
		exoMetaDatas.clear();
	}
		else { QMessageBox::warning(this,tr("Warning"),tr("Exercise %1 exists already in the database").arg(fileBaseName)); }
		++it;
     }
	
	if (metapostFigs) {	launchMpthread();	}	// Si il y a des figures mp à compiler, on lance un thread qui va le faire
		else launchImportthread();				// Sinon on lance directement le thread d'importation
}


QMap<QString,QString> Importer::readFileContent(QString exoFile)
{
	int limit;
	
	QFile file(exoFile);
	QMap<QString,QString> metaList;
	QStringList metaTagList=Preferences::p_getMetaList(); // Liste des types de metadonnées
	
	QString realContent = QString();		// Chaîne contenant le texte de l'exo lui-même sans les "%@"
	
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,tr("Application"),
                            tr("Cannot read file %1:\n%2.")
                             .arg(exoFile)
                             .arg(file.errorString()));
        return metaList;
    }
	// Lecture de ce qu'il y a dans le fichier courant
    QTextStream in(&file); // Création du flux
    in.flush();
    if (useIso) in.setCodec("ISO 8859-15");

    // On récupère le texte de l'exo, en s'occupant des lignes commençant par "%@"
    while (!in.atEnd())
    	{
    		QString line = in.readLine();
			
    		if (line.trimmed().startsWith("%@")) // Si la ligne commence par %@
    		 {	
    			line = line.trimmed();		// On enlève les espaces aux 2 bouts
    			line.remove(0,2);			// On enlève les 2 premiers caractères (% et @)
    			line = line.trimmed();		// On enlève les espaces aux 2 bouts
    			limit = line.indexOf(":");	// On détermine l'endroit où apparaît le caractère ":"
				if (limit==-1)				// Si il n'y est pas, ce n'est pas une ligne de métadonnées
				{	
					realContent = realContent + line +"\n";	// Donc on ajoute la ligne dans le texte de l'exo
				}	
				else
				 {
					 QString key = line.left(limit).trimmed();	// La gauche de la limite est le type de métadonnée
					 if (!metaTagList.contains(key))	// Si ce type n'est pas dans la liste existante
					 {
						realContent = realContent + line +"\n";	// On compte la ligne comme du texte d'exo
					 }
					 else
					 {
						 QString value = line.right(line.size()-limit-1).trimmed();	// La droite de la limite est la valeur de la métadonnée
						 metaList.insertMulti(key,value);	// On l'ajoute dans le tableau associatif
					 }
				 }
			 }
				else realContent = realContent + line +"\n";
    	}
	
	
	// Petite boucle pour remplir (de vide) le tableau au cas où des métadonnées n'auraient pas été trouvées dans l'exo 
	for (int i=0;i<metaTagList.size();i++)
	{
		if (!metaList.contains(metaTagList.at(i))) metaList.insert(metaTagList.at(i),QString());
	}
	
	// On insère enfin le texte de l'exercice dans le tableau
	metaList.insert("exoText",realContent);
    return metaList;
}

void Importer::createLatexFile(QString content, QString currentFile)
{
	QString tmpFileName = QFileInfo(currentFile).path() + QDir::separator() + QFileInfo(currentFile).baseName() + "-preview.tex";
	QFile tmpFile(tmpFileName);
    if (!tmpFile.open(QIODevice::WriteOnly | QIODevice::Text))
    return;
    
    QString exoText = Preferences::p_getPreamble() + "\n";
	if (Preferences::p_getCompiler()=="tex") exoText += "\n"+content+"\n";
	else exoText += "\n"+Preferences::p_getBeginDoc()+"\n" + content + "\n"+Preferences::p_getEndDoc()+"\n";
    QTextStream out(&tmpFile);
    out.flush();
   	if (useIso) out.setCodec("ISO 8859-15");
 	out << exoText;
}


void Importer::launchMpthread()
{
	MpThread *mpThread = new MpThread(newExosList,containsMpFiles,this);
	connect(mpThread,SIGNAL(finished()),mpThread,SLOT(deleteLater()));
	connect(mpThread,SIGNAL(mpTreated()),this,SLOT(launchImportthread()));
	connect(mpThread,SIGNAL(errorOccured(const int &,const QString &)),
			this,SLOT(mpostErrorOccured(const int &,const QString &)));
	mpThread->start();
}

void Importer::launchImportthread()
{
	ImportThread *importThread = new ImportThread(newExosList,this);
	connect(importThread,SIGNAL(finished()),importThread,SLOT(deleteLater()));
	connect(importThread,SIGNAL(exoTreated(const QMap<QString,QString> *)),this,SLOT(pdfSuccess(const QMap<QString,QString> *)));
	connect(importThread,SIGNAL(errorOccured(const QString &,const QString &)),
			this,SLOT(compileErrorOccured(const QString &,const QString &)));
	importThread->start();
}


void Importer::pdfSuccess(const QMap<QString,QString>  * currentFileMap)
{	
	QMap<QString,QString> localExoMap = *currentFileMap;
	QString currentFile = localExoMap.value("filepath");
	QStringList extensions;
	
    extensions << ".log" << ".aux" << ".tex" << "-old.pdf" << ".out";
	
	QString trashFile;
	
	foreach (QString ext,extensions)
	{
		trashFile = QFileInfo(currentFile).path() + QDir::separator() + QFileInfo(currentFile).baseName() + "-preview" + ext;
		if (QFileInfo(trashFile).exists()) QFile(trashFile).remove();
	}
	
	if (!fileUpdate) { 
		domHandler->writeExoDom(localExoMap);
		localModel->updateModel(localExoMap,false);
                emit fileImported(currentFile);
	}
	else 
	{	
		domHandler->updateExo(localExoMap);
		localModel->updateModel(localExoMap,true);
                emit fileImported(currentFile);
	}
	
}

void Importer::mpostErrorOccured(const int &fileRef,const QString &error)
{
	QString errorText=QString(tr("The following problem happened trying to compile metapost file in exercise file %1 :")).arg(QFileInfo(newExosList.at(fileRef).value("filepath")).fileName());
	errorText+="\n\n <font color=\"red\">" + error + "</font>";
	QMessageBox::critical(0,tr("Error"),errorText);
}

void Importer::compileErrorOccured(const QString &currentFile,const QString &error)
{
	PdflatexLogDialog *pdflatexLogDialog = new PdflatexLogDialog(this,currentFile,error);
	pdflatexLogDialog->show();
}
