#include "preferences.h"

QString Preferences::curVersion=QString(QObject::tr("%1.%2").arg(MAJOR).arg(MINOR));
QString Preferences::sqliteDbPath="/Users/gwenael/Documents/Developpement/Qt/TeXoMaker/DB/";
QString Preferences::dbfile=QString();
QString Preferences::ltx2pdf=QString();
QString Preferences::preamble=QString();
QString Preferences::macroFiles=QString();
QString Preferences::compiler=QString();
QString Preferences::sheetHeader=QString();
QString Preferences::sheetFooter=QString();
QString Preferences::sheetExo=QString();
QString Preferences::sheetBetween=QString();
QString Preferences::beginDoc=QString();
QString Preferences::endDoc=QString();
bool Preferences::openAtLaunch=true;
bool Preferences::useIso=false;
QString Preferences::compilationOptions=QString();
QStringList Preferences::metaList=QStringList();
QStringList Preferences::metaToView=QStringList();
QStringList Preferences::sheetBookmarks=QStringList();
QMap<QString, QString> Preferences::binMap;
