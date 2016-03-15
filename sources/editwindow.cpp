#include "preferences.h"
#include "editwindow.h"
#include "highlighter.h"
#include "ui_editwindow.h"

EditWindow::EditWindow(QWidget *parent,QString m_filePath) :
    QMainWindow(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    filePath = m_filePath;
    setWindowTitle(tr("TeXoMaker editor - %1").arg(QFileInfo(filePath).fileName()));
    createActions();
    createToolBar();
    QFont textFont("Helvetica");
    textFont.setStyleHint(QFont::SansSerif);
    ui->textEdit->setFont(textFont);
    ui->textEdit->setTabStopWidth(20);
    highlighter = new Highlighter(ui->textEdit->document());
    readFile();
    highlighter->rehighlight();
}

void EditWindow::createActions()
{
    /*
    saveAct = new QAction(QIcon(":/images/save.png"),tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save current file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(fileSave()));
    */

    updateAct = new QAction(QIcon(":/images/update.png"),tr("&Update"), this);
    updateAct->setShortcut(tr("Ctrl+U"));
    updateAct->setStatusTip(tr("Update current file in database"));
    connect(updateAct, SIGNAL(triggered()), this, SLOT(fileUpdate()));
}

void EditWindow::createToolBar()
{
    toolBar = addToolBar(tr("Tools"));
    //toolBar->addAction(saveAct);
    toolBar->addAction(updateAct);
}

bool EditWindow::readFile()
{
    if (!QFile::exists(filePath))
            return false;
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly))
            return false;

        QByteArray data = file.readAll();
        QTextCodec *codec = Qt::codecForHtml(data);
        QString str = codec->toUnicode(data);
        if (Qt::mightBeRichText(str)) {
            ui->textEdit->setHtml(str);
        } else {
            str = QString::fromLocal8Bit(data);
            ui->textEdit->setPlainText(str);
        }

        return true;
}

bool EditWindow::maybeSave()
{
    if (!ui->textEdit->document()->isModified())
        return true;

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

bool EditWindow::fileSave()
{
    QTextDocumentWriter writer(filePath);
   // writer.setCodec("UTF-8");
    writer.setFormat("plaintext");
    bool success = writer.write(ui->textEdit->document());
    if (success)
        ui->textEdit->document()->setModified(false);
    return success;
}

bool EditWindow::fileUpdate()
{
    fileSave();
    emit updated();
}

void EditWindow::closeEvent(QCloseEvent *e)
{
    if (maybeSave())
        e->accept();
    else
        e->ignore();
}

EditWindow::~EditWindow()
{
    delete ui;
}
