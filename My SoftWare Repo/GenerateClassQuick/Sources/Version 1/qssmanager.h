#ifndef QSSMANAGER_H
#define QSSMANAGER_H

#include <QFile>
#include <QDebug>
class QSSManager
{
public:
    QSSManager() = default;
    const QSSManager& operator=(const QSSManager&) = delete;
    QSSManager(const QSSManager&) = delete;

    static QString getQSS(QString path){
        QFile qssFile(path);
        if(!qssFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "QSS Failed!";
            return "";
        }

       return qssFile.readAll();
    }

};

#endif // QSSMANAGER_H
