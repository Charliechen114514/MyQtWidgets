#ifndef EDITFUNCTIONWINDOW_H
#define EDITFUNCTIONWINDOW_H

#include <QMainWindow>
#include "classlistandwidgetitemmanager.h"
#include <QCloseEvent>
class NewFunction;
class NewClass;
class NewFunctionWindow;
namespace Ui {
class EditFunctionWindow;
}



class EditFunctionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditFunctionWindow(NewClass* class_, QWidget *parent = nullptr);
    EditFunctionWindow() = delete;
    ~EditFunctionWindow();
    NewFunction* getCurrentFocusFunction();
    void doPreview();
    void showAddFunction();
    void showEditFunction();

signals:
    void emitFinishWork();

private slots:
    void successReceiveNewFunction(NewFunction* m);

    void closeEvent(QCloseEvent*);

    void on_btn_newMethod_clicked();

    void on_listsOfMember_itemClicked(QListWidgetItem *item);

    void on_btn_editFunction_clicked();

private:
    Ui::EditFunctionWindow *ui;
    NewFunction* func                       = nullptr;
    NewClass*    bindingClass               = nullptr;
    ClassListAndWidgetItemManager   manager;
    NewFunctionWindow*    newFunctionWindow = nullptr;
    bool                    isEdit          = false;
    NewFunction*            isOnEdit        = nullptr;
    void initManager();
    void switchToCurrentFunction(QListWidgetItem* item);
};

#endif // EDITFUNCTIONWINDOW_H
