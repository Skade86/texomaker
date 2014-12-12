#include <QtWidgets>
#include "tableview.h"

TableView::TableView(QWidget *parent) : QTableView(parent)
{}

void TableView::contextMenuEvent(QContextMenuEvent * e)
{
	Q_UNUSED(e);
/*	
	QMenu *menu = new QMenu(this);
	QModelIndex index = indexAt(e->pos());
	if (index.isValid())
		menu->addAction(QString("Row %1 - Col %2 was clicked on").arg(index.row()).arg(index.column()));
	else
		menu->addAction("No item was clicked on");
	
	menu->exec(QCursor::pos());
 */
}
