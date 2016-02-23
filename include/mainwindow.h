#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml>

class QAction;
class QMenu;
class QTextEdit;
class QLabel;
class QLineEdit;
class QSplitter;
class QGroupBox;
class QVBoxLayout;
class QHBoxLayout;
class TableView;
class QSortFilterProxyModel;
class QItemSelectionModel;
class DirPrefDialog;
class GeneralSettingsDialog;
class Importer;
class XmlDomHandler;
class Viewer;
class ExosModel;
class ExoSheet;
class PropertyDialog;
class NewExoDialog;
class QNetworkReply;
class QPushButton;
class AdvancedFilter;
class EditWindow;

//! Classe de la fenêtre principale

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! Constructeur
    MainWindow();

public slots:

    //! Ecriture des préférences du programme (taille et positionnement de la fenêtre, ...
    void writeSettings();
    
protected:
    //!	Evénement de fermeture de fenêtre
    void closeEvent(QCloseEvent *event);

    void contextMenuEvent(QContextMenuEvent * e);

private slots:

    //! Importe des exercices
    /*!
        Slot appelé par la QAction addAct qui va créer un objet de la classe \a Importer, et lancer l'importation à l'aide
        de la méthode publique de cette classe, \a ImportFiles().
        */
    void import(QStringList list2import = QStringList());

    //! Slot appelé quand le widget de filtre avancé est fermé
    void onAdvancedClosed();

    //! Mets à jour l'exercice sélectionné
    void updateFile();

    //! Mets à jour la barre de statut
    void updateStatusBar();

    void updateImported(const QString & fileName);

    //! Boîte A propos ...
    void about();
    
    //! Affichage du manuel
    void manualCalled();

    //! Edition du fichier dans un éditeur externe
    void editExo();
    
    //! Affichage des réglages de la base.
    /*! \sa dirPrefDialog, DirPrefDialog */	
    void settings();

    //! Affichage des réglages généraux.
    void generalSettings();
    
    //! Fonction de filtre exécutée à chaque modification de lineEdit
    void applyFilter();
    
    //! Fonction exécutée qui affiche le pdf de l'exo courant
    void activateExoView();
    
    //! Suppression d'un exo
    void suppressExo(bool update=false);
    
    //! Création d'une feuille d'exercice
    void createSheet();
    
    //! Ajout d'un exo dans la feuille d'exercice
    void addExoSheet(const QModelIndex & index);
    
    //! Slot correspondant au signal de fermeture du Dialog ExoSheet
    void onExoSheetClose();
    
    //! Slot exécuté après une feuille d'exos créée avec succès
    void sheetCreated(const QString &pdfFile,const QString &output);

    //! Slot correspondant à la création d'une nouvelle base de données
    void createDb();

    //! Slot correspondant à l'action saveBaseAct : sauvegarde de la base
    void saveBase();

    //! Slot correspondant à l'action importBaseAct : importation de la base
    void importBase();

    //! Slot déclenché pour régler le fichier de config .<dbfile>
    void setDbCfg();

    //! Slot correspondant au chargement d'une nouvelle base de donnée
    void loadNewDb(QString dbFile=QString());

    //! Ecriture des types de metadonnées dans le fichier de config de la database
    void writeMetaDb();

    //! Lance l'éditeur des propriétés
    void editProperties();

    //! Vérifie si la version actuelle est la dernière version
    void checkUpdate();

    //! Appui sur les filtres avancés
    void onAdvancedFilterClicked();

    //! Met à jour le système MV (lors d'un changement de base par exemple)
    void initializeModelView();

     //! Créer un nouvel exercice
    void createExo();

    //! Open internal Editor
    void openEditor(QString m_filePath);

    void updateReplyFinished(QNetworkReply* reply);


