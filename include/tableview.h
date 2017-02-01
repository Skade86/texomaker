#ifndef TABLEVIEW_H
#define TABLEVIEW_H

class TableView : public QTableView
{

public:
	
	TableView(QWidget *parent);
	
    void contextMenuEvent ( QContextMenuEvent * e );
    void wheelEvent ( QWheelEvent * event );
};
#endif
