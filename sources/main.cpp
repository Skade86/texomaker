#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QMessageBox>
#include <QSplashScreen>
#include <QTextCodec>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setFont(QFont("Calibri", 13));

 //   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
 //   QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QPixmap pixmap(":images/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
	
    QLocale locale = QLocale::system();

    QString lang = locale.name().left(2);
    if (lang!="fr") lang="en";

    QTranslator appTranslator;
    appTranslator.load("texomaker_"+lang+".qm",":/translations");
    app.installTranslator(&appTranslator);

    QTranslator qtTranslator;
    qtTranslator.load("qt_"+lang+".qm",":/translations");
    app.installTranslator(&qtTranslator);


    MainWindow mainWin;
    
    mainWin.show();
    splash.finish(&mainWin);
    return app.exec();
}
