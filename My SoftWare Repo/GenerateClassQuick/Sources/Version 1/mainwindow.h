#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "classlistandwidgetitemmanager.h"
#include "exportmanager.h"
class EditClassWindow;
class NewMemberWindow;
class NewFunctionWindow;
class QListWidgetItem;
class NewClass;
class NewMember;
class NewFunction;
class NewClassWindow;
class SavingConfigWindow;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void addClassWidgetItemToList(QString name);
    void buildClass();
    void buildFuncToClass();
    void buildMemberToClass();
    void switchTargetClass(QListWidgetItem* item);
    void makePreview(NewClass* whatClass);
    void eraseCurrentClass();
    void switchQSS(QString path);
    void toLightView();
    void toDarkView();
    void moveToNext();
    void moveToPrevious();
    void showCurrentClassHeader();
    void showCurrentClassSource();
    NewClass* getCurrentNewClass();
    void prepareExport();
    void showEditClassWindow();
    void showConfigWindow();
    void previewSave();

    void exportToCmake();
    void exportNormal();
    ~MainWindow();

private slots:
    void    successSetClass(NewClass* settingClass); // signals are newClass::success_set_class_signal

    void    successSetMember(NewMember* member);

    void    successSetFunction(NewFunction* func);

    void    successGetEditedClass();

    void    on_btn_setClassFirst_clicked();

    void    on_btn_showHeader_clicked();

    void    on_btn_showSource_clicked();

    void    on_classList_itemClicked(QListWidgetItem *item);

    void    on_btn_add_member_clicked();

    void    on_btn_add_function_clicked();

    void    on_btn_clear_current_clicked();

    void    on_btn_next_clicked();

    void    on_btn_pre_clicked();

    void    on_btn_composeCurrentClass_clicked();

    void    on_btn_refresh_clicked();

    void    keyPressEvent(QKeyEvent*);

private:
    Ui::MainWindow *ui;
    NewClassWindow*     newClassWindow      = nullptr;
    NewFunctionWindow*  newFunctionWindow   = nullptr;
    NewMemberWindow*    newMemberWindow     = nullptr;
    bool                isClassSet_Already  = false; // for new Class Windows signal
    QList<NewClass*>    Classes;
    ClassListAndWidgetItemManager managerGlobal;
    ExportManager*      manager             = nullptr;
    EditClassWindow*    editClassWindow     = nullptr;
    SavingConfigWindow* configWindow        = nullptr;
    bool                isFinishConfig       = false;
    void                initClassesListWidget();
    bool                initBasicConnection();
    bool                initConnectionWithNewClassWindow();
    bool                initMemberWindow();
    bool                initFunctionWindow();
    void                loadDefaultStyleSheet();
    void                initMenuConnections();
    void                processSetClassExportPath();
    void                beginExport();
    void                endExport();
    void                makeExport(ExportBuilderMethod& method);
};
#endif // MAINWINDOW_H
