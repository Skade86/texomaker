#ifndef NEWEXODIALOG_H
#define NEWEXODIALOG_H

#include <QDialog>

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
};

#endif // NEWEXODIALOG_H
