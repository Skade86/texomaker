#ifndef HEADERDIALOG_H
#define HEADERDIALOG_H

#include <QDialog>
#include "ui_headerdialog.h"

class HeaderDialog : public QDialog, public Ui::HeaderDialog
{
	
Q_OBJECT

public:

//! 
/*! 
	
*/
	HeaderDialog(QString header = "");

signals:

	void headerChoosen(const QString & headerText);
	
private slots:

	void on_okButton_clicked();

};
#endif
