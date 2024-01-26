#ifndef CLASSLISTANDWIDGETITEMMANAGER_H
#define CLASSLISTANDWIDGETITEMMANAGER_H

#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <windows.h>
#define AVOID_HORIZONTAL_BAR_ADJ    10
#define CONST_WIDTH                 lists->width() - AVOID_HORIZONTAL_BAR_ADJ


class ClassListAndWidgetSizePolicyDecision
{
public:

    static QSize sizePolicyDecision(QListWidget* lists, int defaultHeight = 100)
    {
        QSize size;
        size.setWidth(CONST_WIDTH);
        qDebug() << lists->count() * defaultHeight << lists->height();
        if(lists->count() * defaultHeight <= lists->height())
            size.setHeight(defaultHeight);
        else
        {
            size.setHeight(lists->height() / (lists->count() == 0 ? 1 : lists->count()));
        }
        return size;
    }

};


class ClassListAndWidgetItemManager
{
public:
    ClassListAndWidgetItemManager() = default;
    const ClassListAndWidgetItemManager& operator=(const ClassListAndWidgetItemManager&) = delete;
    ClassListAndWidgetItemManager(const ClassListAndWidgetItemManager&) = delete;
    ~ClassListAndWidgetItemManager();

    // push back
    bool createWidgetItem(QListWidget* pFather, QString name);
    // erase
    bool eraseItem(QListWidgetItem* whatItem);
    bool clearItem();

    // getter
    QString getItemText(QListWidgetItem* whatItem);
    int getItemCount(){return this->items.size();}
    QListWidgetItem* matchByName(QString name);

    void debugText(){for(QListWidgetItem* it : items) qDebug() << it->text();};
    void setCurrentItemFocus(QListWidgetItem* whatItem);
    QListWidgetItem* getCurrentItemFocus(){return this->currentItemFocus;}
    QString getCurrentItemText();
    bool isFocusAnyItem(){return this->currentItemFocus != nullptr;}


private:
    QList<QListWidgetItem*>                 items;
    QListWidget* pFather                    = nullptr;
    Qt::ItemFlags itemFlags                 = Qt::ItemIsEnabled|Qt::ItemIsUserCheckable;
    QListWidgetItem* currentItemFocus       = nullptr;
};

#endif // CLASSLISTANDWIDGETITEMMANAGER_H
