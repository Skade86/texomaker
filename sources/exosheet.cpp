#include <QtXml>
#include "exosheet.h"
#include "pdflatexlogdialog.h"
#include "headerdialog.h"
#include "xmldomhandler.h"
#include "preferences.h"

ExoSheet::ExoSheet( QWidget * parent,bool useIso,XmlDomHandler *handler) 
	: QDialog(parent), useIso(useIso)
{
	setupUi(this);
	outputDir = QString();
	outputFile = QString();
	exoSheetList.clear();
	listWidget->clear();
	domHandler = handler;
        favoritesBox->addItem(tr("Don't use a favorite"));
        favoritesBox->addItems(Preferences::p_getSheetBookmarks());
        favoritesBox->setCurrentIndex(0);
        commentEdit->selectAll();
        existingSheetOpened = false;
        dateLabel->setText("New sheet");
        revLabel->setNum(0);
        headerEdit->setText(Preferences::p_getSheetHeader());
        exoEdit->setText(Preferences::p_getSheetExo());
        betweenEdit->setText(Preferences::p_getSheetBetween());
        footerEdit->setText(Preferences::p_getSheetFooter());
}

void ExoSheet::on_openButton_clicked()
{
    QString rootDir;
    if (favoritesBox->currentIndex()==0) rootDir = QDir::homePath();
    else rootDir = favoritesBox->currentText();

    QString xmlFileName = QString();
    xmlFileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 rootDir,
                                                 tr("XML files (*.xml)"));
    if (xmlFileName.isEmpty()) return;

    createDomSheetFile(xmlFileName);

    // On vérifie que la base utilisée pour créer cette feuille était la même que l'actuelle.
    QString xmlBaseName = getDomBaseName();
    if (xmlBaseName != Preferences::p_getDbfile()) {
        QMessageBox::warning(this,tr("Error"),tr("This sheet wasn't created from the current opened base, but with :\n %1").arg(xmlBaseName));
        return;
    }
    existingSheetOpened = true;
    outputDir = getDomDirName();
  //  dirEdit->setText(outputDir);
    outputFile = QFileInfo(xmlFileName).baseName();
    nameEdit->setText(outputFile);

    QList <QStringList > exosList = getDomExoList();;
    QStringList baseExoList = domHandler->getExosTitleList();
    QStringList missingExos = QStringList();

    for (int i=0;i<exosList.size();i++) {
        if (!baseExoList.contains(exosList[i].at(0))) {
            missingExos << exosList[i].at(0);
            exosList.removeAt(i);
        }
    }

    if (!missingExos.isEmpty()) {
        QString warningTxt = tr("The following exercises are not anymore is the current base :\n");
        foreach (QString str,missingExos) warningTxt += str +"\n";
        QMessageBox::warning(this,tr("Exercises missing"),warningTxt);
    }

    for (int i=0;i<exosList.size();i++) listWidget->addItem(exosList[i].at(0));
    exoSheetList += exosList;

    historyNode = new QDomNodeList(domSheet->elementsByTagName("History"));
    revLabel->setNum(getCurRevNumber());
    commentEdit->setText(getCurComment());
    dateLabel->setText(getCurDate());
    QStringList optionsList = getLatexOptionList();

    if (!optionsList.isEmpty()) {
        headerEdit->setText(optionsList.at(0));
        exoEdit->setText(optionsList.at(1));
        betweenEdit->setText(optionsList.at(2));
        footerEdit->setText(optionsList.at(3));
    }
}

QString ExoSheet::getDomBaseName()
{
    QDomNodeList baseList = domSheet->elementsByTagName("Base");
    QString baseName = baseList.item(0).toElement().text();
    return baseName;
}

QString ExoSheet::getDomDirName()
{
    QDomNodeList dirList = domSheet->elementsByTagName("Directory");
    QString dirName = dirList.item(0).toElement().text();
    return dirName;
}

int ExoSheet::getCurRevNumber()
{
    QDomNodeList hisNode = domSheet->elementsByTagName("History");
    QDomNode lastRev = hisNode.item(0).lastChildElement("Revision");
    int revNb = lastRev.toElement().attribute("No").toInt();
    return revNb;
}

QString ExoSheet::getCurComment()
{
    QDomNodeList hisNode = domSheet->elementsByTagName("History");
    QDomNode lastRev = hisNode.item(0).lastChildElement("Revision");
    lastRev = lastRev.lastChildElement("Comment");
    QString com = lastRev.toElement().text();
    return com;
}

