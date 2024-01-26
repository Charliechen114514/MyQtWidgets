#ifndef CMAKECONFIGMAINWINDOW_H
#define CMAKECONFIGMAINWINDOW_H

#include <QMainWindow>
#include "CMakeUtilsMainWindowBase.h"
#include <QCloseEvent>
namespace Ui {
class CMakeConfigMainWindow;
}

class CMakeProjectCoreInfo;

class CMakeConfigMainWindow : public QMainWindow, public CMakeUtilsMainWindowBase
{
    Q_OBJECT

public:
    explicit CMakeConfigMainWindow(QWidget *parent = nullptr);
    virtual ~CMakeConfigMainWindow();

    virtual void closeEvent(QCloseEvent*);
    void bindCurrentInfoPackage(CMakeProjectCoreInfo* info);

    void refreshView();

private slots:

    void on_btn_setLanguage_clicked();

    void on_btn_setProjectName_clicked();

    void on_btn_setProjectVersion_clicked();

    void on_btn_setDes_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();


    void on_btn_fresh_clicked();

private:
    Ui::CMakeConfigMainWindow*      ui;
    CMakeProjectCoreInfo*           info = nullptr;
    QStringList                     enableLanuages{"C", "CXX", "C CXX"};
};

#endif // CMAKECONFIGMAINWINDOW_H
