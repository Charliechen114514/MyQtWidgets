#include "savingconfigwindow.h"
#include "ui_savingconfigwindow.h"
#include "savepathgenerator.h"
#include "utils.h"


SavingConfigWindow::SavingConfigWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SavingConfigWindow)
{
    ui->setupUi(this);
}

SavingConfigWindow::~SavingConfigWindow()
{
    delete ui;
}

void SavingConfigWindow::disAbleAll()
{
    DISABLE_WIDGET(srin_ConfigWidget);
    DISABLE_WIDGET(allinone_ConfigWidget);
}

void SavingConfigWindow::moveToDefault()
{
    saveOption = SAVE_OPTION::SRIN;
    ENABLE_WIDGET(srin_ConfigWidget);
    DISABLE_WIDGET(allinone_ConfigWidget);
}

void SavingConfigWindow::closeEvent(QCloseEvent*)
{
    if(saveOption != SAVE_OPTION::PREVIEW)
    {
        if(!isUserDecideHisDecision(this, "保存配置? ", "保存你的配置嘛？","", true))
        {
            saveOption = SAVE_OPTION::CANCLE;
        }
    }

    emit finishEditSavingConfig();
}

void SavingConfigWindow::on_btn_setSRIN_clicked()
{
    saveOption = SAVE_OPTION::SRIN;
    ENABLE_WIDGET(srin_ConfigWidget);
    DISABLE_WIDGET(allinone_ConfigWidget);
}


void SavingConfigWindow::on_btn_setAll_clicked()
{
    saveOption = SAVE_OPTION::ALL;
    DISABLE_WIDGET(srin_ConfigWidget);
    ENABLE_WIDGET(allinone_ConfigWidget);
}


void SavingConfigWindow::on_btn_setHeader_clicked()
{
    QString headerPath;
    if(!SavePathGenerator::getSavingDir(this, headerPath))
    {
        return;
    }

    ui->rdonly_header_path_lineEdit->setText(headerPath);
    this->headerPath = headerPath;
}

void SavingConfigWindow::clearItems()
{
    for(QTreeWidgetItem* each : this->items)
    {
        if(each)
            delete each;
        each = nullptr;
    }
    ui->treeWidget->clear();
}

void SavingConfigWindow::on_btn_setSource_clicked()
{
    QString sourcePath;
    if(!SavePathGenerator::getSavingDir(this, sourcePath))
    {
        return;
    }

    ui->rdonly_source_path_lineEdit->setText(sourcePath);
    this->sourcePath = sourcePath;
}

void SavingConfigWindow::on_btn_setAllPath_clicked()
{
    QString allpath;
    if(!SavePathGenerator::getSavingDir(this, allpath))
    {
        return;
    }

    ui->allpath_edit->setText(allpath);
    this->allPath = allpath;
}



void SavingConfigWindow::makePreview(QMap<QString, QStringList>* map)
{
    if(map == nullptr)
        return;
    clearItems();
    QStringList classesName = map->keys();
    for(QString& eachClass : classesName)
    {
        QTreeWidgetItem* it = new QTreeWidgetItem(ui->treeWidget);
        it->setText(0, eachClass);
        it->setText(1, map->value(eachClass, QStringList("unknown","unknown"))[0]);
        it->setText(2, map->value(eachClass, QStringList("unknown","unknown"))[1]);
        it->setToolTip(1, map->value(eachClass, QStringList("unknown","unknown"))[0]);
        it->setToolTip(2, map->value(eachClass, QStringList("unknown","unknown"))[1]);
        items.push_back(it);
    }
}


