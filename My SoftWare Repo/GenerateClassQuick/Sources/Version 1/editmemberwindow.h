#ifndef EDITMEMBERWINDOW_H
#define EDITMEMBERWINDOW_H

#include <QMainWindow>
#include "classlistandwidgetitemmanager.h"
#include <QCloseEvent>
class NewClass;
class NewMember;
class NewMemberWindow;
class EditTargetMember;
namespace Ui {
class EditMemberWindow;
}

class EditMemberWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditMemberWindow(NewClass* _class_, QWidget *parent = nullptr);
    ~EditMemberWindow();
    NewMember* getCurrentFocusMember();
    void doPreview();
    bool eraseFocusMember();
    void showAddMember();
    void showEditMember();
signals:
    void emitFinishWork();

private slots:
    void closeEvent(QCloseEvent*);

    void successReceiveNewMember(NewMember* m);

    void on_listsOfMember_itemClicked(QListWidgetItem *item);

    void on_btn_addMember_clicked();

    void on_btn_editMember_clicked();

private:
    Ui::EditMemberWindow            *ui;
    NewClass*                       bindingClass = nullptr;
    ClassListAndWidgetItemManager   manager;
    NewMemberWindow*                newMemberWindow = nullptr;
    EditTargetMember*               editTagertWindow = nullptr;
    void initManager();
    void switchTargetMember(QListWidgetItem* item);
    bool                            isEdit = false;
    NewMember*                      isOnEdit = nullptr;
};

#endif // EDITMEMBERWINDOW_H