QString ExoSheet::getCurDate()
{
    QDomNodeList hisNode = domSheet->elementsByTagName("History");
    QDomNode lastRev = hisNode.item(0).lastChildElement("Revision");
    lastRev = lastRev.lastChildElement("Date");
    QString date = lastRev.toElement().text();
    return date;
}

QList<QStringList > ExoSheet::getDomExoList()
{
    QList<QStringList > exosList;
    QStringList exoList = QStringList();
    QDomNodeList exoNodeList = domSheet->elementsByTagName("Exercise");
    for (int i=0;i<exoNodeList.size();i++) {
        exoList.clear();
        QDomNodeList childNodesList  = exoNodeList.item(i).childNodes();
        for (int j=0;j<childNodesList.size();j++) {
            exoList << childNodesList.item(j).toElement().text();
        }
        exosList.append(exoList);
    }
    return exosList;
}

QStringList ExoSheet::getLatexOptionList()
{
    QStringList optionList=QStringList();
    QDomNodeList optionNodeList = domSheet->elementsByTagName("LatexOptions");
    if (optionNodeList.size()==0) return optionList;
    QDomNodeList childNodesList  = optionNodeList.item(0).childNodes();
    for (int j=0;j<childNodesList.size();j++) {
        optionList << childNodesList.item(j).toElement().text();
    }
    return optionList; // Header - Exos - Between - Footer
}

void ExoSheet::addExo(QStringList exoList)
{
	listWidget->addItem(exoList.at(0));
	exoSheetList.append(exoList);
}

void ExoSheet::on_upButton_clicked()
{
    int curRow = listWidget->currentRow();
    if ((listWidget->count() == 0) || (curRow==0) )  return;
    QListWidgetItem * movedItem = listWidget->takeItem(curRow);
    listWidget->insertItem(curRow-1,movedItem);
    listWidget->setCurrentRow(curRow-1);
}

void ExoSheet::on_downButton_clicked()
{
    int curRow = listWidget->currentRow();
    if ((listWidget->count() == 0) || (curRow==listWidget->count()-1))  return;
    QListWidgetItem * movedItem = listWidget->takeItem(curRow);
    listWidget->insertItem(curRow+1,movedItem);
    listWidget->setCurrentRow(curRow+1);
}

void ExoSheet::on_removeButton_clicked()
{
	QListWidgetItem * removedItem = listWidget->takeItem(listWidget->currentRow());
	
	if (removedItem==0)
	{
		QMessageBox::warning(0, QObject::tr("Error"),QObject::tr("Please select an exercise to remove !"));
		return;
	}
	
	QString exoTitle = removedItem->text();
	
	for (int i=0;i<exoSheetList.size();i++)
	{
		if (exoSheetList.at(i)[0]==exoTitle) exoSheetList.removeAt(i);
	}
	
}

void ExoSheet::on_createButton_clicked()
{
	// On commence par vérifier que le nom et le répertoire de la feuille ont été définis
        if (outputFile.isEmpty()) outputFile = nameEdit->text();
        if (outputDir.isEmpty()) {
        if (favoritesBox->currentText() == tr("Don't use a favorite")) {

            if (outputDir.isEmpty() || outputFile.isEmpty()) {
                    QMessageBox::warning(this, QObject::tr("Error"),QObject::tr("Please choose a destination directory and a file name !"));
                    return;
            }
        }
        else outputDir = favoritesBox->currentText();
    }

        if (listWidget->count()==0) {
            QMessageBox::warning(this, QObject::tr("Error"),QObject::tr("Please choose at least an exercise (by double-clicking on the left panel) !"));
            return;
        }

        if (addBox->isChecked()) {
            if (favoritesBox->findText(outputDir) == -1) {
                favoritesBox->addItem(outputDir);
                QStringList curFav = Preferences::p_getSheetBookmarks();
                curFav << outputDir;
                Preferences::p_setSheetBookmarks(curFav);
            }
        }
	
	// Copie des figures des exercices dans le bon dossier
	
	bool mpFigs =false;
	for (int i =0; i<exoSheetList.size(); i++)
		{
			if (copyFigsFiles(exoSheetList.at(i))) mpFigs=true;
		}
	
	// Création du fichier *tex
	createTexFile(mpFigs);
	
	// Compilation du fichier de sortie
	stdPdflatexOutput = compileTexFile();
	
	QString pdfFile = outputDir + QDir::separator() + outputFile +".pdf";
        QString xmlFile;
	QFile file;
	if (file.exists(pdfFile))
		{
                        xmlFile = createXmlSheetFile();         // Création du fichier xml vide
                        createDomSheetFile(xmlFile);            // Création de l'arbre DOM
                        writeXmlSheetFile(commentEdit->toPlainText()); // Ecriture des données dans l'arbre
                        QFile fileOut(xmlFile);                 // Ecriture de l'arbre dans le fichier xml
                        fileOut.open(QIODevice::WriteOnly);
                        QTextStream out(&fileOut);
                        domSheet->save(out,4);
                        Preferences::p_setSheetHeader(headerEdit->toPlainText());
                        Preferences::p_setSheetExo(exoEdit->toPlainText());
                        Preferences::p_setSheetBetween(betweenEdit->toPlainText());
                        Preferences::p_setSheetFooter(footerEdit->toPlainText());
			emit pdflatexFinished(pdfFile,stdPdflatexOutput);
                        closeButton->click();
		}
	else
		{
			PdflatexLogDialog *pdflatexLogDialog = new PdflatexLogDialog(this,sheetFile,stdPdflatexOutput);
			pdflatexLogDialog->show();
		}
}