private:

    //! Fenêtre d'affichage des préférences de la base
    /*!
         Objet de la classe \a DirPrefDialog. C'est la fenêtre de dialogue des préférences de la base courante.
         \sa DirPrefDialog
         */
    DirPrefDialog * dirprefDialog;

    //! Fenêtre d'affichage des préférences générales
    /*!
         Objet de la classe \a GeneralSettingsDialog. C'est la fenêtre de dialogue des préférences générales.
         \sa GeneralSettingsDialog
         */
    GeneralSettingsDialog * generalSettingsDialog;
    
    //! Classe d'importation
    /*!
         Objet de la classe \a Importer. Contient toutes les méthodes permettant d'importer des exercices dans la base de donnée.
         */
    Importer * importObject;
    
    //! Classe gérant les manipulations xml
    /*!
         Objet de la classe \a XmlDomHandler utilisé pour créer un arbre Dom à partir du fichier xml, et pour faire toutes les manipulations
         sur cet arbre et sur le fichier xml.
         */
    XmlDomHandler * domHandler;

    //! Classe pour le dialogue new exo

    NewExoDialog * newexodialog;
    
    //!  Arbre Dom du fichier XML de données
    QDomDocument xmlDom;
    
    //! Widget d'affichage des exercices
    /*!
         Objet de la classe \a AfficheDialog gérant l'affichages des aperçus des exercices
         */
    Viewer *viewer;

    PropertyDialog *propertyDialog;

    AdvancedFilter *advancedFilterWidget;

    EditWindow *editWindow;

    //! Vérification de la base au démarrage
    void checkDB();
    //!	Création des actions
    void createActions();
    //!	Création des menus
    void createMenus();
    //!	Création de la  barre d'outil
    void createToolBars();
    //!	Création de la barre de statut
    void createStatusBar(bool test);
    //! Méthode qui crée le système MV
    void createModelView();
    //! Méthode créant le système de layouts
    void createLayout();
    //! Création des éléments de la partie filtre
    void createFilter();
    //! Création des éléments de l'affichage
    void createAffiche();
    //! Attribution du chemin du script ltx2pdf (suivant environnement)
    void setLtxPath();
    //! Récupère le filepath de l'exo en cours de sélection
    QString getCurrentPath();
    //! Retourne une list des toutes les données d'exercices de la base
    QStringList createExosDataList();
    //! Change dans le fichier xml donné tout les chemins des exercices
    void changeAllXmlPaths(QString xmlFile,QString newPath=QString());
    //! Supprime les noeuds "exo" dans le fichier xml
    void removeExosNode(QString xmlFile);
    
    //! Pointeur sur l'objet de visualisation
    //! Cette vue est gérée suivant le principe Model / View. Le model gérant cette vue est du type \a ExosModel.
    TableView *exosTableView;

    QPushButton *advancedFilterButton;
    QLineEdit *simpleFilterLine;
    QLabel *simpleFilterLabel;
    QLabel *statusLabel;
    QSplitter *mainSplitter;
    QSplitter *leftSplitter;
    QGroupBox  *groupBox;
    QVBoxLayout *leftLayout;
    QHBoxLayout *topLeftLayout;
    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;

    QAction *pdflatexAct;
    QAction *addAct;
    QAction *updateAct;
    QAction *exitAct;
    QAction *settingsAct;
    QAction *generalSettingsAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *manualAct;
    QAction *useIsoAct;
    QAction *removeExoAct;
    QAction *createSheetAct;
    QAction *loadDbAct;
    QAction *createDbAct;
    QAction *propertiesAct;
    QAction *editExoAct;
    QAction *saveBaseAct;
    QAction *importBaseAct;
    QAction *checkUpdateAct;
    QAction *newExoAct;
    
    //! Lecture des préférences du programme (taille et positionnement de la fenêtre, ...
    void readSettings();

    //! Lecture des types de metadonnées dans le fichier de config de la database
    void readMetaDb();

    //! Chargement de la base de donnée en cours
    bool loadLastDb();

    //! Vérifie que name est bien un fichier de TeXoMaker
    bool testDbFile(QString name);
    
    //! Liste contenant à la suite toutes les données de tout les exercices
    /*!
    Cette liste est créée par la méthode \a getExosList de la classe \a XmlDomHandler. Celle-ci est
    appelée dans le constructeur de la fenêtre principale, une fois l'arbre Dom créé. Cette liste est le support des
    données du modèle \a ExosModel.
    */
    QList<QMap<QString,QString> > exos;
    
    //! Fonction qui crée l'arbre Dom et la liste \a exos.
    QList<QMap<QString,QString> > readXmlDom();

    //! Modèle utilisée par la vue \a exosTableView.
    ExosModel *model;
    
    //! Modèle de filtre
    QSortFilterProxyModel *filterModel;
    
    //! Modèle gérant les sélections
    QItemSelectionModel *selectionModel;
    
    //! Objet gérant la création d'un feuille d'exercice
    ExoSheet *exoSheet;

    //! Chemin du dernier exercice importé
    QString currentImportPath;

    //! Chemin de la dernière base de donnée ouverte
    QString currentDBPath;

    //! Booléen indiquant si le système MVC a été déjà créé
    bool mvExists;

    //! Chaîne contenant le chemin des exécutables latex sous Windows
    QString latexBinDir;

    bool creation;

};

#endif
