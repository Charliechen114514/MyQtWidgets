#include "utils.h"
#include "newclass.h"
#include <QFileDialog>
#include <QStandardPaths>
bool isUserDecideHisDecision(QWidget* father, QString title, QString fini_text, QString cancel_text,bool isConstructFinish)
{
    if(isConstructFinish)
    {
        // 这个时候询问的是是否需要保留此次记录
        if(QMessageBox::No == QMessageBox::question(father, title, fini_text))
        {
            return false;
        }

        return true;
    }
    else
    {
        if(QMessageBox::No == QMessageBox::question(father, title, cancel_text))
        {
            return false;
        }

        return true;
    }
}

void qDebugTheNewClassInfo(QList<NewClass*>& _class)
{
    qDebug() << "Leaving class";
    for(NewClass* _each : _class)
        qDebug().noquote() << _each->getName();
}

QString upperFirst(QString str)
{
    if(str.size() < 1)
        return "";

    if(str.first(1).isUpper()){
        return str;
    }

    QString upper = str.first(1).toUpper();
    upper += str.removeFirst();
    return upper;
}

QString lowerFirst(QString str)
{
    if(str.size() < 1)
        return "";

    if(str.first(1).isLower()){
        return str;
    }

    QString upper = str.first(1).toLower();
    upper += str.removeFirst();
    return upper;
}

bool getPath(QWidget* pa ,QString title, QString& path)
{
    QString epath = QFileDialog::getOpenFileName(pa, title, DESKTOP_PATH);
    if(epath.isEmpty())
        return false;
    path = epath;
    return true;
}



