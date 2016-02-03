#include <QtWidgets>
#include <QFileDialog>
#include "dirprefdialog.h"
#include "preferences.h"
#include "xmldomhandler.h"

DirPrefDialog::DirPrefDialog(QWidget * parent,XmlDomHandler * domHandler,int rows,bool creation)
        : QDialog(parent),domHandler(domHandler),rows(rows),creation(creation)
{
    setupUi(this);

    QTabBar* curTab = new QTabBar();
    curTab = prefTab->tabBar();
    curTab -> moveTab(0,2);

    prefTab->setCurrentIndex(0);
    metaAdded=0;

    if (!Preferences::p_getPreamble().isEmpty()) instructionEdit->setPlainText(Preferences::p_getPreamble());
    macroFilesList->addItems(Preferences::p_getMacroFiles());


    if (Preferences::p_getCompiler()=="latex")
    {
        latexRadio->setChecked(true);
    }
    else if (Preferences::p_getCompiler()=="tex")
    {
        texRadio->setChecked(true);
    }
    else if (Preferences::p_getCompiler()=="lualatex")
    {
        lualatexRadio->setChecked(true);
    }
    else pdflatexRadio->setChecked(true);

    beginDocEdit->setText(Preferences::p_getBeginDoc());
    endDocEdit->setText(Preferences::p_getEndDoc());

    compileOptionsEdit->setText(Preferences::p_getCompilationOptions());

    setMetaTab();
}

void DirPrefDialog::setMetaTab()
{
    viewChecked = 0;

    metaSize = Preferences::p_getMetaSize()-4;
    QStringList metaOblig;
    metaOblig << "filepath" << "importdate" << "figure" << "metapost";
    QStringList metaAll = Preferences::p_getMetaList();
    QStringList metaView = Preferences::p_getMetaToView();

    metaGrid->addWidget(line, 1, 0, 1, 4);

    for (int i=0;i<metaSize;i++)

    {
        metaLine.append(new QLineEdit(metaTab));
        metaCheck.append(new QCheckBox(metaTab));
        metaSpin.append(new QSpinBox(metaTab));
        metaRadio.append(new QRadioButton(metaTab));
    }


    for (int i=0;i<metaSize;i++)
    {
        metaSpin.at(i)->setEnabled(false);
        metaSpin.at(i)->setMinimum(1);
        metaSpin.at(i)->setWrapping(true);

        metaRadio.at(i)->setAutoExclusive(true);

        connect(metaCheck.at(i),SIGNAL(toggled(bool)),metaSpin.at(i),SLOT(setEnabled(bool)));
        connect(metaCheck.at(i),SIGNAL(toggled(bool)),this,SLOT(updateMax(bool)));


        metaGrid->addWidget(metaRadio.at(i),i+6,0,1,1);
        metaGrid->addWidget(metaLine.at(i),i+6,1,1,1);
        metaGrid->addWidget(metaCheck.at(i),i+6,2,1,1);
        metaGrid->addWidget(metaSpin.at(i),i+6,3,1,1);

        if (rows>0) metaLine.at(i)->setReadOnly(true);
    }

    if (metaView.contains("figure"))
    {
        checkFig->setChecked(true);
        spinFig->setValue(metaView.indexOf("figure")+1);
    }

    if (metaView.contains("metapost"))
    {
        checkMp->setChecked(true);
        spinMp->setValue(metaView.indexOf("metapost")+1);
    }

    if (metaView.contains("importdate"))
    {
        checkDate->setChecked(true);
        spinDate->setValue(metaView.indexOf("importdate")+1);
    }

    if (metaView.contains("filepath"))
    {
        checkPath->setChecked(true);
        spinPath->setValue(metaView.indexOf("filepath")+1);
    }

    int j=0;

    for (int i=0;i<metaSize+4;i++)
    {
        if (!metaOblig.contains(metaAll.at(i)))
        {
            metaLine.at(j)->setText(metaAll.at(i));

            if (metaView.contains(metaAll.at(i)))
            {
                metaCheck.at(j)->setChecked(true);
                viewChecked++;
                metaSpin.at(j)->setValue(metaView.indexOf(metaAll.at(i))+1);
            }
            j++;
        }
    }

    if (checkMp->isChecked()) viewChecked++;
    if (checkDate->isChecked()) viewChecked++;
    if (checkFig->isChecked()) viewChecked++;
    if (checkPath->isChecked()) viewChecked++;

    for (int i=0;i<metaSize;i++)
    {
        metaSpin.at(i)->setMaximum(viewChecked);
    }
    spinMp->setMaximum(viewChecked);
    spinFig->setMaximum(viewChecked);
    spinPath->setMaximum(viewChecked);
    spinDate->setMaximum(viewChecked);

    connect(checkMp,SIGNAL(toggled(bool)),this,SLOT(updateMax(bool)));
    connect(checkFig,SIGNAL(toggled(bool)),this,SLOT(updateMax(bool)));
    connect(checkDate,SIGNAL(toggled(bool)),this,SLOT(updateMax(bool)));
    connect(checkPath,SIGNAL(toggled(bool)),this,SLOT(updateMax(bool)));


}

