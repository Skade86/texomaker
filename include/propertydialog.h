#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>
#include "ui_propertydialog.h"

class XmlDomHandler;
class QLineEdit;
class QCheckBox;
class QLabel;

class PropertyDialog : public QDialog, public Ui::PropertyDialog
{
    Q_OBJECT

public:
    PropertyDialog(QWidget * parent,XmlDomHandler * domHandler =0,QString currentPath="",bool iso=false);

signals:
    void modifWritten();

private:
    XmlDomHandler * domHandler;
    QString currentPath;
    bool iso;

    QMap<QString, QString> currentMetaMap;

    QMap<QString, QString> getOldMetaMap();

    QMap<QString, QString> getNewMetaMap();

    QList<QLineEdit *> editList;

    QList<QLabel *> labelList;

    QList<QCheckBox *> checkList;

    void createLayout();

    void writeChangesToExo(QMap<QString, QString> map);


private slots:

    void on_applyButton_clicked();
    void on_addButton_clicked();
    void on_removeButton_clicked();


};
#endif
