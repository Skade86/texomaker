#ifndef ADVANCEDFILTER_H
#define ADVANCEDFILTER_H

#include <QDockWidget>
#include <QCloseEvent>
#include "preferences.h"
#include "ui_advancedfilter.h"


class AdvancedFilter : public QDockWidget, public Ui::AdvancedFilter
{
    Q_OBJECT
    
public:
     AdvancedFilter(QWidget *parent = 0);
    
protected:
     void closeEvent(QCloseEvent *event);

signals:
     void onFerme();

private slots:

     void onFilterButtonClicked();

};

#endif // ADVANCEDFILTER_H