void DirPrefDialog::updateMax(bool checked)
{
    // Mets à jour les maximum des spinbox des métadonnées suite à ajout (true) ou
    // suppression (false) d'une métadonnée.
    for (int i=0;i<metaSize;i++)
        if (checked)
        {
            metaSpin.at(i)->setMaximum(metaSpin.at(i)->maximum()+1);
        }
        else metaSpin.at(i)->setMaximum(metaSpin.at(i)->maximum()-1);

    if (checked)
    {
        spinMp->setMaximum(spinMp->maximum()+1);
    }
    else spinMp->setMaximum(spinMp->maximum()-1);

    if (checked)
    {
        spinFig->setMaximum(spinFig->maximum()+1);
    }
    else spinFig->setMaximum(spinFig->maximum()-1);

    if (checked)
    {
        spinDate->setMaximum(spinDate->maximum()+1);
    }
    else spinDate->setMaximum(spinDate->maximum()-1);

    if (checked)
    {
        spinPath->setMaximum(spinPath->maximum()+1);
    }
    else spinPath->setMaximum(spinPath->maximum()-1);
}

void DirPrefDialog::updateMetaSpin(int toRemove)
{
    // Suite à la suppression d'une métadonnée. Si celle-ci était visible, cette fonction
    // mets à jour (décale de -1) l'ordre d'apparition des autres métadonnées visibles qui étaient
    // placées après

    for (int i=0;i<metaSize;i++)
        if ((i!=toRemove)&&(metaSpin.at(i)->value()>metaSpin.at(toRemove)->value()))
        {
            metaSpin.at(i)->setValue(metaSpin.at(i)->value()-1);
        }
}

void DirPrefDialog::on_addButton_clicked()
{
    metaSize++;

    metaRadio.append(new QRadioButton(metaTab));
    metaLine.append(new QLineEdit(metaTab));
    metaCheck.append(new QCheckBox(metaTab));
    metaSpin.append(new QSpinBox(metaTab));

    int newMax=metaSpin.at(metaSize-2)->maximum()+1;
    metaRadio.at(metaSize-1)->setAutoExclusive(true);
    metaSpin.at(metaSize-1)->setEnabled(true);
    metaSpin.at(metaSize-1)->setWrapping(true);
    metaSpin.at(metaSize-1)->setMinimum(1);
    metaSpin.at(metaSize-1)->setValue(newMax);

    // On met à jour le maximum des spinBox
    for (int i=0;i<metaSize;i++) metaSpin.at(i)->setMaximum(newMax);

    metaCheck.at(metaSize-1)->setChecked(true);

    connect(metaCheck.at(metaSize-1),SIGNAL(toggled(bool)),metaSpin.at(metaSize-1),SLOT(setEnabled(bool)));
    connect(metaCheck.at(metaSize-1),SIGNAL(toggled(bool)),this,SLOT(updateMax(bool)));

    metaGrid->addWidget(metaRadio.at(metaSize-1),metaSize-1+6,0,1,1);
    metaGrid->addWidget(metaLine.at(metaSize-1),metaSize-1+6,1,1,1);
    metaGrid->addWidget(metaCheck.at(metaSize-1),metaSize-1+6,2,1,1);
    metaGrid->addWidget(metaSpin.at(metaSize-1),metaSize-1+6,3,1,1);

    metaAdded++;
}

void DirPrefDialog::on_removeButton_clicked()
{
    int toRemove;

    toRemove=-1;
    // On remplit une liste contenant les indices des metadonnées
    for (int i=0;i<metaSize;i++) if (metaRadio.at(i)->isChecked()) toRemove = i;

    if (toRemove<0) return;

    updateMetaSpin(toRemove);
    updateMax(false);

    QString metaText = metaLine.at(toRemove)->text();

    // On ferme les widgets
    metaRadio.at(toRemove)->close();
    metaSpin.at(toRemove)->close();
    metaCheck.at(toRemove)->close();
    metaLine.at(toRemove)->close();
    // On les supprime de la liste
    metaRadio.removeAt(toRemove);
    metaLine.removeAt(toRemove);
    metaCheck.removeAt(toRemove);
    metaSpin.removeAt(toRemove);

    // Mise à jour des listes dans les préférences
    QStringList oldMetaAll = Preferences::p_getMetaList();
    QStringList oldMetaView = Preferences::p_getMetaToView();

    for (int i=0;i<oldMetaAll.size();i++)
        if (oldMetaAll.at(i)==metaText) oldMetaAll.removeAt(i);

    for (int i=0;i<oldMetaView.size();i++)
        if (oldMetaView.at(i)==metaText) oldMetaView.removeAt(i);

    Preferences::p_setMetaList(oldMetaAll);
    Preferences::p_setMetaToView(oldMetaView);
    if (!creation) {
        domHandler->removeExoMeta(metaText);
        domHandler->saveInXml();
    }
    metaSize--;
}

