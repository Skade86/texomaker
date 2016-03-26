#ifndef NEWEXODIALOG_H
#define NEWEXODIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QButtonGroup>

namespace Ui {
class NewExoDialog;
}

class NewExoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewExoDialog(QWidget *parent = 0,QString fileName=QString(),QStringList metas=QStringList());
    ~NewExoDialog();

private:
    Ui::NewExoDialog *ui;
    QString fileName_m;
    QStringList metas_m;
    QList<QLabel *> labelList;
    QList<QLineEdit * >  lineList;
    QList<QHBoxLayout *> hLayoutList;
    QButtonGroup *editorGroup;

private slots:
    void create();

signals:

    void exoToEdit(const QString &filePath,const bool &creation);
};

#endif // NEWEXODIALOG_H
