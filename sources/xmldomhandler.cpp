#include <QtWidgets>
#include <QtXml>

#include "xmldomhandler.h"
#include "preferences.h"

XmlDomHandler::XmlDomHandler(QDomDocument *domDoc) 
{
	doc = domDoc;
}

void XmlDomHandler::createDomDocument()
{
	QString errorStr;
	int errorLine;
	int errorColumn;
	QFile file(Preferences::p_getDbfile());
	
	if (!doc->setContent(&file, true, &errorStr, &errorLine,
                        &errorColumn)) 
	 {
        QMessageBox::warning(0, QObject::tr("DOM Parser"),
                             QObject::tr("Parse error at line %1, "
                                         "column %2:\n%3")
                             .arg(errorLine)
                             .arg(errorColumn)
                             .arg(errorStr));
        return;
	}
	
	// Si il y a déjà des exercices dans la base, on récupère la liste des types de métadonnées, et on
	// met à jour la variable statique Preferences::metaList
	if (getCurrentDbsize()>0)
	{
		QStringList metaList = getMetaList();
		Preferences::p_setMetaList(metaList);
	}
}

QStringList XmlDomHandler::getMetaList()
{
	QStringList metaList;
	QDomElement root = doc->documentElement();
	QDomNode ExoNode = root.firstChild();

        while (!ExoNode.isNull()) {
            if (ExoNode.nodeName()=="exo") {
                QDomNode curNode = ExoNode.firstChild(); // Node courant dans l'exo
                while (!curNode.isNull())
                {
                        metaList << curNode.nodeName();
                        curNode = curNode.nextSibling();
                }
                break;
            } else ExoNode = ExoNode.nextSibling();
        }
	
	return metaList;
}

QStringList XmlDomHandler::getSettings(QString tagName)
{
        QStringList settingsList=QStringList();
        QDomElement root = doc->documentElement();
        QDomNode curNode = root.firstChild();

        while (!curNode.isNull()) {
            if (curNode.nodeName()=="Settings") {
                QDomNodeList optDomList = curNode.toElement().elementsByTagName(tagName);
                for (int i=0;i<optDomList.size();i++) settingsList << optDomList.item(i).toElement().text();
                break;
            } else curNode = curNode.nextSibling();
        }

        return settingsList;
}

bool XmlDomHandler::settingsNodeExists(QString tagName)
{
    bool reponse=false;

    QDomElement root = doc->documentElement();
    QDomNode curNode = root.firstChild();

    while (!curNode.isNull()) {
        if (curNode.nodeName()=="Settings") {
            curNode=curNode.firstChild();
            break;
        } else curNode = curNode.nextSibling();
    }

    while (!curNode.isNull()) {
        if (curNode.nodeName()==tagName) {
            reponse=true;
            break;
        } else curNode = curNode.nextSibling();
    }
    return reponse;
}

void XmlDomHandler::writeExoDom(QMap<QString,QString> datas)
{
	QDomNode newNode;

	newNode = createNode(datas,getCurrentDbsize() + 1);
	incDbsize();
	QDomElement root=doc->lastChild().toElement();
	root.appendChild(newNode.toElement());

    saveInXml();
}

void XmlDomHandler::removeSettings()
{
    QDomElement root = doc->documentElement();

    QDomNode curNode = root.firstChild();

    while (!curNode.isNull()) {
        if (curNode.nodeName()=="Settings") break;
        curNode = curNode.nextSibling();
    }

    root.removeChild(curNode);
}

void XmlDomHandler::writeSettings(QString tagName,QStringList content)
{
    QDomElement root = doc->documentElement();

    QDomNode curNode = root.firstChild();

    while (!curNode.isNull()) {
        if (curNode.nodeName()=="Settings") break;
        curNode = curNode.nextSibling();
    }

    QDomElement settingsElt;

    if (curNode.isNull()) {
        settingsElt = doc->createElement("Settings");
        root.appendChild(settingsElt);
    }
    else settingsElt = curNode.toElement();
    
    for (int i=0;i<content.size();i++) {
        QDomElement curElt = doc->createElement(tagName);
        settingsElt.appendChild(curElt);
        QDomText curTxt = doc->createTextNode(content.at(i));
//        QMessageBox::warning(0, QObject::tr("Warning"),QObject::tr("%1").arg(content.at(i)));
        curElt.appendChild(curTxt);
    }
}

