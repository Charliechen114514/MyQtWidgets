#ifndef TEXTTREEWIDGET_H
#define TEXTTREEWIDGET_H

#include "TextTreeWidget_global.h"
#include <QWidget>
#include <QTreeWidgetItem>
#include <QMouseEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

enum class TextTreeWidget_DeletePolicy{
    DELETE_TICKED,
    DELETE_ALL
};

enum class TextTreeWidget_CheckablePolicy{
    ENABLE_CHILD_CHECK,
    ENABLE_ONLY_FATHER_CHECK
};

class TEXTTREEWIDGET_EXPORT TextTreeWidget : public QWidget
{
    Q_OBJECT

public:
    TextTreeWidget(QWidget *parent = nullptr);
    bool loadStyleSheetFile(QString filePath);
    void import_subTextTreeToTop(QString singleText);
    void import_subTextTreeToTop(QString theme, QStringList& subTextLists);
    bool import_subTextTree(QString singleText, QTreeWidgetItem* pFather);
    bool import_subTextTree(QString theme, QStringList subTextLists, QTreeWidgetItem* pFather);
    void open_checkState(QTreeWidgetItem* subTree, \
                                                   TextTreeWidget_CheckablePolicy policy = TextTreeWidget_CheckablePolicy::ENABLE_CHILD_CHECK);
    void open_checkState(TextTreeWidget_CheckablePolicy policy = TextTreeWidget_CheckablePolicy::ENABLE_CHILD_CHECK);
    void updateCurrentFocus(QTreeWidgetItem* p){pSelect = p;}
    bool eraseTheFocusedTree();
    QTreeWidgetItem* getCurrentSelectionOfItem(){return pSelect;}
protected:
    void mouseEvent(QMouseEvent*);
    ~TextTreeWidget();

private slots:
    void on_coretreeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_coretreeWidget_itemChanged(QTreeWidgetItem *item, int column);

private:
    Ui::Widget *ui;
    QTreeWidgetItem* pSelect;
    void deleteAll(TextTreeWidget_DeletePolicy policy);
    void deleteCurrentSub(QTreeWidgetItem* father, TextTreeWidget_DeletePolicy policy);
    void duel_TextTreeWidget_DeletePolicy(QTreeWidgetItem* item, TextTreeWidget_DeletePolicy policy);
};

#endif // TEXTTREEWIDGET_H
