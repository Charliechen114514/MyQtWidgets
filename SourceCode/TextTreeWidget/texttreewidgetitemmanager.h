#ifndef TEXTTREEWIDGETITEMMANAGER_H
#define TEXTTREEWIDGETITEMMANAGER_H
#include <QTreeWidgetItem>

class TextTreeWidgetItemManager
{
public:
    TextTreeWidgetItemManager();
    const TextTreeWidgetItemManager& operator=(const TextTreeWidgetItemManager&) = delete;
    TextTreeWidgetItemManager(const TextTreeWidgetItemManager&) = delete;

    void setTreeWidgetCheck(QTreeWidgetItem* item, Qt::CheckState state= Qt::Checked, int column = 0);
    void setTreeWidgetTextCheckStyle(QTreeWidgetItem* item,Qt::CheckState state = Qt::Checked, int column = 0);
    void scanToUpdate(QTreeWidgetItem* item, int column = 0);
private:
    QFont checkedFont;
    QFont uncheckedFont;
};

#endif // TEXTTREEWIDGETITEMMANAGER_H