void XmlDomHandler::saveInXml()
{
    // Sauvegarde des modifications dans le fichier Xml
    QFile fileOut(Preferences::p_getDbfile());
    fileOut.open(QIODevice::WriteOnly);
    QTextStream out(&fileOut);
    doc->save(out,4,QDomNode::EncodingFromTextStream);
}

int XmlDomHandler::removeExoDom(QStringList exoCars)
{
	const int indent = 4;
	int curId = 0;
	QDomElement root = doc->documentElement();
	QDomNodeList titleDomList = doc->elementsByTagName(exoCars.at(0));
	
    for (int i = 0; i < titleDomList.length(); ++i)
			{
				QDomNode item = titleDomList.item(i);
				QString curTitle = item.toElement().text();
				if (curTitle == exoCars.at(1))
					{	
						QDomNode nodeToRemove = item.parentNode();
						curId = nodeToRemove.toElement().attribute("Id").toInt();
						QDomNode nextExo = nodeToRemove.nextSibling();
						root.toElement().removeChild(nodeToRemove);
						while (!nextExo.isNull())
							{
								int exoId = nextExo.toElement().attribute("Id").toInt();
								nextExo.toElement().setAttribute("Id",exoId-1);
								nextExo = nextExo.nextSibling();
							}
						int dbsizeId = root.attribute("dbsize").toInt();
						root.setAttribute("dbsize",dbsizeId-1);
						break;
					}
			}
	
	QFile fileOut(Preferences::p_getDbfile());
	fileOut.open(QIODevice::WriteOnly);
	QTextStream out(&fileOut);
    doc->save(out,indent,QDomNode::EncodingFromTextStream);
	return curId;
}

QDomNode XmlDomHandler::createNode(QMap<QString,QString> datas,int id)
{
	QDomDocument newdoc;
	QDomNode node;
	
	QStringList metaList = Preferences::p_getMetaList();
	
	QDomElement exo=newdoc.createElement("exo");
	exo.setAttribute("Id",id);
	newdoc.appendChild(exo);
	
	QDomElement metaName = QDomElement();
	QDomText metaText = QDomText();
	
	QMapIterator<QString, QString> i(datas);
	
	while (i.hasNext())
	{
		i.next();
		if (!(i.key()=="exoText"))
		{
		metaName=newdoc.createElement(i.key());
		metaText=newdoc.createTextNode(i.value());
		
		exo.appendChild(metaName);
		metaName.appendChild(metaText);
		
		metaName.clear();
		metaText.clear();
		}
	}

	node = newdoc.firstChild();
	return node;
}

int XmlDomHandler::getCurrentDbsize() 
{
	QDomElement root=doc->documentElement();
	int dbsizeId = root.attribute("dbsize").toInt();
	return dbsizeId;
}

void XmlDomHandler::incDbsize() 
{
	QDomElement root=doc->documentElement();
	int dbsizeId = root.attribute("dbsize").toInt();
	root.setAttribute("dbsize",dbsizeId+1);
}

int XmlDomHandler::searchExoPath(QString path)
{
	QDomNodeList pathDomList = doc->elementsByTagName("filepath");
    for (int i = 0; i < pathDomList.length(); ++i)
			{
				QDomNode item = pathDomList.item(i);
				QString curPath = item.toElement().text();
				if (curPath == path) return 1;
			}
	return 0;
}

QString XmlDomHandler::getExoMeta(QString path,QString meta)
{
	QString metaText;
	
	QDomNodeList pathDomList = doc->elementsByTagName("filepath");
	
    for (int i=0;i<pathDomList.length(); ++i)
	{
		QDomNode item = pathDomList.item(i);
		QString curPath = item.toElement().text();
		if (curPath == path)
		{
			QDomNode nodeToUpdate = item.parentNode();
			QDomNode curElement = nodeToUpdate.firstChild();
			while (!curElement.isNull())
			{  
				QDomNode e = curElement.firstChild();
				QString eTag = curElement.toElement().tagName();
				if ((eTag==meta)&&(metaText.isEmpty())) metaText =  e.toText().data();
				else if ((eTag==meta)&&(!metaText.isEmpty())) metaText =  metaText + " / " + e.toText().data();
				curElement = curElement.nextSibling();
			}
		}
	}
	
	return metaText;
}