// Ajoute un fichier perso à la liste
void DirPrefDialog::on_addMacroFileButton_clicked()
{
    QString home = QDir::homePath();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a file to add"),
                                                    home,tr("Latex files (*.*)"));
    if (fileName.isEmpty()) return;
    macroFilesList->addItem(fileName);

}

// Enlève le fichier perso sélectioné à la liste
void DirPrefDialog::on_delMacroFileButton_clicked()
{
    QListWidgetItem * removedItem = macroFilesList->takeItem(macroFilesList->currentRow());

    if (removedItem==0)
    {
        QMessageBox::warning(0, QObject::tr("Error"),QObject::tr("No file was selected to remove."));
        return;
    }

}

void DirPrefDialog::on_okButton_clicked()
{
    Preferences::p_setPreamble(instructionEdit->toPlainText());
    QStringList macroStringList = QStringList();
    for(int row = 0; row < macroFilesList->count(); row++)
    {
             QListWidgetItem *item = macroFilesList->item(row);
             macroStringList << item->text();
    }
    Preferences::p_setMacroFiles(macroStringList);

    Preferences::p_setBeginDoc(beginDocEdit->text());
    Preferences::p_setEndDoc(endDocEdit->text());
    Preferences::p_setCompilationOptions(compileOptionsEdit->text());

    if (latexRadio->isChecked())
    {
        Preferences::p_setCompiler("latex");
    }
    else if (texRadio->isChecked())
    {
        Preferences::p_setCompiler("tex");
    }
    else if (lualatexRadio->isChecked())
    {
        Preferences::p_setCompiler("lualatex");
    }
    else  Preferences::p_setCompiler("pdflatex");

   // if (metaAdded>0) updateMetaDatas();
    updateMetaDatas();
    emit aboutToClose();
    close();
}


bool DirPrefDialog::updateMetaDatas()
{
    QStringList metaLst;
    QMap<QString, int> toViewMap;
    QStringList metaToView;
    int newSize = 0;
    // On enregistre dans metaLst tous les noms (hors obligatoires) de es
    // On /ordre d'apparition si la checkBox est checkée
    for (int i=0;i<metaSize;i++)
    {
        if (!metaLine.at(i)->text().isEmpty())
        {
            newSize++;
            metaLst << metaLine.at(i)->text();
            if (metaCheck.at(i)->isChecked()) toViewMap.insert(metaLine.at(i)->text(),metaSpin.at(i)->value());
        }
    }


    metaLst << "filepath" << "importdate" << "figure" << "metapost";

    if (checkDate->isChecked()) toViewMap.insert(metaDate->text(),spinDate->value());
    if (checkPath->isChecked()) toViewMap.insert(metaPath->text(),spinPath->value());
    if (checkFig->isChecked()) toViewMap.insert(metaFig->text(),spinFig->value());
    if (checkMp->isChecked()) toViewMap.insert(metaMp->text(),spinMp->value());

    for (int i=0;i<toViewMap.size();i++) metaToView.append("");

    QStringList keys;

    QMapIterator<QString, int> j(toViewMap);

    while (j.hasNext())
    {
        j.next();
        keys = toViewMap.keys(j.value());
        if (keys.size()>1)
        {
            QMessageBox::warning(this, QObject::tr("Error"),QObject::tr("There is a problem with the order of the metadatas you have choosen : probably two differents metadatas associated to the same value.\nPlease correct this before clicking \"Ok\"."));
            return false;
        }
        keys.clear();
        metaToView.replace(j.value()-1,j.key());
    }

    QStringList oldMetaList = Preferences::p_getMetaList();
    QStringList oldMetaView = Preferences::p_getMetaToView();

    Preferences::p_setMetaList(metaLst);
    Preferences::p_setMetaToView(metaToView);

//    if ((oldMetaList!= Preferences::p_getMetaList()) || ((oldMetaView!= Preferences::p_getMetaToView()) || (creation))) emit aboutToClose();

    return true;
}
