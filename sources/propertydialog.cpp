#include <QtWidgets>
#include "propertydialog.h"
#include "xmldomhandler.h"
#include "preferences.h"

PropertyDialog::PropertyDialog(QWidget * parent,XmlDomHandler * domHandler, QString currentPath,bool iso)
: QDialog(parent), domHandler(domHandler), currentPath(currentPath),iso(iso)
{
	setupUi(this);
	
	setWindowTitle(QString(tr("Properties editor - %1").arg(QFileInfo(currentPath).fileName())));
	
	currentMetaMap = getOldMetaMap();
	
	createLayout();
	
	adjustSize();
}

void PropertyDialog::createLayout()
{
	QMapIterator<QString, QString> i(currentMetaMap);
	
	int j=0;
	while (i.hasNext()) 
	{
		i.next();
		
		if (!i.value().isEmpty())
		{
		editList.append(new QLineEdit(i.value(),this));
		labelList.append(new QLabel(i.key(),this));
		checkList.append(new QCheckBox(this));
		
		metaLayout->addWidget(checkList.at(j),j,0,1,1);
		metaLayout->addWidget(labelList.at(j),j,1,1,1);
		metaLayout->addWidget(editList.at(j),j,2,1,1);
		
		checkList.at(j)->setAutoExclusive(true);
		if ((i.key()=="importdate")||(i.key()=="filepath"))
		{	
			editList.at(j)->setReadOnly(true);
			checkList.at(j)->setEnabled(false);
		}
		j++;
		}
	}
	
	QStringList metaList; 
	
	foreach (QString str,Preferences::p_getMetaList()) if ((str!="filepath")&&(str!="importdate")) metaList << str;
	
	metaComboBox->addItems(metaList);
}

void PropertyDialog::on_addButton_clicked()
{
	int size = editList.size();
	
	editList.append(new QLineEdit(this));
	labelList.append(new QLabel(metaComboBox->currentText(),this));
	checkList.append(new QCheckBox(this));
	
	metaLayout->addWidget(checkList.at(size),size,0,1,1);
	metaLayout->addWidget(labelList.at(size),size,1,1,1);
	metaLayout->addWidget(editList.at(size),size,2,1,1);
	
	checkList.at(size)->setAutoExclusive(true);
}

void PropertyDialog::on_removeButton_clicked()
{
	for (int i=0;i<checkList.size();i++)
	{
		if (checkList.at(i)->isChecked())
		{
			checkList.at(i)->close();
			editList.at(i)->close();
			labelList.at(i)->close();
			
			checkList.removeAt(i);
			editList.removeAt(i);
			labelList.removeAt(i);
			
			adjustSize();
			return;
		}
	}
}

QMap<QString, QString> PropertyDialog::getOldMetaMap()
{
	QMap<QString, QString> metaMap;
	QStringList metaList = Preferences::p_getMetaList();
	QStringList currentMetaList = QStringList();
	QString currentMeta=QString();
	
	for (int i=0;i<metaList.size();i++)
	{
		currentMeta = domHandler->getExoMeta(currentPath,metaList.at(i));
		currentMetaList = currentMeta.split(" / ");
		
		for (int j=0;j<currentMetaList.size();j++) metaMap.insertMulti(metaList.at(i),currentMetaList.at(j));
	}
	
	return metaMap;
}

QMap<QString, QString> PropertyDialog::getNewMetaMap()
{
	QMap<QString, QString> metaMap;
	
	for (int i=0;i<labelList.size();i++) metaMap.insertMulti(labelList.at(i)->text(),editList.at(i)->text());
	
	return metaMap;
}

void PropertyDialog::on_applyButton_clicked()
{
	QMap<QString, QString> newMetaMap = getNewMetaMap();
	
	writeChangesToExo(newMetaMap);
	
	emit modifWritten();
	
	close();
}

void PropertyDialog::writeChangesToExo(QMap<QString, QString> map)
{
	// On ouvre le fichier en lecture
	QFile inFile(currentPath);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	
	QTextStream in(&inFile); // Création du flux
    in.flush();
    if (iso) in.setCodec("ISO 8859-15");
	
	
    // On récupère le texte de l'exo, en supprimant toutes les métadonnées existantes
	QString content=QString();
	int limit;
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
				content = content + line +"\n";	// Donc on ajoute la ligne dans le texte de l'exo
			}	
		}
		else content = content + line +"\n";
	}
	
	inFile.close();
	inFile.remove();
	
	// On crée une chaîne (metaStr) contenant les nouvelles métadonnées.
	QStringList newMetaList;
	QStringList metaList = Preferences::p_getMetaList();
	QStringList metaView = Preferences::p_getMetaToView();
	
	for (int i=0;i<metaView.size();i++)
	{
		QStringList valeurs = QStringList();
		valeurs = map.values(metaView.at(i));
		if ((!valeurs.isEmpty())&&(metaView.at(i)!="filepath")&&(metaView.at(i)!="importdate"))
		{ for (int j=0;j<valeurs.size();j++) 
			{
				if (!valeurs.at(j).isEmpty())
				{
					QString str = QString(tr("%@ %1: %2").arg(metaView.at(i)).arg(valeurs.at(j)));
					newMetaList.append(str);
				} 

			}
		}
		map.remove(metaView.at(i));
	}
	
	QMapIterator<QString, QString> k(map);

	while (k.hasNext()) 
	{
		k.next();
		if ((k.key()!="filepath")&&(k.key()!="importdate"))
		{
			QString str = QString(tr("%@ %1: %2").arg(k.key()).arg(k.value()));
			newMetaList.append(str);
		}
	}
	
	QString metaStr=QString();
	for (int i=0;i<newMetaList.size();i++)
	{
		metaStr+= newMetaList.at(i) + "\n";
	}
	
	content = metaStr + content;
	
	// On ouvre le fichier en écriture
	QFile outFile(currentPath);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	
	QTextStream out(&outFile); // Création du flux
    out.flush();
    if (iso) out.setCodec("ISO 8859-15");
	out << content;
	outFile.close();
}
