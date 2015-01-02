#include "zipthread.h"
#include "JlCompress.h"

ZipThread::ZipThread(QString zipName, QString dirName, QObject *parent) :
    QThread(parent), zipName(zipName), dirName(dirName)
{
}

void ZipThread::run()
{
    JlCompress::compressDir(zipName,dirName);
    emit zipCreated(dirName);
}
