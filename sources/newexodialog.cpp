#include "newexodialog.h"
#include "ui_newexodialog.h"
#include <QFileInfo>

NewExoDialog::NewExoDialog(QWidget *parent,QString fileName,QStringList metas) :
    QDialog(parent),
    ui(new Ui::NewExoDialog)
{
    ui->setupUi(this);
    metas_m = metas;
    fileName_m = fileName;
    setWindowTitle(QFileInfo(fileName).fileName());
}

NewExoDialog::~NewExoDialog()
{
    delete ui;
}
