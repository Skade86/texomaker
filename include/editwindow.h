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
    explicit EditWindow(QWidget *parent = 0,QString filePath = QString(),bool m_creation=false);
    ~EditWindow();

private:
    Ui::EditWindow *ui;
    bool readFile();
    QString filePath;
    bool creation;
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
    void created(const QString & path);

};

#endif // EDITWINDOW_H
