#ifndef NEWCLASSWINDOW_H
#define NEWCLASSWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "newfunctionwindow.h"
#include "newmemberwindow.h"
#include "newclass.h"
namespace Ui {
class NewClassWindow;
}

class NewClassWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewClassWindow(QWidget *parent = nullptr);
    ~NewClassWindow();

    enum class CONSTRUCTOR_INDEX{
        DISABLE_ALL_DEF_CONSTRUCT,
        DISABLE_DEF_ONLY,
        DISABLE_COPY_ONLY,
        ALLOW_ALL
    };

    // deal with closeEbvent
    void closeEvent(QCloseEvent*) override;


signals:
    // closeEvents
    void success_setClass(NewClass* settingClass);

private slots:
    void receive_setMember(NewMember* member);

    void receive_setFunction(NewFunction* func);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_className_lineEdit_returnPressed();

    void on_comboBox_activated(int index);

    void on_btn_preview_clicked();

    void on_btn_add_member_clicked();

    void on_btn_add_function_clicked();

    void on_className_lineEdit_textEdited(const QString &arg1);

    void on_comment_edit_textChanged();

private:
    Ui::NewClassWindow  *ui;
    NewMemberWindow*    memberWindow            = nullptr;
    NewFunctionWindow*  functionWindow          = nullptr;
    // 此变量维护的是用户是否需要构建这个类（点击确定方才认为是构建，余下所有键都将会被认为被抛弃！）
    bool                is_construct             = false;
    NewClass*           constructing_newClass   = nullptr;
    QList<NewMember>    members;
    bool                initConnectionWithNewMemberWindow();
    bool                initConnectionWithNewFunctionWindow();
    void                initStyles();
    void                switchTheNewClass(QString newName);
    void                enableSettings();
    void                disableSettings();
    void                collectInfos();
    void                doPreview();
};

#endif // NEWCLASSWINDOW_H
