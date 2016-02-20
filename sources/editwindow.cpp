#include <Qsci/qsciscintilla.h>
#include<Qsci/qscilexertex.h>
#include "editwindow.h"
#include "ui_editwindow.h"

EditWindow::EditWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    QsciLexerTeX * lexer = new QsciLexerTeX;
    //lexer->setColor(Qt::blue,QsciLexerTeX::Command);
    ui->textEdit->setLexer(lexer);

}

EditWindow::~EditWindow()
{
    delete ui;
}
