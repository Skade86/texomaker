#ifndef EXOSHEET_H
#define EXOSHEET_H

#include <QDialog>
#include "ui_exosheet.h"

class QDomDocument;
class QDomNodeList;
class MainWindow;
class XmlDomHandler;

class ExoSheet : public QDialog, public Ui::ExoSheet
{
Q_OBJECT
public:

//! Constructeur
/*! 
	
*/
	ExoSheet( QWidget * parent = 0, bool useIso = false, XmlDomHandler *handler=0);
	
	void addExo(QStringList exoList);
	
	QString stdPdflatexOutput;
	
signals:

	void pdflatexFinished(const QString &pdfFile,const QString &output);
	
private slots:

	void on_createButton_clicked();
	void on_browseButton_clicked();
    void on_openButton_clicked();
	void on_removeButton_clicked();
    void on_upButton_clicked();
    void on_downButton_clicked();

private:

	void createTexFile(bool mpTest);
    QString createXmlSheetFile();
    void createDomSheetFile(QString fileName);
    void writeXmlSheetFile(QString comment);
	QString replaceMeta(QString path, QString str);
	bool copyFigsFiles(QStringList exo);
	QString compileTexFile();
    QString getDomBaseName();
    QString getDomDirName();
    QList<QStringList > getDomExoList();
    QDomNodeList * historyNode;
    int getCurRevNumber();
    QString getCurComment();
    QString getCurDate();
    QStringList getLatexOptionList();
    int returnCurrentSheetListIndex(QString title);
	
    QString outputDir;
    QString outputFile;
    QString sheetFile;
    QList<QStringList> exoSheetList;


    bool existingSheetOpened;

    int numItem;
    bool useIso;
    XmlDomHandler * domHandler;
    QDomDocument * domSheet;

    void writeSettings();

};
#endif