void ExoSheet::createTexFile(bool mpTest)
{
	// Création du nom complet du fichier tex
	sheetFile = outputDir + QDir::separator() + outputFile +".tex";
	
	// Chaîne qui contiendra le contenu du fichier tex final
	QString sheetFileContent; 
	
	//*************Remplissage de sheetFileContent
	
	// On commence par le préambule et le \begin{document}
	sheetFileContent = Preferences::p_getPreamble()+ "\n" + Preferences::p_getMacroFiles() + "\n";
	
	if (mpTest) sheetFileContent += "\n\\DeclareGraphicsRule{*}{mps}{*}{}\n\n";
	
	if (Preferences::p_getCompiler()!="tex") sheetFileContent += "\n"+Preferences::p_getBeginDoc()+"\n";
		else sheetFileContent += "\n\n";
	
	// On ajoute l'éventuel header
        sheetFileContent += headerEdit->toPlainText() + "\n\n";
	
	// On boucle sur la liste des exercices et on ajoute successivement :
	//		@ le titre de l'exercice dans \exo{}
	//		@ le texte de l'exercice
	for (int i =0; i<exoSheetList.size(); i++)
		{
                        QString sheetExo = replaceMeta(exoSheetList[i].at(1),exoEdit->toPlainText());
			
			sheetFileContent += "\n" + sheetExo +"\n";
			QFile exoFile(exoSheetList[i].at(1));
			exoFile.open(QFile::ReadOnly | QFile::Text);
			QTextStream exoStream(&exoFile);
			exoStream.flush();
			if (useIso) exoStream.setCodec("ISO 8859-15");
                        sheetFileContent += exoStream.readAll() + "\n" + betweenEdit->toPlainText() +"\n";
		}
	
	// On ajoute enfin la fin du fichier
	
	
        if (Preferences::p_getCompiler()!="tex") sheetFileContent += footerEdit->toPlainText() + "\n"+Preferences::p_getEndDoc();
		else sheetFileContent += Preferences::p_getSheetFooter();
	
	// On crée le fichier de sortie, on l'ouvre en écriture
	// et on le remplit avec la chaîne sheetFileContent que l'on vient de créer
	QFile file(sheetFile);
	file.open(QFile::WriteOnly | QFile::Text);
	QTextStream out(&file);
	out.flush();
	if (useIso) out.setCodec("ISO 8859-15");
	out << sheetFileContent;
}

QString ExoSheet::replaceMeta(QString path,QString str)
{
	QString text;
	QString newText;
	
	int fromIndex=0;
	int toIndex,index=0;
	
	while(index!=-1)
	{
	index = str.indexOf("%@",fromIndex);
	if (index==-1) return str;
	toIndex = str.indexOf("@",index+2);
	if (toIndex==-1) return str;
	text = str.mid(index+2,toIndex-index-2).trimmed();
//	QMessageBox::warning(0, QObject::tr("Error"),QObject::tr("index : %1\ntoIndex : %2\ntext : %3").arg(index).arg(toIndex).arg(text));
	newText = domHandler->getExoMeta(path,text);
//	QMessageBox::warning(0, QObject::tr("Error"),QObject::tr("newText : %1").arg(newText));
	str.replace("%@"+text+"@",newText);
	fromIndex=toIndex;
	}
	return str;
}

