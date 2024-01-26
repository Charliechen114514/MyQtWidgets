#ifndef UTILS_H
#define UTILS_H
#include "CCSTDC_Assert.h"
#include <QMessageBox>
#define DESKTOP_PATH            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
#define ENABLE_WIDGET(widget)   ui->widget->setEnabled(true)
#define DISABLE_WIDGET(widget)  ui->widget->setEnabled(false)
#define REJECT(TITLE, TEXT)     QMessageBox::critical(this,TITLE, TEXT);return
#define REJECT_EMPTY_TEXT(text, TITLE, TEXT) if(text.isEmpty()){ REJECT(TITLE, TEXT);}
#define CONSIDER_RECEIVE  is_construct = true
#define CONSIDER_REJECT   is_construct = false

class       NewClass;
bool        isUserDecideHisDecision(QWidget* father, QString title, QString fini_text, QString cancel_text,bool isConstructFinish);
void        qDebugTheNewClassInfo(QList<NewClass*>& _class);
QString     upperFirst(QString str);
QString     lowerFirst(QString str);
bool        getPath(QWidget* pa ,QString title, QString& path);
#endif // UTILS_H
