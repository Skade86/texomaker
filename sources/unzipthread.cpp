#include "unzipthread.h"
#include "JlCompress.h"

UnzipThread::UnzipThread(QString zipName,QString dirName, QObject *parent) :
    QThread(parent), zipName(zipName), dirName(dirName)
{
}

void UnzipThread::run()
{
    JlCompress::extractDir(zipName,dirName);
    emit unzipCreated(dirName);
}
