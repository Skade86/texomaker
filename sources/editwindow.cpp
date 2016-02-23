#include "preferences.h"
#include "editwindow.h"
#include "ui_editwindow.h"

EditWindow::EditWindow(QWidget *parent,QString m_filePath) :
    QMainWindow(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    createActions();
    createToolBar();
    filePath = m_filePath;
    readFile();
}

void EditWindow::createActions()
{
    saveAct = new QAction(QIcon(":/images/save.png"),tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save current file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(fileSave()));

    updateAct = new QAction(QIcon(":/images/update.png"),tr("&Update"), this);
    updateAct->setShortcut(tr("Ctrl+U"));
    updateAct->setStatusTip(tr("Update current file in database"));
    connect(updateAct, SIGNAL(triggered()), this, SLOT(fileUpdate()));
}

void EditWindow::createToolBar()
{
    toolBar = addToolBar(tr("Tools"));
    toolBar->addAction(saveAct);
    toolBar->addAction(updateAct);
}

void EditWindow::readFile()
{
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("Application"),
                            tr("Cannot read file %1:\n%2.")
                             .arg(filePath)
                             .arg(file.errorString()));
        return;
    }
    // Lecture de ce qu'il y a dans le fichier courant
    QTextStream in(&file); // Création du flux
    in.flush();
    if (Preferences::p_getUseIso()) in.setCodec("ISO 8859-15");
    else
    {
        in.setCodec("UTF-8");
    }
    while (!in.atEnd())
        {
            QString line = in.readLine();
            ui->textEdit->append(line);
        }
}

bool EditWindow::fileSave()
{
    QTextDocumentWriter writer(filePath);
    bool success = writer.write(ui->textEdit->document());
    if (success)
        ui->textEdit->document()->setModified(false);
    qDebug() << success;
    return success;
}

bool EditWindow::fileUpdate()
{

}

EditWindow::~EditWindow()
{
    delete ui;
}
