#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CMakeSelfInfo;
class CMakeProjectCoreInfo;
class BuildTarget;
class CMakeUtilsMainWindowBase;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void refresh();
    ~MainWindow();

private slots:
    void handleTheDestroyObject(QObject* obj);

    void on_btn_CMakeSelfConfig_clicked();

    void on_btn_setProjectProperty_clicked();

    void on_btn_refresh_clicked();

    void on_btn_add_target_clicked();

    void on_btn_generate_clicked();

private:
    Ui::MainWindow                      *ui;
    QList<CMakeUtilsMainWindowBase*>    configWindows;
    CMakeUtilsMainWindowBase*           curOperateWindow = nullptr;
    CMakeSelfInfo*                      cmake_self_infos = nullptr;
    CMakeProjectCoreInfo*               projectCore_infos = nullptr;

    BuildTarget*                        currentBuild = nullptr;
    void                                initStyle();
    void                                initCoreInfo();
};


#endif // MAINWINDOW_H
