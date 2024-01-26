#include "edittargetmember.h"
#include "ui_editTargetWindow.h""

#define FUNC_STR                        "函数变量"
#define PRIVATE_STR                     "设置为private"
#define PUBLIC_STR                      "设置为public"
#define PROTECTED_STR                   "设置为protected"
#define SET_PROPERTY_BTN_CAPTION        ui->btn_setVar_Property->setText(btn_caption)
#define SET_AS_STATIC_STR               "设置为static"
#define SET_AS_NO_STATIC_STR            "设置为non-static"
#define SET_STATIC_BTN_CAPTION          ui->btn_static->setText((is_static?SET_AS_STATIC_STR:SET_AS_NO_STATIC_STR))
#define SET_AS_CONST_STR                "设置为const"
#define SET_AS_NO_CONST_STR             "设置为non-const"
#define SET_CONST_BTN_CAPTION           ui->btn_const->setText((is_const?SET_AS_CONST_STR:SET_AS_NO_CONST_STR))

EditTargetMember::EditTargetMember(NewMember* edited, QString className, QWidget *parent):
    NewMemberWindow(className, parent)
{
    this->edited = edited;
    ui->setupUi(this);
    initBasic();
}


void EditTargetMember::initBasic()
{
    parseMember();
}

void EditTargetMember::parseMember()
{
    ui->name_edit->setText(this->edited->getName());
    ui->type_edit->setText(edited->getType());
    QString btn_caption;
    switch (edited->getMemberPropertyType()) {
    case NewMember::MEMBER_TYPE::FOR_PRIVATE:
        btn_caption = PUBLIC_STR;
        break;
    case NewMember::MEMBER_TYPE::FOR_PUBLIC:
        btn_caption = PRIVATE_STR;
        break;
    default:
        break;
    }

    bool is_const = edited->getIsConst();
    SET_CONST_BTN_CAPTION;

    bool is_static = edited->getIsStatic();
    SET_STATIC_BTN_CAPTION;

    ui->comment_Edit->setText(edited->getComment());

    ui->textBrowser->setText(edited->getGeneratePreview());
}
