#ifndef IMPORTER_H
#define IMPORTER_H

#include <QWidget>
#include <QDateTime>
#include <QProcess>
#include <QMap>

class ExosModel;
class QDomDocument;
class XmlDomHandler;
class Viewer;

//! Classe qui gère l'importation des exos : création des aperçus pdf via pdflatex, des métadonnées, 

class Importer : public QWidget
{

Q_OBJECT

    signals:

    void fileImported(const QString & file);

public:

//! Constructeur
/*! 
	Prend comme arguments la liste des fichiers à importer (venant d'un QFileDialog de MainWindow); le préambule LateX courant; 
	un booléen qui est vrai si l'utilisation de l'encodage ISO-8859-15 est demandé, un pointeur sur l'arbre Dom courant,
	Le nom du fichier XML, et un pointeur sur le modèle courant.
*/
    Importer(QStringList texFiles,bool iso,QDomDocument *doc,ExosModel *model,Viewer *affiche,bool update);

//! Fonction publique appelée par MainWindow qui va effectuer le travail d'importation.
	void importFiles();
	
private slots:

	//! Méthode qui effectue les opérations nécessaires après la création du pdf preview.
	/*!
	C'est un slot qui est connecté à un signal du thread de la classe \a ImportThread dans lequel se fait la compilation pdflatex.
	Cette méthode est appelée en cas de création réussie par pdflatex. Ce slot récupère le nom du fichier dont la compilation
	a réussi et  effectue les opérations suivantes :
	- nettoyage des fichiers auxiliaires (.log, .aux et le fichier *-preview.tex)
	- ajout de l'exercice dans l'arbre Dom et écriture dans le fichier XML
	- ajout de l'exercice dans le modèle model (qui met à jour la vue automatiquement)
	*/
	void pdfSuccess(const QMap<QString,QString> * currentFileMap);
	
	//! Méthode appelée en cas d'erreur de pdflatex
	/*!
	Ce slot est connecté à un signal du thread de traitement de la classe \a ImportThread; signal émis si aucun pdf n'a été produit
	par pdflatex. Cette fonction récupère le nom du fichier qui a provoqué l'erreur, ainsi que la sortie standard de pdflatex. Elle crée une instance
	de la classe \a PdflatexLogDialog qui informe l'utilisateur de l'erreur.
	*/
	void compileErrorOccured(const QString &currentFile, const QString &error);
	
	//! Méthode appelée en cas d'erreur de metapost
	/*!
	 Ce slot est connecté à un signal du thread de traitement de la classe \a MpThread; signal émis lors d'un problème avec
	 metapost (soit une erreur de compilation soit un fichier .mp absent par exemple). Cette fonction récupère la référence dans newExosList
	 de l'exo où se situe l'erreur, qui a provoqué l'erreur, ainsi que la sortie standard.
	 */
	void mpostErrorOccured(const int &fileRef, const QString &error);
	
	void launchImportthread();
	
private:
//! booléen local (égal à iso)
	bool useIso;
	
//! Liste locale des fichiers à importer
	QStringList fileList;
	
//! Contenu du fichier courant dans la boucle sur la fileList
	QString fileContent;
	
//! Liste contenant le texte de l'exercice plus les métadonnées de l'exo courant
	QMap<QString,QString> exoDatas;
	
//! Liste contenant les métadonnées de l'exo courant
	QMap<QString,QString> exoMetaDatas;
	
//! Lit le fichier dont le nom est passé en argument.
/*!
	Renvoi un tableau associatif constitué de la liste des types de métadonnées auxquels on associe
	le contenu (il peut donc y avoir par exemple plusieurs "figure"). Le tableau contient également le texte propre
	de l'exercice.
*/
	QMap<QString,QString> readFileContent(QString exoFile);
	
//! Crée le fichier .tex correspondant à l'exo courant (avec le préambule courant).
	void createLatexFile(QString content,QString currentFile);

//! Date actuelle
	QDateTime date;
	
//! Liste de QMap, chacun contenant les métadonnées d'un exercice à importer sous forme d'un tableau associatif.
	QList<QMap<QString,QString> > newExosList;
	
//! Instance de la classe \a XmlDomHandler utilisée pour mettre à jour la base de donnée
	XmlDomHandler * domHandler;

//! Pointeur local à la classe, sur l'arbre Dom courant
	QDomDocument *localDoc;
	
//! Booléen indiquant s'il s'agit d'une nouvelle importation ou de la mise à jour d'un exercice
	bool fileUpdate;
	
//! Booléen devenant True si au moins un exercice à importer (ou mettre à jour) contient une figure metapost
	bool metapostFigs;

//! Pointeur local sur le modèle de la classe \a ExosModel courant
	ExosModel *localModel;
	
//! Pointeur local sur l'affichage
    Viewer * localAffiche;
	

//! Liste contenant les indices des exos de newExosList qui contiennent un ou des fichiers metapost
	QList<int> containsMpFiles;

	void launchMpthread();
};

#endif
