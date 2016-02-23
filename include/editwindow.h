#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QMainWindow>

namespace Ui {
class EditWindow;
}

class EditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = 0,QString filePath = QString());
    ~EditWindow();

private:
    Ui::EditWindow *ui;
    void readFile();
    QString filePath;

    QAction * saveAct;
    QAction * updateAct;
    QToolBar * toolBar;
    void createActions();
    void createToolBar();

private slots:
    bool fileSave();
    bool fileUpdate();

};

#endif // EDITWINDOW_H
