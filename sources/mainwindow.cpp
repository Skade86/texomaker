#include <QtWidgets>
#include <QFont>
#include <QtNetwork>
#include <QtXml>

#include "mainwindow.h"
#include "tableview.h"
#include "dirprefdialog.h"
#include "generalsettingsdialog.h"
#include "importer.h"
#include "xmldomhandler.h"
#include "affichedialog.h"
#include "exosmodel.h"
#include "exosheet.h"
#include "preferences.h"
#include "propertydialog.h"
#include "zipthread.h"
#include "unzipthread.h"
#include "version.h"
#include "advancedfilter.h"

MainWindow::MainWindow()
{	
    // Création des actions, menus, barre de statut, barre d'outil ...
    createActions();
    createToolBars();
    createMenus();
    createFilter();
    createAffiche();
    createLayout();

    // Récupération des préférences
    readSettings();

    // Dimensionnement de la fenêtre principale
    QDesktopWidget desktop;
    QRect screenRect=desktop.screenGeometry(); // Récupération de la taille de l'écran
    resize(0.85*screenRect.width(),0.85*screenRect.height());
    move(QPoint(0.05*screenRect.width(),0.05*screenRect.height()));

    checkUpdate();

    // Attribution de l'emplacement du script ltx2pdf
    setLtxPath();

    // Initialisation de 2 booléens
    mvExists = false;	// pas de système modèle/vue encore
    creation = false;	// a priori il ne s'agit pas de la création d'une nouvelle DB

    // Tant qu'aucune base n'est chargée, on désactive les actions liées à une base
    settingsAct->setEnabled(false);
    addAct->setEnabled(false);
    propertiesAct->setEnabled(false);
    updateAct->setEnabled(false);
    removeExoAct->setEnabled(false);
    createSheetAct->setEnabled(false);

    // Si on trouve une DB valide récente (et que l'option "open at Launch" est activée)
    bool test;

    if ((test=loadLastDb()))
    {
        // Construction de l'arbre DOM
        exos = readXmlDom();

        // Vérification de l'intégrité de la base
        checkDB();

        // Lecture de l'historique des métadonnées et options
        readMetaDb();

        // Création du système de modèle / Vue
        createModelView();

        settingsAct->setEnabled(true);
        addAct->setEnabled(true);
        propertiesAct->setEnabled(true);
        updateAct->setEnabled(true);
        removeExoAct->setEnabled(true);
        createSheetAct->setEnabled(true);
        editExoAct->setEnabled(true);
    }

    createStatusBar(test);

    connect(simpleFilterLine,SIGNAL(textChanged ( const QString)),this,SLOT(applyFilter()));

}

void MainWindow::setLtxPath()
{
    QDir appDir = QDir::current();
    QString appDirName = appDir.dirName();
    QString curPath = appDir.path();

#ifdef Q_WS_MACX

    if (appDirName=="MacOS")
    {
        appDir.cdUp();
        if (appDir.dirName()=="Contents")
        {
            curPath = appDir.path();
            Preferences::p_setLtx2pdf(curPath+"/Resources/./ltx2pdf");
        }
    } else {
        Preferences::p_setLtx2pdf(curPath+"/TeXoMaker.app/Contents/Resources/./ltx2pdf");
    }
#endif

#ifndef Q_WS_MACX
#ifdef Q_WS_WIN
    Preferences::p_setLtx2pdf(curPath+"/ltx2pdf.bat");
#else
    Preferences::p_setLtx2pdf(curPath+"/./ltx2pdf");
#endif

#endif
}

void MainWindow::createFilter()
{
    simpleFilterLabel = new QLabel(tr("Filter :"));
    simpleFilterLine = new QLineEdit;
    advancedFilterButton = new QPushButton(tr("Advanced..."));
    connect(advancedFilterButton,SIGNAL(clicked()),this,SLOT(onAdvancedFilterClicked()));
}

void MainWindow::onAdvancedFilterClicked()
{
    advancedFilterWidget = new AdvancedFilter(this);
    addDockWidget(Qt::LeftDockWidgetArea,advancedFilterWidget);
    connect(advancedFilterWidget,SIGNAL(onFerme()),this,SLOT(onAdvancedClosed()));

    leftLayout->removeItem(topLeftLayout);
}

void MainWindow::onAdvancedClosed()
{
    leftLayout->insertLayout(0,topLeftLayout);
}

void MainWindow::createAffiche()
{
    afficheDialog = new AfficheDialog;
}

void MainWindow::createLayout()
{
    exosTableView = new TableView(this);
    exosTableView->setFont(QFont("Calibri", 12));
    connect(
        exosTableView->horizontalHeader(),
        SIGNAL(sectionResized(int, int, int)),
        exosTableView,
        SLOT(resizeRowsToContents()));
    groupBox = new QGroupBox;
    leftLayout = new QVBoxLayout;
    topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(simpleFilterLabel);
    topLeftLayout->addWidget(simpleFilterLine);
    topLeftLayout->addWidget(advancedFilterButton);
    simpleFilterLabel->setBuddy(simpleFilterLine);
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(exosTableView);
    groupBox->setLayout(leftLayout);

    leftSplitter  = new QSplitter(Qt::Horizontal);
    leftSplitter->addWidget(groupBox);
    leftSplitter->setStretchFactor(1,1);
    mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(leftSplitter);
    mainSplitter->addWidget(afficheDialog);
    mainSplitter->setStretchFactor(1,1);
    setCentralWidget(mainSplitter);
#ifdef Q_WS_MACX
    setUnifiedTitleAndToolBarOnMac(true);
#endif
    setWindowIcon(QIcon(":/images/TeXoMaker.png"));
    setFocusPolicy(Qt::StrongFocus);
}

/* 	Création de toutes les actions (qui seront par la suite attribuées
                dans les menus, barres d'outils ...
                A chaque fois : nom, icone, raccourci, connection au slot correspondant	*/
