#ifndef GENERALSETTINGSDIALOG_H
#define GENERALSETTINGSDIALOG_H

#include <QDialog>
#include "ui_generalsettingsdialog.h"


class GeneralSettingsDialog : public QDialog, public Ui::GeneralSettingsDialog
{
    Q_OBJECT

public:
    GeneralSettingsDialog(QWidget *parent = 0);

private:
    void testBin(QString bin);

private slots:

//! Acceptation des (éventuelles) modifications
    void on_okButton_clicked();
    void on_pdflatexBrowser_clicked();
    void on_latexBrowser_clicked();
    void on_dvipsBrowser_clicked();
    void on_ps2pdfBrowser_clicked();
    void on_mpostBrowser_clicked();
};

#endif // GENERALSETTINGSDIALOG_H
