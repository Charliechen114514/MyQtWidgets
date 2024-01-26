#ifndef NEWMEMBERWINDOW_H
#define NEWMEMBERWINDOW_H

#include <QMainWindow>
#include "newmember.h"
#include <QCloseEvent>
namespace Ui {
class NewMemberWindow;
}

class NewMemberWindow : public QMainWindow
{
    Q_OBJECT

public:
    // disbale all defaults
    NewMemberWindow() = delete;
    const NewMemberWindow& operator=(const NewMemberWindow&) = delete;
    NewMemberWindow(const NewMemberWindow&) = delete;

    explicit NewMemberWindow(QString belongClass, QWidget *parent = nullptr);
    ~NewMemberWindow();

    void setIsClassMember(bool state){this->is_class_member = state;};

signals:
    void success_construct_member(NewMember* on_constructing_newMember);
    void success_construct_param(NewMember* param);
private slots:
    void on_name_edit_returnPressed();

    void on_type_edit_returnPressed();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void closeEvent(QCloseEvent*) override;
    void on_btn_preview_clicked();

    void on_btn_setVar_Property_clicked();

    void on_btn_static_clicked();

    void on_btn_const_clicked();

    void on_type_edit_textEdited(const QString &arg1);

    void on_name_edit_textEdited(const QString &arg1);

    void on_comment_Edit_textChanged();

protected:
    Ui::NewMemberWindow     *ui;
    void                    enableCommon();
    void                    enableSetVarProperty();
    void                    disableAll();
    void                    switchNewMember(QString name, QString type);
    void                    collectInfo();
    void                    doPreview();

private:
    NewMember*              on_constructing_newMember = nullptr;
    NewMember::MEMBER_TYPE  propertyType    = NewMember::MEMBER_TYPE::FOR_FUNC;
    int                     clickTimes      = static_cast<int>(NewMember::MEMBER_TYPE::FOR_PRIVATE);
    bool                    is_construct    = false;
    bool                    is_class_member = true;
    bool                    is_static       = false;
    bool                    is_const        = false;
    bool                    enableClick     = true;
    QString                 belongClass;
    QString                 type;
};


#endif // NEWMEMBERWINDOW_H
