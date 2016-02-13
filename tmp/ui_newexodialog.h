/********************************************************************************
** Form generated from reading UI file 'newexodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWEXODIALOG_H
#define UI_NEWEXODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_NewExoDialog
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NewExoDialog)
    {
        if (NewExoDialog->objectName().isEmpty())
            NewExoDialog->setObjectName(QStringLiteral("NewExoDialog"));
        NewExoDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(NewExoDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(NewExoDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), NewExoDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), NewExoDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(NewExoDialog);
    } // setupUi

    void retranslateUi(QDialog *NewExoDialog)
    {
        NewExoDialog->setWindowTitle(QApplication::translate("NewExoDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class NewExoDialog: public Ui_NewExoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWEXODIALOG_H
