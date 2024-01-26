#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileDialog>

#include "targetsourceeditingwindow.h"
#include "ui_targetsourceeditingwindow.h"
#include "CMakeWindowUtils.h"

const char* CMAKE_TARGET_SOURCE_CONFIG = "配置资源窗口";
SaveMonitor cmake_config_target_source_savemonitor;

TargetSourceEditingWindow::TargetSourceEditingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TargetSourceEditingWindow)
{
    ui->setupUi(this);
    AutoDelete;
    SetName(CMAKE_TARGET_SOURCE_CONFIG);
    initConnection();
    ui->show_config->resizeColumnToContents(1);
}

void TargetSourceEditingWindow::initConnection()
{}

void TargetSourceEditingWindow::bindBuildSrc(QStringList* srcList)
{
    this->shownLists = srcList;
    if(!srcList)
        return;
    scanAndGenerateItems();
    updateUi();
}

void TargetSourceEditingWindow::updateUi()
{
    ui->show_config->setRowCount(tableItems.size());
    for(int i = 0; i < tableItems.size(); i++)
        ui->show_config->setItem(i, 0, tableItems[i]);
}

QStringList TargetSourceEditingWindow::selectByFileDialog()
{
    return QFileDialog::getOpenFileNames(this, "选择资源文件", UsablePath::getDesktopPath(), enableFile);
}


void TargetSourceEditingWindow::scanAndGenerateItems()
{
    for(QString& e : *shownLists)
    {
        QTableWidgetItem* item = new QTableWidgetItem(e);

        item->setToolTip(e);

        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        this->tableItems.push_back(item);
    }

}

void TargetSourceEditingWindow::clearItems()
{
    for(QTableWidgetItem* each : tableItems)
        delete each, each = nullptr;
    this->tableItems.clear();
    ui->show_config->setRowCount(0);
}

void TargetSourceEditingWindow::configSource(QStringList l, bool need_refresh)
{
    if(need_refresh)
    {
        clearListsSource();
    }

    *shownLists << l;
    clearItems();
    scanAndGenerateItems();
    updateUi();
}

void TargetSourceEditingWindow::closeEvent(QCloseEvent*)
{
    if(!cmake_config_target_source_savemonitor.queryForLeave(this))
        return;
}


TargetSourceEditingWindow::~TargetSourceEditingWindow()
{
    clearItems();
    delete ui;
}

void TargetSourceEditingWindow::on_show_config_itemClicked(QTableWidgetItem *item)
{
    setFocusItems(item);
}


void TargetSourceEditingWindow::on_btn_append_src_clicked()
{
    QStringList l = selectByFileDialog();
    if(l.isEmpty())
        return;
    cmake_config_target_source_savemonitor.setShellSaveQuery();
    configSource(l, false);
}

void TargetSourceEditingWindow::on_btn_remove_src_clicked()
{
    if(!currentFocus)
    {
        QMessageBox::critical(this, "没有选中资源","没有选中资源");
        return;
    }
    cmake_config_target_source_savemonitor.setShellSaveQuery();
    this->shownLists->removeOne(currentFocus->text());
    tableItems.removeOne(currentFocus);
    delete currentFocus;
    currentFocus = nullptr;
    updateUi();
}


void TargetSourceEditingWindow::on_btn_resetSources_clicked()
{
    QStringList l = selectByFileDialog();
    if(l.isEmpty())
        return;
    cmake_config_target_source_savemonitor.setShellSaveQuery();
    configSource(l, true);
}


void TargetSourceEditingWindow::on_buttonBox_accepted()
{
    this->close();
}


void TargetSourceEditingWindow::on_buttonBox_rejected()
{
    this->close();
}