void ExoSheet::on_browseButton_clicked()
{
	// Cette méthode ouvre une fenêtre permettant de choisir le
	// répertoire de sortie, outputDir
        QString initialDir = QDir::homePath();
	QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     initialDir,
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    dirName =  QDir::toNativeSeparators(dirName);
    if (!dirName.isEmpty()) 
    	{
    		outputDir = dirName;
    	}
}

QString ExoSheet::createXmlSheetFile()
{
    // On crée le nom du fichier à créer
    QString fileName = outputDir + QDir::separator() + outputFile + ".xml";

    // On le crée (en ouvrant un fichier qui n'existe pas cela le crée)
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,tr("Error"),
                             tr("Cannot create file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return QString();
    }

    // On crée la chaîne à écrire dans le fichier
    QString emptyDbTxt = "<?xml version='1.0'?>\n";
    emptyDbTxt = emptyDbTxt + "<sheet>\n"+"</sheet>";

    // On ouvre un flux
    QTextStream out(&file);
    out.flush();

    // On écrit dans le fichier et on le ferme
    out << emptyDbTxt;
    file.close();

    return fileName;
}

void ExoSheet::createDomSheetFile(QString name)
{
    // Création de l'arbre Dom
    domSheet = new QDomDocument("xmlSheet");
    QString errorStr;
    int errorLine;
    int errorColumn;
    QFile file(name);
    if (!domSheet->setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        QMessageBox::warning(0, QObject::tr("DOM Parser"),
                             QObject::tr("Parse error at line %1, "
                                         "column %2:\n%3")
                             .arg(errorLine)
                             .arg(errorColumn)
                             .arg(errorStr));
        return;
    }
}

void ExoSheet::writeXmlSheetFile(QString comment)
{
    QDomElement root=domSheet->documentElement();

    // Ajout du nom de la base
    QDomElement baseElt = domSheet->createElement("Base");
    QDomText baseTxt = domSheet->createTextNode(Preferences::p_getDbfile());
    root.appendChild(baseElt);
    baseElt.appendChild(baseTxt);

    // Ajout du répertoire de destination
    QDomElement dirElt = domSheet->createElement("Directory");
    QDomText dirTxt = domSheet->createTextNode(outputDir);
    root.appendChild(dirElt);
    dirElt.appendChild(dirTxt);

    // Ajout de l'historique (Date/Revision/commentaire)

    QDomElement hisElt;

    if (!existingSheetOpened) hisElt = domSheet->createElement("History");
    else hisElt = historyNode->item(0).toElement();

    root.appendChild(hisElt);

    QDomElement revElt = domSheet->createElement("Revision");
    hisElt.appendChild(revElt);
    int newRev = revLabel->text().toInt();
    if (dateLabel->text()!="New sheet") newRev+=1;
    revElt.setAttribute("No",newRev);

    QDomElement datElt = domSheet->createElement("Date");
    QDateTime dateTime = QDateTime::currentDateTime();
    QString today = dateTime.toString("dd MMMM yyyy - hh:mm");
    QDomText datTxt = domSheet->createTextNode(today);
    revElt.appendChild(datElt);
    datElt.appendChild(datTxt);

    QDomElement comElt = domSheet->createElement("Comment");
    QDomText comTxt = domSheet->createTextNode(comment);
    revElt.appendChild(comElt);
    comElt.appendChild(comTxt);

    // Ajout des options Latex

    QDomElement latexElt = domSheet->createElement("LatexOptions");

    root.appendChild(latexElt);

    QDomElement headerElt = domSheet->createElement("Header");
    latexElt.appendChild(headerElt);
    QDomText headerTxt = domSheet->createTextNode(headerEdit->toPlainText());
    headerElt.appendChild(headerTxt);

    QDomElement exoHeadElt = domSheet->createElement("ExerciseHeader");
    latexElt.appendChild(exoHeadElt);
    QDomText exoHeadTxt = domSheet->createTextNode(exoEdit->toPlainText());
    exoHeadElt.appendChild(exoHeadTxt);

    QDomElement betweenElt = domSheet->createElement("Between");
    latexElt.appendChild(betweenElt);
    QDomText betweenTxt = domSheet->createTextNode(betweenEdit->toPlainText());
    betweenElt.appendChild(betweenTxt);

    QDomElement footerElt = domSheet->createElement("Footer");
    latexElt.appendChild(footerElt);
    QDomText footerTxt = domSheet->createTextNode(footerEdit->toPlainText());
    footerElt.appendChild(footerTxt);

    // Ajout des exercices
    for (int i=0;i<exoSheetList.size();i++) {
        QDomElement exoElt = domSheet->createElement("Exercise");
        root.appendChild(exoElt);

        QDomElement namElt = domSheet->createElement("Name");
        QDomText namTxt = domSheet->createTextNode(exoSheetList[i].at(0));
        exoElt.appendChild(namElt);
        namElt.appendChild(namTxt);

        QDomElement filElt = domSheet->createElement("FilePath");
        QDomText filTxt = domSheet->createTextNode(exoSheetList[i].at(1));
        exoElt.appendChild(filElt);
        filElt.appendChild(filTxt);

        QDomElement figElt = domSheet->createElement("Figures");
        QDomText figTxt = domSheet->createTextNode(exoSheetList[i].at(2));
        exoElt.appendChild(figElt);
        figElt.appendChild(figTxt);

        QDomElement metElt = domSheet->createElement("Metapost");
        QDomText metTxt = domSheet->createTextNode(exoSheetList[i].at(3));
        exoElt.appendChild(metElt);
        metElt.appendChild(metTxt);
    }
}

