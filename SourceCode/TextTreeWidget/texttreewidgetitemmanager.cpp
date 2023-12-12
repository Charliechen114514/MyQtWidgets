#include "texttreewidgetitemmanager.h"



void setCheckedFont(QFont* pFont)
{
    pFont->setItalic(true);
    pFont->setBold(true);
    pFont->setStrikeOut(true);
}

void setUnCheckedFont(QFont* pFont){
    pFont->setItalic(false);
    pFont->setBold(false);
    pFont->setStrikeOut(false);
}


TextTreeWidgetItemManager::TextTreeWidgetItemManager()
{
    setCheckedFont(&checkedFont);
    setUnCheckedFont(&uncheckedFont);
}

void TextTreeWidgetItemManager::setTreeWidgetTextCheckStyle(QTreeWidgetItem* item, Qt::CheckState state, int column)
{
    if(item->checkState(column) != state)
    {
        qDebug() << "Exception with un matched state";
        return;
    }

    switch(state)
    {
    case Qt::Unchecked:
        item->setFont(column, uncheckedFont);
        break;

    case Qt::Checked:
        item->setFont(column, checkedFont);
        break;

    case Qt::PartiallyChecked:
        // haven't yet make any decision lol
        break;

    default:
        qDebug() << "Unknown policy, ignore the case";
        break;
    }
}





void TextTreeWidgetItemManager::setTreeWidgetCheck(QTreeWidgetItem* item, Qt::CheckState state,int column)
{
    item->setCheckState(column, state);
    setTreeWidgetTextCheckStyle(item, state,column);
}

void TextTreeWidgetItemManager::scanToUpdate(QTreeWidgetItem* item, int column)
{
    setTreeWidgetTextCheckStyle(item, item->checkState(column), column);
}

