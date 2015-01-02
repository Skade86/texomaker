#include "generalsettingsdialog.h"
#include "preferences.h"

GeneralSettingsDialog::GeneralSettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

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

	testBin("pdflatex");
    testBin("latex");
    testBin("dvips");
    testBin("ps2pdf");
    testBin("mpost");

    pdflatexEdit->setText(Preferences::p_getBin("pdflatex"));
    if (QFile(Preferences::p_getBin("pdflatex")).exists()) pdflatexCheck->setCheckState(Qt::Checked);
    latexEdit->setText(Preferences::p_getBin("latex"));
    if (QFile(Preferences::p_getBin("latex")).exists()) latexCheck->setCheckState(Qt::Checked);
    dvipsEdit->setText(Preferences::p_getBin("dvips"));
    if (QFile(Preferences::p_getBin("dvips")).exists()) dvipsCheck->setCheckState(Qt::Checked);
    ps2pdfEdit->setText(Preferences::p_getBin("ps2pdf"));
    if (QFile(Preferences::p_getBin("ps2pdf")).exists()) ps2pdfCheck->setCheckState(Qt::Checked);
    mpostEdit->setText(Preferences::p_getBin("mpost"));
    if (QFile(Preferences::p_getBin("mpost")).exists()) mpostCheck->setCheckState(Qt::Checked);
}

void GeneralSettingsDialog::testBin(QString bin)
{
    if (Preferences::p_getBin(bin).isEmpty())
    {
#ifdef Q_OS_MAC
        if ((bin == "ps2pdf")&&(QFile(QString("/usr/local/bin/"+bin)).exists()))
        {
            Preferences::p_setBin(bin,"/usr/local/bin/"+bin);
        }
        else if (QFile(QString("/usr/texbin/"+bin)).exists()) {
            Preferences::p_setBin(bin,"/usr/texbin/"+bin);  }
#endif

#ifdef Q_OS_LINUX
        if (QFile(QString("/usr/bin/"+bin)).exists()) {
            Preferences::p_setBin(bin,"/usr/bin/"+bin); }
#endif

#ifdef Q_OS_WIN
        Preferences::p_setBin(bin,bin);
#endif
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


    close();
}