bool ExoSheet::copyFigsFiles(QStringList exo)
{
	if ((exo.at(2).isEmpty()) && (exo.at(3).isEmpty())) return false;
	
	
	QStringList figList=QStringList();
		
	figList = domHandler->getFigList(exo.at(1));

	// Nom du répertoire de l'exercice courant
	QString exoDir = QFileInfo(exo.at(1)).path();
	
	// On boucle sur la taille de la liste, on extrait successivement
	// tous les fichiers de figure sous forme de chaîne, et on les copie
	// dans le répertoire de la feuille d'exercice, outputDir
	for (int i = 1; i < figList.size(); ++i)
		{
 			QString completeFigName = exoDir + QDir::separator() + figList.at(i);
 			QString newCompleteFigName = outputDir + QDir::separator() + figList.at(i);
 			QFile file(completeFigName);
 			bool test = file.copy(newCompleteFigName);

			QString Test="Succeed";
			if (!test) Test="Failed";
 		}
	
	if (figList.at(0)=="mp") return true;
	return false;
}

QString ExoSheet::compileTexFile()
{
	QProcess compileProcess;
	QString processBin = QString();
	
	if ((Preferences::p_getCompiler()=="latex")||(Preferences::p_getCompiler()=="tex"))
	{
		processBin=Preferences::p_getLtx2pdf();
	}
	else processBin=Preferences::p_getBin(Preferences::p_getCompiler());
	
#ifndef Q_WS_WIN
	QStringList env = QProcess::systemEnvironment();
	int j = env.indexOf(QRegExp("^PATH=(.*)"));
	int limit = env.at(j).indexOf("=");
	QString value = env.at(j).right(env.at(j).size()-limit-1).trimmed();
	value = "PATH=" + value + ":" + QFileInfo(processBin).path() + ":" + QFileInfo(Preferences::p_getBin("latex")).path() + ":" + QFileInfo(Preferences::p_getBin("dvips")).path() + ":" + QFileInfo(Preferences::p_getBin("ps2pdf")).path();
	env.replace(j,value);
	compileProcess.setEnvironment(env);
#endif
	
	compileProcess.setWorkingDirectory(outputDir);
	
	QStringList args;
	
	if ((Preferences::p_getCompiler()=="latex")||(Preferences::p_getCompiler()=="tex"))
	{ 
#ifdef Q_WS_WIN
		args << outputFile << Preferences::p_getBin("latex") << Preferences::p_getBin("dvips") << Preferences::p_getBin("ps2pdf") << Preferences::p_getCompilationOptions();
#else
		args << "-c" << outputFile +".tex" << Preferences::p_getBin("latex") << Preferences::p_getBin("dvips") << Preferences::p_getBin("ps2pdf") << Preferences::p_getCompilationOptions();
#endif
	}
	else args << Preferences::p_getCompilationOptions() << outputFile +".tex";
	
	compileProcess.start(processBin,args);
	compileProcess.waitForFinished(-1);
	return compileProcess.readAll();
}
