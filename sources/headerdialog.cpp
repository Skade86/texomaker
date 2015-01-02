#include <QtWidgets>
#include "headerdialog.h"

HeaderDialog::HeaderDialog(QString header) 
	: QDialog()
{
	setupUi(this);
	if (!header.isEmpty()) headerEdit->setPlainText(header);
}

void HeaderDialog::on_okButton_clicked()
{
	emit(headerChoosen(headerEdit->toPlainText()));
	close();
}

