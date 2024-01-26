#include "cmakeconfigmainwindow.h"
#include "qinputdialog.h"
#include "ui_cmakeconfigmainwindow.h"
#include "CMakeWindowUtils.h"
#include "cmakesentencegenerator.h"
#include "cmakeprojectcoreinfo.h"
SaveMonitor cmake_projectself_config_save_monitor;
const char* CMAKE_PROJECT_CONFIG_WINDOW             =   "CMake工程配置主窗口";

CMakeConfigMainWindow::CMakeConfigMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMakeConfigMainWindow)
{
    ui->setupUi(this);
    // setAutoClear
    AutoDelete;
    SetName(CMAKE_PROJECT_CONFIG_WINDOW);
}

void CMakeConfigMainWindow::refreshView()
{
    ui->viewer->setText(CMakeProjectGenerator(this->info).generate());
}

void CMakeConfigMainWindow::bindCurrentInfoPackage(CMakeProjectCoreInfo* info)
{
    if(!info)
        this->info = new CMakeProjectCoreInfo("Default_Name");
    else
        this->info = info;

    refreshView();
}


void CMakeConfigMainWindow::closeEvent(QCloseEvent*)
{
    if(!cmake_projectself_config_save_monitor.queryForLeave(this))
        return;
}

CMakeConfigMainWindow::~CMakeConfigMainWindow()
{
    delete ui;
}

void CMakeConfigMainWindow::on_btn_setLanguage_clicked()
{
    cmake_projectself_config_save_monitor.setShellSaveQuery();
    QString selectionLang = QInputDialog::getItem(this, "选择支持的语言", "选择支持的语言", enableLanuages);
    this->info->setProjectEnableLanguage(selectionLang);
    refreshView();
}


void CMakeConfigMainWindow::on_btn_setProjectName_clicked()
{
    cmake_projectself_config_save_monitor.setShellSaveQuery();
    QString pro_name = QInputDialog::getText(this, "设置工程名称", "设置工程名称");
    this->info->setProjectName(pro_name);
    refreshView();
}

void CMakeConfigMainWindow::on_btn_setProjectVersion_clicked()
{
    cmake_projectself_config_save_monitor.setShellSaveQuery();
    short major_set =
        QInputDialog::getInt(this,
                             "设置CMake主版本",
                             "设置CMake主版本", this->info->DEFAULT_PROJECT_MAJOR_VERSION, 0);

    short minor_set =
        QInputDialog::getInt(this,
                             "设置CMake主版本",
                             "设置CMake主版本", this->info->DEFAULT_PROJECT_MINOR_VERSION, 0);

    this->info->getInfos().project_major_version = major_set;
    this->info->getInfos().project_minor_version = minor_set;
    refreshView();
}

void CMakeConfigMainWindow::on_btn_setDes_clicked()
{
    cmake_projectself_config_save_monitor.setShellSaveQuery();
    QString des = QInputDialog::getText(this, "设置描述", "设置描述");
                                        this->info->getInfos().descriptions = des;
    refreshView();
}


void CMakeConfigMainWindow::on_buttonBox_accepted()
{
    this->close();
}


void CMakeConfigMainWindow::on_buttonBox_rejected()
{
    this->close();
}


void CMakeConfigMainWindow::on_btn_fresh_clicked()
{
    refreshView();
}

