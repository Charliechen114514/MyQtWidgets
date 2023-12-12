#include "texttreewidget.h"
#include "ui_texttreewidget.h"
#include <texttreewidgetitemmanager.h>
#include <QFile>
TextTreeWidgetItemManager manager;
#define REVERSE_RECURSIVE 0

void TextTreeWidget::duel_TextTreeWidget_DeletePolicy(QTreeWidgetItem* item, TextTreeWidget_DeletePolicy policy)
{
    switch(policy)
    {

    case TextTreeWidget_DeletePolicy::DELETE_TICKED:
        if(item->checkState(0) == Qt::Checked)
        {
            delete item;
            item = nullptr;
        }
        break;
    case TextTreeWidget_DeletePolicy::DELETE_ALL:
        delete item;
        item = nullptr;
        break;

    default:
        break;
    }
}

TextTreeWidget::TextTreeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    open_checkState();
}

bool TextTreeWidget::loadStyleSheetFile(QString path)
{
    QFile qssFile(path);
    QString styles;
    if(qssFile.open(QIODevice::ReadOnly))
    {
        styles = qssFile.readAll();
        this->setStyleSheet(styles);
        return true;
    }
    return false;
}



bool TextTreeWidget::import_subTextTree(QString singleText, QTreeWidgetItem* pFather)
{
    if(pFather == nullptr)
    {
        return false;
    }

    QTreeWidgetItem* pChild = new QTreeWidgetItem(pFather);
    pChild->setText(0, singleText);
    if(pSelect == pFather)
        pSelect = nullptr;
    return true;
}

bool TextTreeWidget::import_subTextTree(QString theme, QStringList subTextLists, QTreeWidgetItem* pFather)
{
    if(pFather == nullptr)
    {
        return false;
    }
    QTreeWidgetItem* pChild = new QTreeWidgetItem(pFather);
    pChild->setText(0, theme);
    for(QString& eachText : subTextLists)
    {
        QTreeWidgetItem* tmp = new QTreeWidgetItem(pChild) ;
        tmp->setText(0, eachText);
        pChild->addChild(tmp);
    }

    if(pSelect == pFather)
        pSelect = nullptr;

    return true;
}

void TextTreeWidget::import_subTextTreeToTop(QString singleText)
{
    QTreeWidgetItem* pChild = new QTreeWidgetItem(ui->coretreeWidget);
    pChild->setText(0, singleText);
}

void TextTreeWidget::import_subTextTreeToTop(QString theme, QStringList& subTextLists)
{
    QTreeWidgetItem* pChild = new QTreeWidgetItem(ui->coretreeWidget);
    pChild->setText(0, theme);
    for(QString& eachText : subTextLists)
    {
        QTreeWidgetItem* tmp = new QTreeWidgetItem(pChild) ;
        tmp->setText(0, eachText);
        pChild->addChild(tmp);
    }
}

void TextTreeWidget::deleteCurrentSub(QTreeWidgetItem* father, TextTreeWidget_DeletePolicy policy)
{
    int childCnt = father->childCount();
    if(childCnt == 0)
    {
        qDebug() << father->text(0);
        duel_TextTreeWidget_DeletePolicy(father, policy);
        return;
    }

    for(int i  = 0; i < childCnt; i++)
    {
        deleteCurrentSub(father->child(REVERSE_RECURSIVE), policy);
    }
    qDebug() << father->text(0);
    duel_TextTreeWidget_DeletePolicy(father, policy);
    return;
}

void TextTreeWidget::deleteAll(TextTreeWidget_DeletePolicy policy)
{
    // we declare this var as we need to prevent the going less cnt of child cnt
    int cnt = ui->coretreeWidget->topLevelItemCount();
    for(int i = 0; i < cnt; i++)
    {
        deleteCurrentSub(ui->coretreeWidget->topLevelItem(REVERSE_RECURSIVE),policy);
    }
}

void TextTreeWidget::open_checkState(TextTreeWidget_CheckablePolicy policy)
{
    // we declare this var as we need to prevent the going less cnt of child cnt
    int cnt = ui->coretreeWidget->topLevelItemCount();
    for(int i = 0; i < cnt; i++)
    {
        open_checkState(ui->coretreeWidget->topLevelItem(i), policy);
    }
}

void TextTreeWidget::open_checkState(QTreeWidgetItem* item, TextTreeWidget_CheckablePolicy policy)
{
    int childCnt = item->childCount();
    if(policy == TextTreeWidget_CheckablePolicy::ENABLE_ONLY_FATHER_CHECK || childCnt == 0)
    {
        manager.setTreeWidgetCheck(item, Qt::Unchecked);
        return;
    }

    for(int i = 0; i < childCnt; i++)
    {
        open_checkState(item->child(i), policy);
    }

    manager.setTreeWidgetCheck(item, Qt::Unchecked);
    return;
}

bool TextTreeWidget::eraseTheFocusedTree()
{
    if(pSelect == nullptr)
    {
        return false;
    }
    deleteCurrentSub(pSelect, TextTreeWidget_DeletePolicy::DELETE_ALL);
    pSelect = nullptr;
    return true;
}


void TextTreeWidget::on_coretreeWidget_itemClicked(QTreeWidgetItem *item, int column[[maybe_unused]])
{
    updateCurrentFocus(item);
}

void TextTreeWidget::on_coretreeWidget_itemChanged(QTreeWidgetItem *item[[maybe_unused]], int column[[maybe_unused]])
{
    QTreeWidgetItemIterator it(ui->coretreeWidget);
    while(*it)
    {
        manager.scanToUpdate(*it);
        ++it;
    }
}

TextTreeWidget::~TextTreeWidget()
{
    deleteAll(TextTreeWidget_DeletePolicy::DELETE_ALL);
    delete ui;
}


