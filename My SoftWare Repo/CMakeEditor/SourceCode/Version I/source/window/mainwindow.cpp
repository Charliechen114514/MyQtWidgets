#include <QInputDialog>
#include "CMakeWindowUtils.h"
#include "mainwindow.h"
#include "cmakeconfigmainwindow.h"
#include "cmakeselfinfo.h"
#include "cmakesentencegenerator.h"
#include "cmakeversionmainwindow.h"
#include "cmaketargetconfigmainwindow.h"
#include "qfiledialog.h"
#include "ui_mainwindow.h"
#include "CMakeUtilsMainWindowBase.h"
#include "qssmanager.h"
const char* CMake_MainWindow = "CMake编辑器";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initCoreInfo();
    initStyle();
    SetName(CMake_MainWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    if(!this->cmake_self_infos || !this->projectCore_infos)
        return;
    // cmake one
    ui->plainText->setPlainText(
        CMakeSelfInfoSentenseGenerator(this->cmake_self_infos).generate() +
        CMakeProjectGenerator(this->projectCore_infos).generate() +
        CMakeAddWholeGenerator(this->projectCore_infos).generate());
}

void MainWindow::initCoreInfo()
{
    this->projectCore_infos = new CMakeProjectCoreInfo("Default_Name");
    this->cmake_self_infos = new CMakeSelfInfo();
}

void MainWindow::initStyle()
{
    QSSManager m;
    m.loadDefaulltQSS(this);
}


void MainWindow::handleTheDestroyObject(QObject* obj)
{
    // might be unsafe, but just remove a pointer hah
    CMakeUtilsMainWindowBase* m = reinterpret_cast<CMakeUtilsMainWindowBase*>(obj);
    this->configWindows.removeOne(m);
    this->refresh();
}

void MainWindow::on_btn_CMakeSelfConfig_clicked()
{
    curOperateWindow = CMakeUtilsMainWindowProducer::makeWindow(this,
        CMakeUtilsMainWindowProducer::Window_TYPE::CMAKE_SELF_CONFIG_WINDOW);
    CMakeVersionMainWindow* window = static_cast<CMakeVersionMainWindow*>(curOperateWindow);
    this->configWindows.push_back(window);

    connect(window, &CMakeVersionMainWindow::destroyed, this, &MainWindow::handleTheDestroyObject);
    window->bindCMakeSelfInfo(this->cmake_self_infos);
    window->show();
}


void MainWindow::on_btn_setProjectProperty_clicked()
{
    curOperateWindow = CMakeUtilsMainWindowProducer::makeWindow(this,
                                                                CMakeUtilsMainWindowProducer::Window_TYPE::CMAKE_PROJECT_CONFIG_SELF_WINDOW);
    CMakeConfigMainWindow* window = static_cast<CMakeConfigMainWindow*>(curOperateWindow);
    this->configWindows.push_back(window);

    connect(window, &CMakeConfigMainWindow::destroyed, this, &MainWindow::handleTheDestroyObject);
    window->bindCurrentInfoPackage(this->projectCore_infos);
    window->show();
}


void MainWindow::on_btn_refresh_clicked()
{
    refresh();
}



void MainWindow::on_btn_add_target_clicked()
{
    curOperateWindow = CMakeUtilsMainWindowProducer::makeWindow(this, CMakeUtilsMainWindowProducer::Window_TYPE::CMAKE_CONFIG_TARGET_WINDOW);
    CMakeTargetConfigMainWindow* window = static_cast<CMakeTargetConfigMainWindow*>(curOperateWindow);
    connect(window, &CMakeTargetConfigMainWindow::destroyed, this, &MainWindow::handleTheDestroyObject);
    window->bindProjectCoreinfo(this->projectCore_infos);
    window->show();
}

void MainWindow::on_btn_generate_clicked()
{
    QString savingPlace = QFileDialog::getExistingDirectory(this, "选择生成CMake的文件夹", UsablePath::getDesktopPath());
    if(savingPlace.isEmpty()){
        return;
    }
    savingPlace += "/CMakeLists.txt";
    QFile f(savingPlace);
    f.open(QIODevice::WriteOnly);
    if(f.isOpen())
    {
        f.write(ui->plainText->toPlainText().toStdString().c_str());
    }
    f.close();
    QMessageBox::information(this, "成功", "成功写入" + savingPlace);
}

