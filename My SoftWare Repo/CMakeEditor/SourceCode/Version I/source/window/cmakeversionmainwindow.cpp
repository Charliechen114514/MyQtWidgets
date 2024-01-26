#include <QInputDialog>
#include "cmakeversionmainwindow.h"
#include "ui_cmakeversionmainwindow.h"
#include "CMakeWindowUtils.h"
#include "cmakeselfinfo.h"
#include "cmakesentencegenerator.h"
SaveMonitor cmake_version_config_save_monitor;
const char* CMAKE_SELF_CONFIG_WINDOW_MAIN           =   "CMake自身配置设置主窗口";

CMakeVersionMainWindow::CMakeVersionMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMakeVersionMainWindow)
{
    ui->setupUi(this);
    AutoDelete;
    SetName(CMAKE_SELF_CONFIG_WINDOW_MAIN);
}

void CMakeVersionMainWindow::refresh()
{
    ui->viewer->setText(CMakeSelfInfoSentenseGenerator(this->info).generate());
}

void CMakeVersionMainWindow::closeEvent(QCloseEvent*)
{
    if(!cmake_version_config_save_monitor.queryForLeave(this))
        return;
}

void CMakeVersionMainWindow::bindCMakeSelfInfo(CMakeSelfInfo* info)
{
    if(!info)
        this->info = new CMakeSelfInfo;
    else
        this->info = info;

}

void CMakeVersionMainWindow::CMakeVersionConfigInputDialogQuery()
{
    cmake_version_config_save_monitor.setShellSaveQuery();
    short major_set =
        QInputDialog::getInt(this,
                             "设置Project主版本",
                             "设置Project主版本", this->info->DEFAULT_CMAKE_MAJOR_VERSION, 0);

    short minor_set =
        QInputDialog::getInt(this,
                             "设置Project主版本",
                             "设置Project主版本", this->info->DEFAULT_CMAKE_MINOR_VERSION, 0);

    this->info->setCMakeVersion(major_set, minor_set);

    // view
    refresh();
}

CMakeVersionMainWindow::~CMakeVersionMainWindow()
{
    delete ui;
}

void CMakeVersionMainWindow::on_btn_CMakeVersion_clicked()
{
    CMakeVersionConfigInputDialogQuery();
}


void CMakeVersionMainWindow::on_btn_fresh_clicked()
{
    refresh();
}


void CMakeVersionMainWindow::on_buttonBox_accepted()
{
    this->close();
}


void CMakeVersionMainWindow::on_buttonBox_rejected()
{
    this->close();
}

