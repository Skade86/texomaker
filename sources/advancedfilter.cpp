#include "advancedfilter.h"

AdvancedFilter::AdvancedFilter(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);

    // On prépare les comboBox avec la liste des colonnes
    QStringList columns = Preferences::p_getMetaToView();
    filterCombo1->addItems(columns);
    filterCombo2->addItems(columns);
    filterCombo2->setCurrentIndex(1);
    filterCombo3->addItems(columns);
    filterCombo3->setCurrentIndex(2);

    // Connexion du bouton filtre à son slot
    connect(filterLaunchButton,SIGNAL(clicked()),this,SLOT(onFilterButtonClicked()));

}


void AdvancedFilter::onFilterButtonClicked()
{
    QStringList filterList;
    filterList << filterCombo1->currentText() << filterCombo2->currentText() << filterCombo3->currentText();
}

void AdvancedFilter::closeEvent(QCloseEvent *event)
{
    emit onFerme();
    event->accept();
}
