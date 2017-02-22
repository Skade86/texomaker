#include "generalsettingsdialog.h"
#include "preferences.h"

GeneralSettingsDialog::GeneralSettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    editorGroup = new QButtonGroup(programGroupBox);
    encodingGroup = new QButtonGroup(programGroupBox);

    editorGroup->addButton(editorDefaultRadio);
    editorGroup->addButton(editorInternalRadio);
    encodingGroup->addButton(isoRadio);
    encodingGroup->addButton(utf8Radio);
    encodingGroup->setExclusive(true);
    editorGroup->setExclusive(true);


        if (Preferences::p_getOpenAtLaunch()==true) {
        launchRadio->setChecked(true); }
    else launchRadio->setChecked(false);
	
        if (Preferences::p_getUseIso()==true) {
        isoRadio->setChecked(true);
        utf8Radio->setChecked(false);}
    else {
            isoRadio->setChecked(false);
            utf8Radio->setChecked(true);
        }

        if (Preferences::p_getInternEditor()==true) {
        editorInternalRadio->setChecked(true);
        editorDefaultRadio->setChecked(false);}
    else {
            editorInternalRadio->setChecked(false);
            editorDefaultRadio->setChecked(true);
        }

	testBin("pdflatex");
    testBin("latex");
    testBin("lualatex");
    testBin("dvips");
    testBin("ps2pdf");
    testBin("mpost");

    pdflatexEdit->setText(Preferences::p_getBin("pdflatex"));
    if (QFile(Preferences::p_getBin("pdflatex")).exists()) pdflatexCheck->setCheckState(Qt::Checked);
    latexEdit->setText(Preferences::p_getBin("latex"));
    if (QFile(Preferences::p_getBin("latex")).exists()) latexCheck->setCheckState(Qt::Checked);
    lualatexEdit->setText(Preferences::p_getBin("lualatex"));
    if (QFile(Preferences::p_getBin("lualatex")).exists()) lualatexCheck->setCheckState(Qt::Checked);
    dvipsEdit->setText(Preferences::p_getBin("dvips"));
    if (QFile(Preferences::p_getBin("dvips")).exists()) dvipsCheck->setCheckState(Qt::Checked);
    ps2pdfEdit->setText(Preferences::p_getBin("ps2pdf"));
    if (QFile(Preferences::p_getBin("ps2pdf")).exists()) ps2pdfCheck->setCheckState(Qt::Checked);
    mpostEdit->setText(Preferences::p_getBin("mpost"));
    if (QFile(Preferences::p_getBin("mpost")).exists()) mpostCheck->setCheckState(Qt::Checked);
}

void GeneralSettingsDialog::testBin(QString bin)
{
    if ((Preferences::p_getBin(bin).isEmpty()) || (!QFile(Preferences::p_getBin(bin)).exists()))
    {
        QString binPath = QStandardPaths::findExecutable(bin);
        if (!binPath.isEmpty()) Preferences::p_setBin(bin,binPath);

    }
}

void GeneralSettingsDialog::on_pdflatexBrowser_clicked()
{
    QString changedBin =
        QFileDialog::getOpenFileName(this, tr("Choose file"));
    if (!changedBin.isEmpty()) pdflatexEdit->setText(changedBin);
}

void GeneralSettingsDialog::on_latexBrowser_clicked()
{
    QString changedBin =
        QFileDialog::getOpenFileName(this, tr("Choose file"));
    if (!changedBin.isEmpty()) latexEdit->setText(changedBin);
}

void GeneralSettingsDialog::on_lualatexBrowser_clicked()
{
    QString changedBin =
        QFileDialog::getOpenFileName(this, tr("Choose file"));
    if (!changedBin.isEmpty()) lualatexEdit->setText(changedBin);
}

void GeneralSettingsDialog::on_dvipsBrowser_clicked()
{
    QString changedBin =
        QFileDialog::getOpenFileName(this, tr("Choose file"));
    if (!changedBin.isEmpty()) dvipsEdit->setText(changedBin);
}

void GeneralSettingsDialog::on_ps2pdfBrowser_clicked()
{
    QString changedBin =
        QFileDialog::getOpenFileName(this, tr("Choose file"));
    if (!changedBin.isEmpty()) ps2pdfEdit->setText(changedBin);
}

void GeneralSettingsDialog::on_mpostBrowser_clicked()
{
    QString changedBin =
        QFileDialog::getOpenFileName(this, tr("Choose file"));
    if (!changedBin.isEmpty()) mpostEdit->setText(changedBin);
}

void GeneralSettingsDialog::on_okButton_clicked()
{

    Preferences::p_setBin("pdflatex",pdflatexEdit->text());
    Preferences::p_setBin("latex",latexEdit->text());
    Preferences::p_setBin("lualatex",lualatexEdit->text());
    Preferences::p_setBin("dvips",dvipsEdit->text());
    Preferences::p_setBin("ps2pdf",ps2pdfEdit->text());
    Preferences::p_setBin("mpost",mpostEdit->text());

    if (launchRadio->isChecked())
    {
        Preferences::p_setOpenAtLaunch(true);
    }
    else Preferences::p_setOpenAtLaunch(false);

    if (isoRadio->isChecked())
    {
        Preferences::p_setUseIso(true);
    }
    else Preferences::p_setUseIso(false);

    if (editorDefaultRadio->isChecked())
    {
        Preferences::p_setInternEditor(false);
    }
    else Preferences::p_setInternEditor(true);

    close();
}
