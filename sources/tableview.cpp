#include <QtWidgets>
#include <QDebug>
#include "tableview.h"

TableView::TableView(QWidget *parent) : QTableView(parent)
{}

void TableView::contextMenuEvent(QContextMenuEvent * e)
{
	Q_UNUSED(e);
}


void TableView::wheelEvent ( QWheelEvent * event )
{
  if (event->modifiers() & Qt::ControlModifier ) {
      int currentFontSize = this->font().pointSize();
      if ((event->delta()>0) && (currentFontSize<=30))
          this->setFont(QFont("calibri",currentFontSize + 1));
      else if ((event->delta()<0) && (currentFontSize>=6))
          this->setFont(QFont("calibri",currentFontSize - 1));
      this->resizeRowsToContents();
  }
  else QTableView::wheelEvent(event);

  event->accept();
}
