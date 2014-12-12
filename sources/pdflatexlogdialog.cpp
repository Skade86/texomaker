#include <QtWidgets>
#include "pdflatexlogdialog.h"

PdflatexLogDialog::PdflatexLogDialog(QWidget * parent,QString exoFile,QString logText) 
	: QDialog(parent)
{
	setupUi(this);
	QString headerText;
	QString previewTexFile = QFileInfo(exoFile).path() + QDir::separator() + QFileInfo(exoFile).baseName() + "-preview.tex";
	headerTextEdit->setAcceptRichText(true);
	
	setWindowTitle(tr("Compilation error !"));
	headerText =tr(" <font color=\"red\">");
	headerText += tr("<p>An error occured compiling the exercise <b>") + QFileInfo(exoFile).baseName() + tr(".tex</b></p>");
	headerText += tr("Please check the error on the output text below, correct it and import again the exercise. Note that you can find the");
	headerText += tr(" preview .tex file ready to be compiled at the following place : <b>") + previewTexFile + tr("</b></p></font>");
	headerTextEdit->setHtml(headerText);
	outputTextEdit->setPlainText(logText);
}
