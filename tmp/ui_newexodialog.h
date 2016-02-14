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
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_NewExoDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NewExoDialog)
    {
        if (NewExoDialog->objectName().isEmpty())
            NewExoDialog->setObjectName(QStringLiteral("NewExoDialog"));
        NewExoDialog->resize(400, 300);
        verticalLayout_2 = new QVBoxLayout(NewExoDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(NewExoDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


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
