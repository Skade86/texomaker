#ifndef PDFLATEXLOGDIALOG_H
#define PDFLATEXLOGDIALOG_H

#include <QDialog>
#include "ui_pdflatexlogdialog.h"

class PdflatexLogDialog : public QDialog, public Ui::PdflatexLogDialog
{
Q_OBJECT

public:
	PdflatexLogDialog(QWidget * parent,QString exoFile,QString logText);
};
#endif
