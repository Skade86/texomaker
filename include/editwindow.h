#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QMainWindow>

class Highlighter;

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
    bool readFile();
    QString filePath;
    Highlighter * highlighter;
    QAction * saveAct;
    QAction * updateAct;
    QToolBar * toolBar;
    void createActions();
    void createToolBar();
    bool maybeSave();
    void closeEvent(QCloseEvent *e);


private slots:
    bool fileSave();
    bool fileUpdate();

signals:
    void updated();

};

#endif // EDITWINDOW_H
