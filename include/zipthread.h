#ifndef ZIPTHREAD_H
#define ZIPTHREAD_H

#include <QThread>

//! Classe du thread qui zip le dossier suite à la sauvegarde d'une base
/*!
Cette classe héritant de \a QThread et réimplémentant naturellement la méthode \a run(), exécutée lors du lancement
du thread par start().
*/
class ZipThread : public QThread
{
    Q_OBJECT
public:
    ZipThread(QString zipName,QString dirName,QObject *parent);
    void run();
    
signals:
    //! Signal émis lors d'un archivage réussi
    void zipCreated(const QString &dir);
    
private:
    QString zipName;
    QString dirName;
};

#endif // ZIPTHREAD_H