QStringList XmlDomHandler::getFigList(QString path)
{
	QDomNodeList pathDomList = doc->elementsByTagName("filepath");
	QDomNode item;
	QDomElement exoNode;
    for (int i = 0; i < pathDomList.length(); ++i)
	{
		item = pathDomList.item(i);
		QString curPath = item.toElement().text();
		if (curPath == path) exoNode = item.parentNode().toElement();
	}
	
	QStringList figList=QStringList();
	
	QDomNodeList figNodes = exoNode.elementsByTagName("figure");
	QDomNodeList mpNodes = exoNode.elementsByTagName("metapost");
	
	figList << "nomp";
	
    for (int i = 0; i < figNodes.length(); ++i)
	{
		item = figNodes.item(i);
		if (!item.toElement().text().isEmpty()) figList << item.toElement().text();
	}
	
	QString mpFig;
    for (int i = 0; i < mpNodes.length(); ++i)
	{
		item = mpNodes.item(i);
		mpFig= item.toElement().text();
		if (!mpFig.isEmpty()) figList.replace(0,"mp");
		mpFig=QFileInfo(mpFig).baseName()+".1";
		if (!item.toElement().text().isEmpty()) figList << mpFig;
	}
	
	return figList;
}

QStringList XmlDomHandler::getExosTitleList()
{
    QList<QMap<QString,QString> > exosMap = getExosList();
    QStringList exosList = QStringList();

    for (int i=0;i<exosMap.size();i++) {
        QString exoTitle = exosMap.at(i).value(Preferences::p_getMetaList().at(0));
        exosList << exoTitle;
    }

    return exosList;
}

QList<QMap<QString,QString> > XmlDomHandler::getExosList()
{
	QList<QMap<QString,QString> > curList;
	QMap<QString,QString> curExoMap;
	QString metaName;
	QString metaContent;
	QDomNode textNode;

	QDomElement root = doc->documentElement();

	QDomNode ExoNode = root.firstChild();
	while (!ExoNode.isNull())
        {
            if (ExoNode.nodeName()=="exo") {
		QDomNode curNode = ExoNode.firstChild(); // Node courant dans l'exo
		while (!curNode.isNull())
		{
			metaName = curNode.nodeName();
			textNode = curNode.firstChild();
			metaContent = textNode.toText().data();
			curExoMap.insertMulti(metaName,metaContent);
			curNode = curNode.nextSibling();
		}
		curList.append(curExoMap);
		curExoMap.clear();
            }
		ExoNode = ExoNode.nextSibling();
	}
	
	return curList;
}

void  XmlDomHandler::removeExoMeta(QString metaName)
{

    QDomNodeList pathDomList = doc->elementsByTagName("exo");

    for (int i=0;i<pathDomList.length(); ++i)
    {
        QDomNode exoChild = pathDomList.item(i).firstChild();

        while(!exoChild.isNull()) {

            if (exoChild.nodeName()==metaName) {
                QDomNode nodeToRemove = exoChild;
                exoChild = exoChild.nextSibling();
                pathDomList.item(i).removeChild(nodeToRemove);
                break;
            }
            exoChild = exoChild.nextSibling();
        }

    }

    return;
}

QStringList XmlDomHandler::checkXmlIntegrity()
{
	QStringList exoPbList = QStringList();
	
	QDomNodeList pathDomList = doc->elementsByTagName("filepath");
	
    for (int i = 0; i < pathDomList.length(); ++i)
			{
				QDomNode item = pathDomList.item(i);
				QString curExo = item.toElement().text();
				QFile exoFile(curExo);
				QString curExoPreview = QFileInfo(curExo).path() + QDir::separator()+ QFileInfo(curExo).baseName()+"-preview.pdf";
				QFile previewFile(curExoPreview);
				
				if ( (exoFile.exists()) && (!previewFile.exists()) ) exoPbList << curExoPreview ;
				if (!exoFile.exists()) exoPbList << curExo ;
			}
	
	return exoPbList;
}

void XmlDomHandler::updateExo(QMap<QString,QString> datas)
{
	QDomNodeList pathDomList = doc->elementsByTagName("filepath");
	
    for (int i = 0; i < pathDomList.length(); ++i)
			{
				QDomNode item = pathDomList.item(i);
				QString curPath = item.toElement().text();
				if (curPath == datas.value("filepath"))
					{
						
						QDomNode nodeToUpdate = item.parentNode();
						QDomNode curElement = nodeToUpdate.firstChild();
						for (int i = 0;i<Preferences::p_getMetaSize();i++)
							{  
								QDomNode e = curElement.firstChild();
								QString eTag = curElement.toElement().tagName();
								QString eValue = e.toText().data();
								if (eValue != datas.value(eTag)) e.setNodeValue(datas.value(eTag));
								curElement = curElement.nextSibling();
							}
						
					}
			}

        saveInXml();
}
