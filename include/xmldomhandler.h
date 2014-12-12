#ifndef XMLDOMHANDLER_H
#define XMLDOMHANDLER_H

class QDomDocument;
class QDomNode;

//!  Classe qui gère tout les échanges avec le fichier xml (lecture, écriture, ...)
class XmlDomHandler
{

public:

//! Constructeur
/*! 
	Prend en argument le nom (et chemin) du fichier xml contenant les données
*/
	XmlDomHandler(QDomDocument *domDoc);
	
//! Fonction qui crée un arbre dom (et le met dans la variable passée en argument)
	void createDomDocument();

//! Fonction qui rentre les données de l'exo dans l'arbre Dom et le fichier xml
	void writeExoDom(QMap<QString,QString> datas);

//! Fonction qui ajoute un élément dans settings
    void writeSettings(QString tagName,QStringList content);

    void removeSettings();

//! Fonction qui vérifie si une entrée de Settings dans l'arbre existe
    bool settingsNodeExists(QString tagName);

//! Sauvegarde l'arbre Dom dans le fichier Xml courant
    void saveInXml();

//! Récupère le setting "tagName" dans les descendants de Settings.
        QStringList getSettings(QString tagName);
	
//! Fonction qui supprime l'exo donné en argument de la base Dom et du fichier xml
	int removeExoDom(QStringList exoCars);

//! Retourne la valeur de dbsize (nb d'exos dans la db)
	int getCurrentDbsize();

//! Incrémente d'un la valeur de dbsize
	void incDbsize();
	
//! Cherche un exo de path (nom de fichier) donné dans l'arbre et retourne son Id. Retourne 0 si il n'y est pas
	int searchExoPath(QString path);

//! Cherche le contenu de la métadonnée meta dans l'exo path
	QString getExoMeta(QString path,QString meta);
	
//! Renvoi une QList  contenant la liste de tous les exos (title, field, ...) sous forme de QMap
	QList<QMap<QString,QString> > getExosList();

//! Renvoi une QStringList de tous les titres des exercices
        QStringList getExosTitleList();
	
//! Check l'intégrité du fichier XML et renvoi la liste des exercices qui posent problèmes
	QStringList  checkXmlIntegrity();

//! Retire de l'arbre Dom la metadonnée metaName de l'exo exoPath
    void  removeExoMeta(QString metaName);
	
//! Met à jour l'exercice donné en argument
	void updateExo(QMap<QString,QString> datas);
	
//! Récupère la liste des métadonnées de la base en cours
	QStringList getMetaList();
	
//! Récupère la liste des exercices (metapost et autres)
	QStringList getFigList(QString path);
	
private:
	
	//! Méthode utilisée par \a writeExoDom(QStringList datas) pour créer le nouveau noeud d'exercice
	QDomNode createNode(QMap<QString,QString> datas,int id);
	
	//! Arbre Dom
	QDomDocument * doc;
};
#endif
