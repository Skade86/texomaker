#ifndef UNZIPTHREAD_H
#define UNZIPTHREAD_H

#include <QThread>

//! Classe du thread qui dézippe le dossier suite à la sauvegarde d'une base
/*!
Cette classe héritant de \a QThread et réimplémentant naturellement la méthode \a run(), exécutée lors du lancement
du thread par start().
*/
class UnzipThread : public QThread
{
    Q_OBJECT
public:
    UnzipThread(QString zipName,QString dirName,QObject *parent);
    void run();
    
signals:
    //! Signal émis lors d'un archivage réussi
    void unzipCreated(const QString &dirName);
    
private:
    QString zipName,dirName;
};

#endif // UNZIPTHREAD_H