void MainWindow::createActions()
{	 
    addAct = new QAction(QIcon(":/images/ajouter.png"),tr("&Add an exercise"), this);
    addAct->setShortcut(tr("Ctrl+I"));
    addAct->setStatusTip(tr("Add new exercises"));
    connect(addAct, SIGNAL(triggered()), this, SLOT(import()));

    saveBaseAct = new QAction(QIcon(":/images/export.png"),tr("&Save base..."), this);
    saveBaseAct->setStatusTip(tr("Save current database in a single folder."));
    connect(saveBaseAct, SIGNAL(triggered()), this, SLOT(saveBase()));

    importBaseAct = new QAction(QIcon(":/images/import.png"),tr("&Import base..."), this);
    importBaseAct->setStatusTip(tr("Import a previously saved base"));
    connect(importBaseAct, SIGNAL(triggered()), this, SLOT(importBase()));

    propertiesAct = new QAction(QIcon(":/images/properties.png"),tr("&Properties of the exercise"),this);
    propertiesAct->setShortcut(tr("Ctrl+P"));
    propertiesAct->setStatusTip(tr("Edit current exercise properties"));
    connect(propertiesAct,SIGNAL(triggered()),this,SLOT(editProperties()));
    
    updateAct = new QAction(QIcon(":/images/update.png"),tr("&Update exercise"), this);
    updateAct->setShortcut(tr("Ctrl+U"));
    updateAct->setStatusTip(tr("Update current exercise"));
    connect(updateAct, SIGNAL(triggered()), this, SLOT(updateFile()));
    
    removeExoAct = new QAction(QIcon(":/images/del.png"), tr("&Remove exercises"), this);
    removeExoAct->setShortcut(tr("Ctrl+D"));
    removeExoAct->setStatusTip(tr("Remove exercises from database"));
    connect(removeExoAct , SIGNAL(triggered()), this, SLOT(suppressExo()));
    
    createSheetAct = new QAction(QIcon(":/images/new.png"), tr("&Create sheet.."), this);
    createSheetAct->setStatusTip(tr("Create a new exercises sheet"));
    connect(createSheetAct , SIGNAL(triggered()), this, SLOT(createSheet()));

    exitAct = new QAction(QIcon(":/images/quit.png"),tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    settingsAct = new QAction(QIcon(":/images/information.png"),tr("Current base settings"), this);
    settingsAct->setShortcut(tr("Ctrl+S"));
    settingsAct->setStatusTip(tr("Open the settings box"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(settings()));

    generalSettingsAct = new QAction(QIcon(":/images/settings.png"),tr("General Settings"), this);
    generalSettingsAct->setShortcut(tr("Ctrl+G"));
    generalSettingsAct->setStatusTip(tr("Open the général settings box"));
    generalSettingsAct->setMenuRole(QAction::PreferencesRole);
    connect(generalSettingsAct, SIGNAL(triggered()), this, SLOT(generalSettings()));

    editExoAct = new QAction(QIcon(":/images/edit.png"),tr("Edit exercise"), this);
    editExoAct->setShortcut(tr("Ctrl+E"));
    editExoAct->setStatusTip(tr("Edit the current exercise in an external editor."));
    connect(editExoAct, SIGNAL(triggered()), this, SLOT(editExo()));

    loadDbAct = new QAction(QIcon(":/images/load.png"),tr("&Load datafile"), this);
    loadDbAct->setStatusTip(tr("Load an existing TeXoMaker datafile"));
    connect(loadDbAct, SIGNAL(triggered()), this, SLOT(loadNewDb()));

    createDbAct = new QAction(QIcon(":/images/create.png"),tr("&Create datafile"), this);
    createDbAct->setStatusTip(tr("Create a new TeXoMaker datafile"));
    connect(createDbAct, SIGNAL(triggered()), this, SLOT(createDb()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    
    manualAct = new QAction(QIcon(":/images/help.png"),tr("&Manual"), this);
    manualAct->setStatusTip(tr("Launch the manual navigation"));
    connect(manualAct, SIGNAL(triggered()), this, SLOT(manualCalled()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


void MainWindow::createMenus()
{	
    fileMenu = menuBar()->addMenu(tr("Application"));
    fileMenu->addAction(loadDbAct);
    fileMenu->addAction(createDbAct);
    fileMenu->addAction(saveBaseAct);
    fileMenu->addAction(importBaseAct);
    fileMenu->addAction(generalSettingsAct);
    menuBar()->addSeparator();
    fileMenu->addAction(exitAct);

    toolMenu = menuBar()->addMenu(tr("&Base"));
    toolMenu->addAction(settingsAct);
    toolMenu->addAction(addAct);
    toolMenu->addAction(removeExoAct);
    toolMenu->addAction(updateAct);
    toolMenu->addAction(propertiesAct);
    toolMenu->addAction(editExoAct);
    menuBar()->addSeparator();
    toolMenu->addAction(createSheetAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(manualAct);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("Tools"));
    fileToolBar->addAction(addAct);
    fileToolBar->addAction(removeExoAct);
    fileToolBar->addAction(updateAct);
    fileToolBar->addAction(createSheetAct);
    fileToolBar->addAction(propertiesAct);
    fileToolBar->addAction(editExoAct);
    fileToolBar->addAction(settingsAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(generalSettingsAct);
    fileToolBar->addAction(loadDbAct);
    fileToolBar->addAction(createDbAct);
    fileToolBar->addAction(saveBaseAct);
    fileToolBar->addAction(importBaseAct);
}

void MainWindow::createStatusBar(bool test)
{
    if (!test) {
        QString statusMsg = QString(tr("No database loaded"));
        statusLabel = new QLabel(statusMsg);
        statusBar()->addPermanentWidget(statusLabel);
    } else {
        int nbExo = model->rowCount(QModelIndex());
        QString statusMsg = QString(tr("%1 exercices dans la base").arg(nbExo));
        statusLabel = new QLabel(statusMsg);
        statusBar()->addPermanentWidget(statusLabel);
    }
}


void MainWindow::createModelView()
{	

    model = new ExosModel(exos,this);
    filterModel = new QSortFilterProxyModel(this);
    selectionModel = new QItemSelectionModel(filterModel);

    mvExists = true;

    filterModel->setDynamicSortFilter(true);
    exosTableView->setModel(filterModel);
    filterModel->setSourceModel(model);
    exosTableView->setSelectionModel(selectionModel);
    exosTableView->setAlternatingRowColors(true);
    exosTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    exosTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    exosTableView->horizontalHeader()->setSectionsClickable(true);
    exosTableView->horizontalHeader()->setSortIndicatorShown(true);
    exosTableView->horizontalHeader()->setStretchLastSection (true);

    filterModel->setSortCaseSensitivity ( Qt::CaseInsensitive );
    filterModel->setSortLocaleAware ( true);
    filterModel->setFilterKeyColumn(-1);
    QModelIndex rootIndex = filterModel->index(1, 0, QModelIndex());

    exosTableView->setSortingEnabled(true);
    filterModel->sort(2,Qt::AscendingOrder);

    QModelIndex curIndex = filterModel->index(0, 0, QModelIndex());

    exosTableView->setCurrentIndex(curIndex);

    selectionModel->select(curIndex,QItemSelectionModel::Select);

    QStringList metaList = Preferences::p_getMetaList();
    QStringList metaView = Preferences::p_getMetaToView();

    for (int i = metaView.size(); i<metaList.size();i++)
    {
        exosTableView->setColumnHidden(i,true);
    }

    // Connections signaux / slot

    connect(selectionModel,SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)),this,SLOT(activateExoView()));
    connect(exosTableView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(activateExoView()));

    setWindowTitle(QString(tr("Database name : %1").arg(QFileInfo(Preferences::p_getDbfile()).baseName())));

    exosTableView->setFocus();
    activateExoView();
}

void MainWindow::updateStatusBar()
{
    int nbExo = model->rowCount(QModelIndex());
//    QMessageBox::warning(this, QObject::tr("Warning"),QObject::tr("%1").arg(nbExo));
    QString statusMsg = QString(tr("%1 exercices dans la base").arg(nbExo));
    statusLabel->setText(statusMsg);
}

void MainWindow::checkUpdate()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(httpDone(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("texomaker.les-domlols.com/version.txt")));
}

void MainWindow::httpDone(QNetworkReply* reply)
{
  /*  QString lastVersion = http->readAll().trimmed();

    if ((lastVersion!=Preferences::curVersion)&&(!error))
        QMessageBox::information(this, QObject::tr("New version of TeXoMaker"),
                                 QObject::tr("<center>Version %1 of TeXoMaker is available.<br><br>"
                                             "You can download it on the <br><br><a href='http://texomaker.les-domlols.com/index.php/downloads'>TeXoMaker website"
                                             "</a><center>").arg(lastVersion));
*/
}


void MainWindow::suppressExo(bool update)
{
    QModelIndexList curIndexList = selectionModel->selectedRows();
    QModelIndex curIndex;
    int curRow;
    int exosCount = curIndexList.count();
//    QMessageBox::warning(this, QObject::tr("Error"),QObject::tr("selection : %1").arg(exosCount));

    if (!update) {
        QMessageBox msgBox;
        if (exosCount>1) { msgBox.setText(tr("%1 exercises are going to be removed from the database !").arg(exosCount));
        } else { msgBox.setText(tr("%1 exercise is going to be removed from the database !").arg(exosCount));
        }
        msgBox.setInformativeText(tr("Do you confirm this action ?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if (ret==QMessageBox::Cancel) return;
    }

   // QModelIndex curIndex = exosTableView->currentIndex();

    if (model->rowCount(QModelIndex()) == 0) return;

    QStringList idList,nomList;
    QStringList fileCars;

    // Fabrication de deux listes contenant l'id et le nom des exos à supprimer.
    for (int i=0;i<exosCount;i++) {

        curIndex = curIndexList.at(i);
        curRow = curIndex.row();
        if ((curRow<0)&&(!update)) {
            QMessageBox::warning(this, QObject::tr("Error"),QObject::tr("Please select at least an exercise to remove !"));
            return;
        }
        QModelIndex fileIndex = filterModel->index(curRow,0,QModelIndex());

        idList << Preferences::p_getMetaToView().at(0);
        nomList << fileIndex.data().toString();
    }

    // Puis on supprime les exos dans la base et dans le modèle
     for (int i=0;i<exosCount;i++) {
         fileCars=QStringList();
         fileCars << idList.at(i) << nomList.at(i);

        int id = domHandler->removeExoDom(fileCars);
        model->removeRows(id,1,QModelIndex());
        updateStatusBar();
        if (model->rowCount(QModelIndex())==0) {
            afficheDialog->loadImageFile(QString());
            return;
        }
    }

    curIndexList = selectionModel->selectedRows();
    exosCount = curIndexList.count();

    QModelIndex nextIndex;

    if (curRow==0) nextIndex = filterModel->index(0, 0, QModelIndex());
    else nextIndex = filterModel->index(curRow-1, 0, QModelIndex());

    exosTableView->setCurrentIndex(nextIndex);
    selectionModel->select(nextIndex,QItemSelectionModel::Select);
    exosTableView->setFocus();
}

QString MainWindow::getCurrentPath()
{
    QModelIndex curIndex = exosTableView->currentIndex();
    int curRow = curIndex.row();

    // Recherche de la colonne du header contenant "filepath"
    int pathCol=0;
    QString header;

    for (int i=0;i<Preferences::p_getMetaSize();i++)
    {
        header=model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
        if (header=="filepath") pathCol=i;
    }

    QModelIndex fileIndex = filterModel->index(curRow, pathCol, QModelIndex());
    QString curFilePath = fileIndex.data().toString();

    return curFilePath;
}

void MainWindow::activateExoView()
{	
    QString curFilePath = getCurrentPath();

    if (!curFilePath.isEmpty())
    {
        afficheDialog->loadImageFile(curFilePath);
    }
    //    else afficheDialog->imageLabel->setText(tr("Click on + to import new exercises"));



    QStringList metaList = Preferences::p_getMetaList();
    QStringList metaView = Preferences::p_getMetaToView();

    for (int i = metaView.size(); i<metaList.size();i++)
    {
        exosTableView->setColumnHidden(i,true);
    }
}

void MainWindow::editProperties()
{
    if (Preferences::p_getDbfile().isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Warning"),QObject::tr("Please load a database first !"));
        return;
    }
    propertyDialog = new PropertyDialog(this,domHandler,getCurrentPath(),Preferences::p_getUseIso());
    connect(propertyDialog,SIGNAL(modifWritten()),this,SLOT(updateFile()));
    propertyDialog->show();
    propertyDialog->activateWindow();
}

void MainWindow::import(QStringList files2import)
{
    QString curImportDir = QDir::homePath();
    if (!currentImportPath.isEmpty()) curImportDir = currentImportPath;
    QStringList files = files2import;

    if (files.isEmpty()) {
        files = QFileDialog::getOpenFileNames(
                this,
                "Select one or more TeX files to import",
                curImportDir,
                "TeX files (*.tex)");
        if (files.isEmpty()) return;
    }
    currentImportPath = QFileInfo(files.at(0)).absolutePath();
    importObject = new Importer(files,Preferences::p_getUseIso(),&xmlDom,model,afficheDialog,false);
    connect(importObject,SIGNAL(fileImported(const QString &)),this,SLOT(updateImported(const QString &)));
    importObject->importFiles();
}

void MainWindow::updateImported(const QString & fileName)
{
    afficheDialog->loadImageFile(fileName);

    // Recherche de la colonne du header contenant "filepath"
    int pathCol=0;
    QString header;

    for (int i=0;i<Preferences::p_getMetaSize();i++)
    {
        header=model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
        if (header=="filepath") pathCol=i;
    }

    QModelIndex fileIndex = filterModel->index(0, pathCol, QModelIndex());
    QModelIndexList indexList = filterModel->match(fileIndex,0,fileName);
    QModelIndex newIndex = indexList.at(0);

    exosTableView->setCurrentIndex(newIndex);
    selectionModel->select(newIndex,QItemSelectionModel::Select);
    exosTableView->setFocus();
    updateStatusBar();
}

void MainWindow::updateFile()
{
    QStringList files = QStringList();
    QModelIndex curIndex = exosTableView->currentIndex();
    int curRow = curIndex.row();

    // Recherche de la colonne du header contenant "filepath"
    int pathCol=0;
    QString header;

    for (int i=0;i<Preferences::p_getMetaSize();i++)
    {
        header=model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
        if (header=="filepath") pathCol=i;
    }

    QModelIndex fileIndex = filterModel->index(curRow, pathCol, QModelIndex());
    QString curFilePath = fileIndex.data().toString();

    if  (!curFilePath.isEmpty())
    {
        //        afficheDialog->curImage=QImage();
        suppressExo(true);
        files << curFilePath;
        importObject = new Importer(files,Preferences::p_getUseIso(),&xmlDom,model,afficheDialog,false);
        connect(importObject,SIGNAL(fileImported(const QString &)),this,SLOT(updateImported(const QString &)));
        importObject->importFiles();
        return;
    }
    QMessageBox::warning(this, QObject::tr("Error"),QObject::tr("Please select a file to update !"));
}

void MainWindow::generalSettings()
{
    generalSettingsDialog = new GeneralSettingsDialog(this);

 //   connect(dirprefDialog,SIGNAL(aboutToClose()),this,SLOT(setDbCfg()));

    generalSettingsDialog->show();
    generalSettingsDialog->activateWindow();
}

void MainWindow::settings()
{	
    dirprefDialog= new DirPrefDialog(this,domHandler,model->rowCount(QModelIndex()));

    connect(dirprefDialog,SIGNAL(aboutToClose()),this,SLOT(setDbCfg()));

    dirprefDialog->show();
    dirprefDialog->activateWindow();
}


void MainWindow::createSheet()
{	
    createSheetAct->setEnabled(false);
    exoSheet = new ExoSheet(this,Preferences::p_getUseIso(),domHandler);
    connect(exoSheet->closeButton,SIGNAL(clicked()),this,SLOT(onExoSheetClose()));
    connect(exoSheet,SIGNAL(pdflatexFinished(const QString &,const QString &))
            ,this,SLOT(sheetCreated(const QString &,const QString &)));

    mainSplitter->addWidget(exoSheet);
    afficheDialog->scaleComboBox->setCurrentIndex(1);
    exoSheet->nameEdit->setFocus();
    connect(exosTableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(addExoSheet(const QModelIndex &)));
}

void MainWindow::onExoSheetClose()
{
    exoSheet->close();
    disconnect(exosTableView,SIGNAL(doubleClicked(const QModelIndex &)),0,0);
    createSheetAct->setEnabled(true);
    afficheDialog->update();
    afficheDialog->scaleComboBox->setCurrentIndex(3);
    activateExoView();
}

void MainWindow::sheetCreated(const QString &pdfFile,const QString &output)
{
    // On signale que cette variable n'est, pour le moment pas utilisée pour éviter un warning �  la compilation
    Q_UNUSED(output);

    QString urlStr = "file:///" + pdfFile;
    QDesktopServices::openUrl(QUrl(urlStr));
}

void MainWindow::addExoSheet(const QModelIndex & index)
{
    QStringList exoList=QStringList();
    int curRow = index.row();
    QModelIndex fileIndex;

    int pathInd=0;
    int figInd=0;
    int metaInd=0;

    // Le titre (ou en tout cas la première colonne)
    fileIndex = filterModel->index(curRow, 0, QModelIndex());
    exoList <<  fileIndex.data().toString();

    // On détermine dans quelles colonnes sont les métadonnées "figure", "filepath" et "metapost"
    QString header;
    for (int i=0;i<Preferences::p_getMetaSize();i++)
    {
        header=model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
        if (header=="filepath") pathInd=i;
        if (header=="figure") figInd=i;
        if (header=="metapost") metaInd=i;
    }

    // L'emplacement du fichier
    fileIndex = filterModel->index(curRow, pathInd, QModelIndex());
    exoList <<  fileIndex.data().toString();

    // La liste des figures
    fileIndex = filterModel->index(curRow, figInd, QModelIndex());
    exoList <<  fileIndex.data().toString();

    // La liste des fichiers metapost
    fileIndex = filterModel->index(curRow, metaInd, QModelIndex());
    exoList <<  fileIndex.data().toString();

    exoSheet->addExo(exoList);
    exoList.clear();
}

void MainWindow::about()
{
    int version_major = MAJOR;
    int version_minor = MINOR;
    int version_build = BUILD;
    QString version_no = tr("%1.%2 build %3").arg(version_major).arg(version_minor).arg(version_build);
    QMessageBox::about(this, tr("About TeXoMaker"),
                       tr("<center>This is <b>TeXoMaker</b> version %3<br>"
                          "Builded : %1<br>"
                          "<b>TeXoMaker</b> has been developped by <br><a href='mailto:gcleon@les-domlols.com'>Gwena&euml;l Cl&eacute;on</a><br>using <b>Qt version %2</b><br></center>").arg(QDate::currentDate().toString("d MMMM yyyy")).arg(qVersion()).arg(version_no));
}

void MainWindow::manualCalled()
{
    QString urlStr = "http://texomaker.les-domlols.com/index.php/tutoriel";
    QDesktopServices::openUrl(QUrl(urlStr));
}

void MainWindow::editExo()
{
    QStringList files = QStringList();
    QModelIndex curIndex = exosTableView->currentIndex();
    int curRow = curIndex.row();

    // Recherche de la colonne du header contenant "filepath"
    int pathCol=0;
    QString header;

    for (int i=0;i<Preferences::p_getMetaSize();i++)
    {
        header=model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
        if (header=="filepath") pathCol=i;
    }

    QModelIndex fileIndex = filterModel->index(curRow, pathCol, QModelIndex());

    QString urlStr = "file:///"+fileIndex.data().toString();
    QDesktopServices::openUrl(QUrl(urlStr));
}

void MainWindow::createDb()
{

    creation = true;
    //******** Création du fichier xml **********//
            // On récupère le nom du fichier à  créer
    QString fileName = QFileDialog::getSaveFileName(this, tr("Choose a file name"),
                                                    QDir::homePath()+QDir::separator()+"untittled.xml","xml Files (*.xml)");

    if (!Preferences::p_getDbfile().isEmpty()) {
        afficheDialog->loadImageFile(QString());
        statusLabel->setText(tr("Aucun exercice dans la base"));
    }


    // On le crée (en ouvrant un fichier qui n'existe pas cela le crée)
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,tr("Error"),
                             tr("Cannot create file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    // On crée la chaîne à  écrire dans le fichier
    QString emptyDbTxt = "<?xml version='1.0'?>\n";
    emptyDbTxt += "<!-- TeXoMaker database file (don't remove this line) -->\n";
    emptyDbTxt = emptyDbTxt + "<exercices dbsize=\"0\">\n"+"</exercices>";

    // On ouvre un flux
    QTextStream out(&file);
    out.flush();
    // On écrit dans le fichier et on le ferme
    out << emptyDbTxt;
    file.close();

    //! On assigne enfin le datafile au système MV
    if (testDbFile(fileName)) Preferences::p_setDbfile(fileName);

    file.close();

    // Construction de l'arbre DOM
    exos = readXmlDom();

    Preferences::p_setCompilationOptions("-interaction=nonstopmode");
    Preferences::p_setBeginDoc("\\begin{document}");
    Preferences::p_setEndDoc("\\end{document}");

    writeMetaDb();

    creation = false;

    DirPrefDialog *metaPref= new DirPrefDialog(this,0,0,true);

    connect(metaPref,SIGNAL(aboutToClose()),this,SLOT(setDbCfg()));

    metaPref->show();
    metaPref->activateWindow();
}

void MainWindow::setDbCfg()
{
    writeMetaDb();
    initializeModelView();
}

void MainWindow::loadNewDb(QString dbFile)
{
    creation = false;
    QString curDBDir = QDir::homePath();
    if (!currentDBPath.isEmpty()) curDBDir = currentDBPath;
    QStringList fileList;
    QString fileName;

    if (dbFile.isEmpty())
    {
        fileName = QFileDialog::getOpenFileName(this, tr("Open TeXoMaker database file"),curDBDir,tr("TeXoMaker files (*.xml)"));
        if (fileName.isEmpty()) return;
    }
    else fileName=dbFile;

    afficheDialog->loadImageFile(QString());
    currentDBPath=QFileInfo(fileName).absolutePath();

    if (testDbFile(fileName))
    {
        Preferences::p_setDbfile(fileName);

        initializeModelView();

       updateStatusBar();
    }
}

bool MainWindow::testDbFile(QString name)
{
    // Vérification qu'il s'agit bien d'un fichier de TeXoMaker
    QFile file(name);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        if (!name.isEmpty()) QMessageBox::warning(this,tr("Error"),
                                                  tr("Cannot read file %1:\n%2.")
            .arg(name)
            .arg(file.errorString()));
        return false;
    }

    // Lecture de ce qu'il y a dans le fichier courant
    QTextStream in(&file); // Création du flux
    in.flush();	
    QString line = in.readLine();
    line = in.readLine();
    file.close();

    if (line.trimmed()=="<!-- TeXoMaker database file (don't remove this line) -->")
    {
        return true;
    }
    else return false;
}

void MainWindow::initializeModelView()
{
    if (mvExists)
    {
        model->deleteLater();
        filterModel->deleteLater();
        selectionModel->deleteLater();
        exos.clear();
    }

    // Construction de l'arbre DOM
    exos = readXmlDom();

    // Vérification de l'intégrité de la base
    checkDB();

    // Lecture des types de metadonnées
    readMetaDb();

    // Création du système de modèle
    createModelView();

    settingsAct->setEnabled(true);
    addAct->setEnabled(true);
    propertiesAct->setEnabled(true);
    updateAct->setEnabled(true);
    removeExoAct->setEnabled(true);
    createSheetAct->setEnabled(true);
    editExoAct->setEnabled(true);

    return;
}

bool MainWindow::loadLastDb()
{
    //	Récupère des valeurs de variables enregistrés à  chaque fermeture du programme
    QSettings settings("Domlol","TeXoMaker");

    Preferences::p_setDbfile(settings.value("dbfile").toString());

    if ((Preferences::p_getDbfile().isEmpty()) || (!QFileInfo(Preferences::p_getDbfile()).exists()))
    {
        return false;
    }

    return true;
}

void MainWindow::readSettings()
{	
    //	Récupère des valeurs de variables enregistrés à  chaque fermeture du programme
    QSettings settings("Domlol","TeXoMaker");
    // Position et taille de la fenêtre
//    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
//    QSize size = settings.value("size", QSize(400, 400)).toSize();
 //   QRect geom = settings.value("geom",QRect()).toRect();
    QSize AffSize = settings.value("AfficheSize").toSize();
    QByteArray state = settings.value("state",QByteArray()).toByteArray();


    currentImportPath = settings.value("currentImportPath").toString();
    currentDBPath = settings.value("currentDBPath").toString();
    Preferences::p_setOpenAtLaunch(settings.value("openAtLaunch").toBool());
    Preferences::p_setBin("pdflatex",settings.value("pdflatex").toString());
    Preferences::p_setBin("latex",settings.value("latex").toString());
    Preferences::p_setBin("dvips",settings.value("dvips").toString());
    Preferences::p_setBin("mpost",settings.value("mpost").toString());
    Preferences::p_setBin("ps2pdf",settings.value("ps2pdf").toString());

    // On applique ces valeurs à  la nouvelle fenêtre
    //resize(size);
    //afficheDialog->resize(AffSize);
 //   move(pos);
 //   setGeometry(geom);
    restoreState(state);
    mainSplitter->restoreState(settings.value("mainSplitter").toByteArray());
    leftSplitter->restoreState(
            settings.value("leftSplitter").toByteArray());
    Preferences::p_setUseIso(settings.value("useIso").toBool());
}

void MainWindow::writeSettings()
{	
    if (!Preferences::p_getOpenAtLaunch()) Preferences::p_setDbfile(QString());

    // Ecriture des réglages à  la fermeture du programme
    QSettings settings("Domlol", "TeXoMaker");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("geom",geometry());
    settings.setValue("state",saveState());
    settings.setValue("AfficheSize", afficheDialog->size());
    settings.setValue("mainSplitter", mainSplitter->saveState());
    settings.setValue("leftSplitter", leftSplitter->saveState());
    settings.setValue("openAtLaunch",Preferences::p_getOpenAtLaunch());
    settings.setValue("dbfile",Preferences::p_getDbfile());
    settings.setValue("useIso",Preferences::p_getUseIso());
    settings.setValue("currentImportPath",currentImportPath);
    settings.setValue("currentDBPath",currentDBPath);
    settings.setValue("pdflatex",Preferences::p_getBin("pdflatex"));
    settings.setValue("latex",Preferences::p_getBin("latex"));
    settings.setValue("dvips",Preferences::p_getBin("dvips"));
    settings.setValue("mpost",Preferences::p_getBin("mpost"));
    settings.setValue("ps2pdf",Preferences::p_getBin("ps2pdf"));
}

int MainWindow::upgradeBase()
{
    // Fenêtre prévenant que l'on va mettre à  jour la base
     QMessageBox msgBox;
     msgBox.setText(tr("The exercise base will be upgraded to 2.0 format et won't be able"
                                   " to be read by previous versions of TeXoMaker."));
     msgBox.setInformativeText(tr("Do you confirm this action ?"));
     msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
     msgBox.setDefaultButton(QMessageBox::Ok);
     int ret = msgBox.exec();

     if (ret==QMessageBox::Cancel) {
         return ret;
     }


    // On lit les infos sur l'ancien fichier
    readOldMetaDb();
    // On les écrit sur le nouveau
    writeMetaDb();
    // On supprime l'ancien fichier de config
    QString cfgFileName = QFileInfo(Preferences::p_getDbfile()).absolutePath()+QDir::separator()+"."+QFileInfo(Preferences::p_getDbfile()).baseName();
    QFile oldCfg(cfgFileName);
    oldCfg.remove();

    return ret;
}

void MainWindow::readOldMetaDb()
{
    QString cfgFileName = QFileInfo(Preferences::p_getDbfile()).absolutePath()+QDir::separator()+"."+QFileInfo(Preferences::p_getDbfile()).baseName();
    QStringList metaList;
    QStringList metaView;

    QFile file(cfgFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        if (!cfgFileName.isEmpty()) QMessageBox::warning(this,tr("Error"),
                                                         tr("Cannot read file %1:\n%2.")
            .arg(cfgFileName)
            .arg(file.errorString()));
        return;
    }

    // Lecture de ce qu'il y a dans le fichier courant
    QTextStream in(&file); // Création du flux
    in.flush();

    QString tmp;
    QString tampon=QString();
    bool after = false;

    while (!in.atEnd())
    {
        tmp = in.readLine();

        if (tmp=="%@Preamble") break;

        if (tmp.trimmed() == "%@")
        {
            tmp = in.readLine();
            after = true;
        }

        if (after) { metaView << tmp; }
        else metaList << tmp;
    }
    Preferences::p_setMetaList(metaList);
    Preferences::p_setMetaToView(metaView);

    // Préambule
    while (!in.atEnd())
    {
        tmp=in.readLine();
        if (tmp=="%@MacroFiles") break;
        tampon+=tmp+"\n";
    }
    Preferences::p_setPreamble(tampon.trimmed());
    tampon.clear();

    // MacroFiles
    while (!in.atEnd())
    {
        tmp=in.readLine();
        if (tmp=="%@Compilation stuff") break;
        tampon+=tmp+"\n";
    }
    Preferences::p_setMacroFiles(tampon.trimmed());
    tampon.clear();

    // Compilation stuff
    tmp=in.readLine();
    if (tmp.trimmed().isEmpty()) tmp="pdflatex";
    Preferences::p_setCompiler(tmp);
    tmp=in.readLine();
    if (tmp.trimmed().isEmpty()) tmp="-halt-on-error";
    Preferences::p_setCompilationOptions(tmp);
    tmp=in.readLine();
    if (tmp.trimmed().isEmpty()) tmp="\\begin{document}";
    Preferences::p_setBeginDoc(tmp);
    tmp=in.readLine();
    if (tmp.trimmed().isEmpty()) tmp="\\end{document}";
    Preferences::p_setEndDoc(tmp);
    in.readLine();

    // Sheet header
    while (!in.atEnd())
    {
        tmp=in.readLine();
        if (tmp=="%@SheetExo") break;
        tampon+=tmp+"\n";
    }
    Preferences::p_setSheetHeader(tampon.trimmed());
    tampon.clear();

    // Sheet exo
    while (!in.atEnd())
    {
        tmp=in.readLine();
        if (tmp=="%@SheetBetween") break;
        tampon+=tmp+"\n";
    }
    Preferences::p_setSheetExo(tampon.trimmed());
    tampon.clear();

    // Sheet between
    while (!in.atEnd())
    {
        tmp=in.readLine();
        if (tmp=="%@SheetFooter") break;
        tampon+=tmp+"\n";
    }
    Preferences::p_setSheetBetween(tampon.trimmed());
    tampon.clear();

    // Sheet footer
    while (!in.atEnd())
    {
        tmp=in.readLine();
        if (tmp=="%@SheetBookmarks") break;
        tampon+=tmp+"\n";
    }
    Preferences::p_setSheetFooter(tampon.trimmed());

    // Sheet bookmarks
    QStringList tmpLst = QStringList();
    while (!in.atEnd())
    {
        tmp=in.readLine();
        tmpLst << tmp;
    }
    Preferences::p_setSheetBookmarks(tmpLst);

    file.close();

}

void MainWindow::readMetaDb()
{
    // Eventuel upgrade de la base xml
    QString cfgFileName = QFileInfo(Preferences::p_getDbfile()).absolutePath()+QDir::separator()+"."+QFileInfo(Preferences::p_getDbfile()).baseName();
    if (QFileInfo(cfgFileName).exists()) {
        int ret = upgradeBase();
        if (ret==QMessageBox::Cancel) close();
    }

    Preferences::p_setMetaList(domHandler->getSettings("MetadataList"));
    Preferences::p_setMetaToView(domHandler->getSettings("MetadataView"));
    Preferences::p_setPreamble(domHandler->getSettings("Preamble").at(0));
    Preferences::p_setMacroFiles(domHandler->getSettings("MacroFiles").at(0));
    Preferences::p_setCompiler(domHandler->getSettings("Compiler").at(0));
    Preferences::p_setCompilationOptions(domHandler->getSettings("CompilationOptions").at(0));
    Preferences::p_setBeginDoc(domHandler->getSettings("BeginDoc").at(0));
    Preferences::p_setEndDoc(domHandler->getSettings("EndDoc").at(0));
    Preferences::p_setSheetHeader(domHandler->getSettings("SheetHeader").at(0));
    Preferences::p_setSheetExo(domHandler->getSettings("SheetExo").at(0));
    Preferences::p_setSheetBetween(domHandler->getSettings("SheetBetween").at(0));
    Preferences::p_setSheetFooter(domHandler->getSettings("SheetFooter").at(0));
    Preferences::p_setSheetBookmarks(domHandler->getSettings("SheetBookmarks"));

    if (domHandler->settingsNodeExists("Dvips")) {
    if (Preferences::p_getBin("pdflatex").isEmpty()) Preferences::p_setBin("pdflatex",domHandler->getSettings("Pdflatex").at(0));
    if (Preferences::p_getBin("latex").isEmpty()) Preferences::p_setBin("latex",domHandler->getSettings("Latex").at(0));
    if (Preferences::p_getBin("dvips").isEmpty()) Preferences::p_setBin("dvips",domHandler->getSettings("Dvips").at(0));
    if (Preferences::p_getBin("mpost").isEmpty()) Preferences::p_setBin("mpost",domHandler->getSettings("Mpost").at(0));
    if (Preferences::p_getBin("ps2pdf").isEmpty()) Preferences::p_setBin("ps2pdf",domHandler->getSettings("Ps2pdf").at(0));
    }
}

void MainWindow::writeMetaDb()
{
    // Préparation de l'écriture de ce qui concerne les métadonnées
    QStringList metaCfg1;
    QStringList metaCfg2;
    QStringList strlst=QStringList();

    if (creation)
    {
        strlst << "Titre" << "Domaine"  << "Chapitre"  << "filepath" << "importdate" << "figure" <<"metapost";
        metaCfg1 = strlst;
        Preferences::p_setMetaList(strlst);

        strlst.clear();
        strlst << "Titre" << "Domaine"  << "Chapitre";
        metaCfg2 = strlst;
        Preferences::p_setMetaToView(strlst);
    }
    else
    {
        metaCfg1 = Preferences::p_getMetaList();
        metaCfg2 = Preferences::p_getMetaToView();
    }

    domHandler->removeSettings();

    domHandler->writeSettings("MetadataList",metaCfg1);
    domHandler->writeSettings("MetadataView",metaCfg2);

    // Préparation du reste des préférences

    domHandler->writeSettings("Preamble",QStringList(Preferences::p_getPreamble()));
    domHandler->writeSettings("MacroFiles",QStringList(Preferences::p_getMacroFiles()));
    domHandler->writeSettings("Compiler",QStringList(Preferences::p_getCompiler()));
    domHandler->writeSettings("CompilationOptions",QStringList(Preferences::p_getCompilationOptions()));
    domHandler->writeSettings("BeginDoc",QStringList(Preferences::p_getBeginDoc()));
    domHandler->writeSettings("EndDoc",QStringList(Preferences::p_getEndDoc()));
    domHandler->writeSettings("SheetHeader",QStringList(Preferences::p_getSheetHeader()));
    domHandler->writeSettings("SheetExo",QStringList(Preferences::p_getSheetExo()));
    domHandler->writeSettings("SheetBetween",QStringList(Preferences::p_getSheetBetween()));
    domHandler->writeSettings("SheetFooter",QStringList(Preferences::p_getSheetFooter()));
    domHandler->writeSettings("SheetBookmarks",Preferences::p_getSheetBookmarks());

    domHandler->saveInXml();

}

void MainWindow::closeEvent(QCloseEvent *event)
{	
    /*	Définition de l'événement de fermeture qui provoquera
                l'écriture des réglages		*/
//    if ((!currentDBPath.isEmpty())&&(loadLastDb())) {
//        QMessageBox::warning(this, QObject::tr("Error"),"write");
//        writeMetaDb();
//    }

    if ((!currentDBPath.isEmpty())&&(!Preferences::p_getDbfile().isEmpty())&&(QFileInfo(Preferences::p_getDbfile()).exists())) {
//        QMessageBox::warning(this, QObject::tr("Error"),"write");
        writeMetaDb();
    }

    writeSettings();
    event->accept();
}

void MainWindow::contextMenuEvent(QContextMenuEvent * e)
{
    QMenu *menu = new QMenu(this);
    QModelIndex index = exosTableView->indexAt(e->pos());
    if (index.isValid())
        menu->addAction(QString("Row %1 - Col %2 was clicked on").arg(index.row()).arg(index.column()));
    else
        menu->addAction("No item was clicked on");

    menu->exec(QCursor::pos());
}

QList<QMap<QString,QString> > MainWindow::readXmlDom()
{
    domHandler = new XmlDomHandler(&xmlDom);
    domHandler->createDomDocument();
    return domHandler->getExosList();
}

void MainWindow::applyFilter()
{
    QRegExp regExp(simpleFilterLine->text(),Qt::CaseInsensitive);
    filterModel->setFilterRegExp(regExp);
}

void MainWindow::checkDB()
{
    QStringList exoPbList = domHandler->checkXmlIntegrity();
    QString curPb;
    if (!exoPbList.isEmpty())
    {
        QString errorText = tr("The exercise database has the following problems : <ul>");
        QString exoFileName;

        for (int i = 0; i<exoPbList.size(); i++)
        {
            curPb = exoPbList.at(i);
            exoFileName = QFileInfo(curPb).baseName().replace("-preview",".tex");
            if (QFileInfo(curPb).suffix() == "pdf") errorText += QString(tr("<li>Exercise <b>%1</b> has no preview, please update the exercise by clicking the <i>Update the exercise</i> button.</li>")).arg(exoFileName);
            if (QFileInfo(curPb).suffix() == "tex") errorText += QString(tr("<li>Exercise <b>%1</b> has moved. Please put the latex file in the original directory, or remove it and reimport it elsewhere.</li>")).arg(exoFileName);

        }
        errorText += tr("</ul>");
        QMessageBox::warning(this, QObject::tr("Error"),errorText);
    }
}

// Les trois fonctions qui suivent servent à sauvegarder la base de donnée dans un répertoire unique
// qui contiendra tout les fichiers.

void MainWindow::saveBase()
{
    QString dbName = QString();
    dbName = Preferences::p_getDbfile();
    if (dbName.isEmpty()) return;

    dbName = QFileInfo(dbName).baseName();

    QMessageBox msgBox;
    msgBox.setText(tr("The current database will be saved. \n It means that all exercises (including figures) will be copied in a single directory.\n The XML database file will be also copied.\n This copy will be ready to be imported using the \"Import Base\" button."));
    msgBox.setInformativeText(tr("Click Ok to choose the directory where you want to save the database."));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if (ret==QMessageBox::Cancel) return;

    QString savePath;
    QString home = QDir::homePath();
    savePath = QFileDialog::getExistingDirectory(this, tr("Choose a directory"),
                                                    home,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (savePath.isEmpty()) return;

    QDir pathDir(savePath);
    pathDir.mkdir(dbName);
    QString zipFile=savePath+QDir::separator()+dbName+".tom";
    savePath=savePath+QDir::separator()+dbName;
    QFile::copy(Preferences::p_getDbfile(),savePath+QDir::separator()+dbName+".xml");
    QStringList exosFileList = createExosDataList();
    for (int i=0;i<exosFileList.size();i++) {
        QFile::copy(exosFileList.at(i),savePath+QDir::separator()+QFileInfo(exosFileList.at(i)).fileName());
    }
    removeExosNode(savePath+QDir::separator()+dbName+".xml");

    // On zippe le dossier en un fichier .tom
    ZipThread *zipThread= new ZipThread(zipFile,savePath,this);
    connect(zipThread,SIGNAL(finished()),zipThread,SLOT(deleteLater()));
    connect(zipThread,SIGNAL(zipCreated(const QString &)),this,SLOT(zipSuccess(const QString &)));
    zipThread->start();
}

void MainWindow::zipSuccess(const QString &dir)
{
   QDir oldDir = QDir(dir);
   oldDir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
   foreach(QFileInfo fileInfo, oldDir.entryInfoList())
   {
       QFile::remove(fileInfo.filePath());
   }
   oldDir.cdUp();
   oldDir.rmdir(dir);
   QMessageBox::information(this, QObject::tr("Information"),tr("Base successfully saved"));
}

QStringList MainWindow::createExosDataList()
{
    QList<QStringList> sqliteExosList;
    QList<QMap<QString,QString> > allExosList;
    QStringList filesToUploadList;
    QStringList currentExoList;
    QStringList exosMetasSqlite = QStringList();
    int exoNumber;

    for (int i=0; i<3; i++) {
        exosMetasSqlite << Preferences::p_getMetaToView().at(i);
    }

    exoNumber = domHandler->getCurrentDbsize();
    allExosList = domHandler->getExosList();

    for (int i=0; i<exoNumber; i++) {
        QString completeFilePath = QFileInfo(allExosList.at(i).value("filepath")).absolutePath();
        QString fileName = QFileInfo(allExosList.at(i).value("filepath")).fileName();
        filesToUploadList << completeFilePath + QDir::separator() + fileName;
        currentExoList << fileName;
        QStringList figList = domHandler->getFigList(allExosList.at(i).value("filepath"));
        figList.removeFirst();
        for (int j=0;j<figList.size();j++) {
            filesToUploadList << completeFilePath + QDir::separator() + figList.at(j);
        }
        currentExoList << figList.join("; ");
        for (int j=0; j<3; j++) currentExoList << allExosList.at(i).value(exosMetasSqlite.at(j));
        sqliteExosList.append(currentExoList);
        currentExoList.clear();
    }
    return filesToUploadList;
}

void MainWindow::removeExosNode(QString xmlFile)
{
    // Création de l'arbre
    QDomDocument doc("base-xml");
    QFile file(xmlFile);
    if (!file.open(QIODevice::ReadOnly))
         return;
    // Création de l'arbre
    if (!doc.setContent(&file)) {
         file.close();
         return;
     }
    file.close();

    // Suppression des exos dans le fichier xml
    QStringList texFileList = QStringList();

    QDomElement root = doc.firstChildElement("exercices");
    QDomElement exoChild = root.firstChildElement("exo");

    while (!exoChild.isNull())
    {
        QDomElement eltToRemove=exoChild;
        exoChild=exoChild.nextSiblingElement();
        if (eltToRemove.nodeName()=="exo")
        {
            QDomElement fileElt = eltToRemove.firstChildElement("filepath");
            QString path = fileElt.text();
            QString fileName = QFileInfo(path).fileName();
            texFileList << fileName; // On récupère au passage le nom du fichier tex
            root.removeChild(eltToRemove);
        }
    }

    root.setAttribute("dbsize",0);

    QFile fileOut(xmlFile);
    fileOut.open(QIODevice::WriteOnly);
    QTextStream out(&fileOut);
    doc.save(out,4,QDomNode::EncodingFromTextStream);

    // On sauvegarde la liste des fichiers tex dans un fichier texte
    QString listFile = QFileInfo(xmlFile).path()+QDir::separator()+"texlist.txt";

    QFile fileTex(listFile);
    fileTex.open((QIODevice::WriteOnly| QIODevice::Text));
    QTextStream flux(&fileTex);
    flux.setCodec("UTF-8");
    QString str;
    foreach(str,texFileList)
    {
        flux << str << endl;
    }
    fileTex.close();
}

// Fonctions qui gèrent l'importation d'une base précédemment exportée

void MainWindow::importBase()
{
    QString tomFile;
    QString home = QDir::homePath();
    tomFile = QFileDialog::getOpenFileName(this, tr("Choose a tom file"),
                                                    home,
                                       "Tom Files (*.tom)");
    if (tomFile.isEmpty()) return;
    //QMessageBox::warning(this, QObject::tr("Error"),tomFile);
    QString zipDir = QFileInfo(tomFile).absolutePath()+QDir::separator()+QFileInfo(tomFile).baseName();
    QDir dir(zipDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    // On dézippe
    UnzipThread *unzipThread= new UnzipThread(tomFile,zipDir,this);
    connect(unzipThread,SIGNAL(finished()),unzipThread,SLOT(deleteLater()));
    connect(unzipThread,SIGNAL(unzipCreated(const QString &)),this,SLOT(unzipSuccess(const QString &)));
    unzipThread->start();
}

void MainWindow::unzipSuccess(const QString &tomFile)
{
    // Récupération des chemins complet des fichiers xml et texlist.txt
    QDir tomDir=QDir(tomFile);
    QString baseFile=QFileInfo(tomFile+QDir::separator()+tomDir.dirName()).baseName();
    QString xmlFile=tomFile+QDir::separator()+baseFile+".xml";
    QString texFile=tomFile+QDir::separator()+"texlist.txt";

    // On charge la base
    loadNewDb(xmlFile);

    // Récupération de la liste des fichiers tex
    QStringList texFileList=QStringList();
    QFile fileTex(texFile);
    fileTex.open((QIODevice::ReadOnly| QIODevice::Text));
    QTextStream flux(&fileTex);
    flux.setCodec("UTF-8");
    while(!flux.atEnd())
    {
        texFileList << tomFile+QDir::separator()+flux.readLine();
    }
    fileTex.close();

    // On les importe dans la base
    import(texFileList);

//    QMessageBox::warning(this, QObject::tr("Error"),xmlFile);
}

void MainWindow::changeAllXmlPaths(QString xmlFile,QString newPath)
{
    QDomDocument doc("base-xml");
    QFile file(xmlFile);
    if (!file.open(QIODevice::ReadOnly))
         return;
    // Création de l'arbre
    if (!doc.setContent(&file)) {
         file.close();
         return;
     }
    file.close();

    // Remplacement des filePath par newPath
    QDomNodeList pathDomList = doc.elementsByTagName("exo");

    for (int i=0;i<pathDomList.length(); ++i)
    {
        QDomNode exoChild = pathDomList.item(i).firstChild();

        while(!exoChild.isNull()) {

            if (exoChild.nodeName()=="filepath") {
                // Récupération de l'ancien filepath
                QString curPath = exoChild.toElement().text();
                QString fileName = QFileInfo(curPath).fileName();
                QString newFile=newPath+QDir::separator()+fileName;

                // On le supprime
                QDomNode nodeToRemove = exoChild;
                pathDomList.item(i).removeChild(nodeToRemove);

                // On crée le nouveau
                QDomElement newFilepathElt = doc.createElement("filepath");
                QDomText newText = doc.createTextNode(newFile);
                pathDomList.item(i).appendChild(newFilepathElt);
                newFilepathElt.appendChild(newText);
                break;
            }
            exoChild = exoChild.nextSibling();
        }

    }
    // Enregistrement du fichier xml
    QFile fileOut(xmlFile);
    fileOut.open(QIODevice::WriteOnly);
    QTextStream out(&fileOut);
    doc.save(out,4,QDomNode::EncodingFromTextStream);
}
