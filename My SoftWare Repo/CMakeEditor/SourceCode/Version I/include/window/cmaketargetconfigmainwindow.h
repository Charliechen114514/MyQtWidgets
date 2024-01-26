#ifndef CMAKETARGETCONFIGMAINWINDOW_H
#define CMAKETARGETCONFIGMAINWINDOW_H

#include "qlistwidget.h"
#include <QMainWindow>
#include <CMakeUtilsMainWindowBase.h>

class CMakeProjectCoreInfo;
class ClassListAndWidgetItemManager;
class TargetSourceEditingWindow;
class QTableWidgetItem;
namespace Ui {
class CMakeTargetConfig;
}

class CMakeTargetConfigMainWindow : public QMainWindow, public CMakeUtilsMainWindowBase
{
    Q_OBJECT

public:
    struct tmpTargetLib{
        QString name;
        QStringList src;
        enum class LibType{
            SHARED,
            STATIC,
            MODULE
        }libType = LibType::STATIC;
        friend bool operator==(const tmpTargetLib& a, QString b){
            return a.name == b;
        }
    };

    struct tmpTargetExe{
        QString name;
        QStringList src;
        friend bool operator==(const tmpTargetExe& a, QString b){
            return a.name == b;
        }
    };

    enum class TargetType{
        LIB,
        EXE
    };

    const QString LIB_TYPE = "库";
    const QString EXE_TYPE = "可执行文件";

    explicit CMakeTargetConfigMainWindow(QWidget *parent = nullptr);
    void bindProjectCoreinfo(CMakeProjectCoreInfo* t);
    ~CMakeTargetConfigMainWindow();
    void addTarget(QString name, TargetType type);
    void removeTarget(QString name, TargetType type);
    void closeEvent(QCloseEvent*);
    void showConfigSourceWindow(QStringList* l);

    void updateViewSourceUi();

signals:
    void updateCurrentNameShow(QString name);

private slots:
    void updateTextBrowserNameShown(QString name);

    void handleTheDestroyWindow();

    void on_btn_add_src_clicked();

    void on_listbtn_switch_target_itemClicked(QListWidgetItem *item);

    void on_btn_remove_src_clicked();

    void on_btn_config_source_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CMakeTargetConfig *                     ui;
    CMakeProjectCoreInfo*                       buildTarget = nullptr;
    ClassListAndWidgetItemManager*              listManager = nullptr;
    TargetSourceEditingWindow*                  targetConfigWindow = nullptr;
    QList<tmpTargetExe*>                        exes;
    QList<tmpTargetLib*>                        lib;
    QList<QTableWidgetItem*>                    items;
    void                                        init();
    void                                        initConnections();
    void                                        initMyUi();
    void                                        addTargetAtClose();
    void                                        manageTableWidget(QStringList* l);
    void                                        initItems(QStringList* l);
    void                                        clearItems();
};

#endif // CMAKETARGETCONFIGMAINWINDOW_H
