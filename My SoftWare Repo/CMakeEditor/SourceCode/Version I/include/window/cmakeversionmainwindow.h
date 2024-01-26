#ifndef CMAKEVERSIONMAINWINDOW_H
#define CMAKEVERSIONMAINWINDOW_H

#include "CMakeUtilsMainWindowBase.h"
#include <QMainWindow>

namespace Ui {
class CMakeVersionMainWindow;
}


class CMakeSelfInfo;
class CMakeVersionMainWindow : public QMainWindow, public CMakeUtilsMainWindowBase
{
    Q_OBJECT

public:
    explicit CMakeVersionMainWindow(QWidget *parent = nullptr);
    ~CMakeVersionMainWindow();
    // configs
    void                            bindCMakeSelfInfo(CMakeSelfInfo* info);


    // window events
    void                            closeEvent(QCloseEvent*);

    void                            refresh();

private slots:
    void on_btn_CMakeVersion_clicked();


    void on_btn_fresh_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    void CMakeVersionConfigInputDialogQuery();


private:
    Ui::CMakeVersionMainWindow*     ui;
    CMakeSelfInfo*                  info = nullptr;
};

#endif // CMAKEVERSIONMAINWINDOW_H
