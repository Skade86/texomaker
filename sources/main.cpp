#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QMessageBox>
#include <QSplashScreen>
#include <QTextCodec>
#include <QFontDatabase>
#include <preferences.h>

#include "mainwindow.h"

QString copyPDFJS(QString to_dir);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setFont(QFont("Calibri", 13));

 //   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
 //   QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    //QPixmap pixmap(":images/splash.png");
    //QSplashScreen splash(pixmap);
    //splash.show();
	
    QLocale locale = QLocale::system();

    QString lang = locale.name().left(2);
    if (lang!="fr") lang="en";

    QTranslator appTranslator;
    appTranslator.load("texomaker_"+lang+".qm",":/translations");
    app.installTranslator(&appTranslator);

    QTranslator qtTranslator;
    qtTranslator.load("qt_"+lang+".qm",":/translations");
    app.installTranslator(&qtTranslator);

#ifdef Q_OS_OSX
    // Copie du répertoire pdf.js des ressources
    Preferences::p_setPdfJSDir(copyPDFJS(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)));
#endif // Q_OS_OSX


#ifdef Q_OS_LINUX
    // Copie du répertoire pdf.js des ressources
    Preferences::p_setPdfJSDir(copyPDFJS(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)));
#endif // Q_OS_OSX

#ifdef Q_OS_WIN
// Copie du répertoire pdf.js des ressources
    Preferences::p_setPdfJSDir(copyPDFJS(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)));
#endif // Q_OS_WIN

    MainWindow mainWin;
    
    mainWin.show();
    //splash.finish(&mainWin);
    return app.exec();
}

QString copyPDFJS(QString to_dir)
{
    QDir dir;
    QStringList toPaths;
    toPaths << to_dir+QDir::separator()+"pdf.js";
    toPaths << to_dir+QDir::separator()+"pdf.js"+ QDir::separator()+"build";
    toPaths << to_dir+QDir::separator()+"pdf.js"+ QDir::separator()+"web";
    toPaths << to_dir+QDir::separator()+"pdf.js"+ QDir::separator()+"web"+QDir::separator()+"images";
    toPaths << to_dir+QDir::separator()+"pdf.js"+ QDir::separator()+"web"+QDir::separator()+"cmaps";
    toPaths << to_dir+QDir::separator()+"pdf.js"+ QDir::separator()+"web"+QDir::separator()+"locale";
    toPaths << to_dir+QDir::separator()+"pdf.js"+ QDir::separator()+"web"+QDir::separator()+"locale"+QDir::separator()+"fr";

    QString from_Dir = ":/pdf.js";
    QStringList fromPaths;
    fromPaths << from_Dir;
    fromPaths << from_Dir + QDir::separator()+"build";
    fromPaths << from_Dir + QDir::separator()+"web";
    fromPaths << from_Dir + QDir::separator()+"web"+QDir::separator()+"images";
    fromPaths << from_Dir + QDir::separator()+"web"+QDir::separator()+"cmaps";
    fromPaths << from_Dir + QDir::separator()+"web"+QDir::separator()+"locale";
    fromPaths << from_Dir + QDir::separator()+"web"+QDir::separator()+"locale"+QDir::separator()+"fr";

    foreach (QString path, toPaths)
    {
        dir.mkpath(path);
    }

    for (int i=0;i<toPaths.count();i++)
    {
        dir.setPath(fromPaths.at(i));
        foreach (QString file,dir.entryList(QDir::Files))
        {
            QFile::copy(fromPaths.at(i)+QDir::separator()+file,toPaths.at(i)+QDir::separator()+file);
        }
    }
    return "file:///"+to_dir+"/pdf.js/web/viewer.html";
}
