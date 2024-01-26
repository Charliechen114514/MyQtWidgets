#include "classlistandwidgetitemmanager.h"
#define LIMIT_HEIGHT                10
#define SPEED                       5
ClassListAndWidgetItemManager::~ClassListAndWidgetItemManager()
{
    for(QListWidgetItem*& tmp : this->items)
        eraseItem(tmp);
}

bool ClassListAndWidgetItemManager::createWidgetItem(QListWidget* pFather, QString className)
{
    if(pFather == nullptr)
        return false;

    QListWidgetItem* item = new QListWidgetItem(pFather);

    item->setText(className);

    item->setFont(QFont("Consola", 14));

    item->setTextAlignment(Qt::AlignmentFlag::AlignCenter);

    item->setSizeHint(ClassListAndWidgetSizePolicyDecision::sizePolicyDecision(pFather));

    item->setFlags(this->itemFlags);

    this->pFather = pFather;

    this->setCurrentItemFocus(item);

    items.push_back(item);

    return true;
}

void ClassListAndWidgetItemManager::setCurrentItemFocus(QListWidgetItem* whatItem)
{
    qDebug() << "Current focus on:> " << whatItem->text();
    this->currentItemFocus = whatItem;
}

QListWidgetItem* ClassListAndWidgetItemManager::matchByName(QString name)
{
    qDebug() << this->items.size();
    for(QListWidgetItem* tmp : this->items)
        if(tmp->text() == name)
            return tmp;

    return nullptr;
}


QString ClassListAndWidgetItemManager::getCurrentItemText()
{
    if(!this->isFocusAnyItem())
        return "";
    return this->getItemText(this->currentItemFocus);
}


bool ClassListAndWidgetItemManager::eraseItem(QListWidgetItem* whatItem)
{
    if(whatItem == nullptr)
        return false;

    this->items.removeOne(whatItem);
    delete whatItem;
    whatItem = nullptr;
    return true;
}

bool ClassListAndWidgetItemManager::clearItem()
{
    for(QListWidgetItem* tmp : this->items)
        eraseItem(tmp);

    return true;
}

QString ClassListAndWidgetItemManager::getItemText(QListWidgetItem* whatItem)
{
    if(whatItem == nullptr)
        return "";
    return whatItem->text();
}

