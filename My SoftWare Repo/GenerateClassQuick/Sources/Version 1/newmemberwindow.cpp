#include "newmemberwindow.h"
#include "ui_newmemberwindow.h"
#include "utils.h"
#define DEFAULT_TYPE    "int"

#define VALID_APPEND_CLICK      \
        clickTimes++;\
        if(clickTimes > static_cast<int>(NewMember::MEMBER_TYPE::TYPE_INDEX_MAX) - 1) \
            clickTimes = static_cast<int>(NewMember::MEMBER_TYPE::FOR_PRIVATE)
#define ENABLE_CLICK                    this->enableClick = true;
#define DISABLE_CLICK                   this->enableClick = false;

#define TO_INT(type)                    static_cast<int>(NewMember::MEMBER_TYPE::type)
#define REFLECT_PROPERTY_TYPE           static_cast<NewMember::MEMBER_TYPE>(clickTimes)
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


NewMemberWindow::NewMemberWindow(QString belongClass, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewMemberWindow)
{
    ui->setupUi(this);
    this->belongClass = belongClass;
    this->type = DEFAULT_TYPE;
}

void NewMemberWindow::enableCommon()
{
    ENABLE_WIDGET(btn_const);
    ENABLE_WIDGET(btn_static);
    ENABLE_WIDGET(btn_preview);
}



// 允许设置变量的private public 还是protected[TODO]属性
void NewMemberWindow::enableSetVarProperty()
{
    if(this->is_class_member)
    {
        ENABLE_WIDGET(btn_setVar_Property);
        on_btn_setVar_Property_clicked();
    }
}

void NewMemberWindow::disableAll()
{
    ENABLE_WIDGET(btn_const);
    ENABLE_WIDGET(btn_static);
    ENABLE_WIDGET(btn_preview);
    ENABLE_WIDGET(btn_setVar_Property);
}


void NewMemberWindow::switchNewMember(QString name, QString type)
{
    if(this->on_constructing_newMember == nullptr)
        this->on_constructing_newMember = new NewMember(this->belongClass, name, type);
    else
        this->on_constructing_newMember->setMemberName(name);
}

NewMemberWindow::~NewMemberWindow()
{
    delete ui;
}

void NewMemberWindow::on_name_edit_returnPressed()
{
    QString res = lowerFirst(ui->name_edit->text());
    qDebug() << "Member name get:> " << res;
    REJECT_EMPTY_TEXT(res, "不可以为空", "哥们空的，搞不了的");
    switchNewMember(res, this->type);
    enableCommon();
    DISABLE_CLICK;
    enableSetVarProperty();
    ENABLE_CLICK;
}


void NewMemberWindow::on_type_edit_returnPressed()
{
    QString res = ui->type_edit->text();
    qDebug() << "Member type get:> " << res;
    REJECT_EMPTY_TEXT(res, "不可以为空", "哥们空的，搞不了的");
    this->type = res;
}


void NewMemberWindow::on_buttonBox_accepted()
{
    if(this->on_constructing_newMember == nullptr)
    {
        on_buttonBox_rejected();
        return;
    }
    // 用户认为它需要此次构建
    CONSIDER_RECEIVE;
    // 关闭之
    this->close();
}

void NewMemberWindow::on_buttonBox_rejected()
{
    // 用户认为它需要此次构建
    CONSIDER_REJECT;
    // 关闭之
    this->close();
}

void NewMemberWindow::closeEvent(QCloseEvent* env)
{
    if(!isUserDecideHisDecision(this, "确定疑问","保留你的构建", "确定退出？所有记录都会消失", is_construct))
    {
        env->ignore(); // 不取消
        return;
    }

    if(is_construct)
    {
        if(is_class_member)
        {
            emit this->success_construct_member(this->on_constructing_newMember);
        }
        else
        {
            emit this->success_construct_param(this->on_constructing_newMember);
        }
    }
    else
    {
        qDebug() << "is_construct == false, 抛弃构建";
    }
}

void NewMemberWindow::collectInfo()
{
    this->on_constructing_newMember->setComment(ui->comment_Edit->toPlainText());
    this->on_constructing_newMember->setMemberType(ui->type_edit->text());
}

void NewMemberWindow::doPreview()
{
    collectInfo();
    this->ui->textBrowser->setText(
        this->on_constructing_newMember->getGeneratePreview()
    );
}


void NewMemberWindow::on_btn_preview_clicked()
{
    doPreview();
}

void NewMemberWindow::on_btn_setVar_Property_clicked()
{
    // is params no need to set
    QString btn_caption;
    if(!is_class_member)
    {
        this->on_constructing_newMember->setMemberPropertyType(NewMember::MEMBER_TYPE::FOR_FUNC);
        btn_caption = FUNC_STR;
    }
    else
    {
        if(enableClick)
        {
            VALID_APPEND_CLICK;
        }
        switch (clickTimes) {
        case TO_INT(FOR_PRIVATE):
            qDebug() << "member is private";
            this->on_constructing_newMember->setMemberPropertyType(NewMember::MEMBER_TYPE::FOR_PRIVATE);
            btn_caption = PRIVATE_STR;
            break;
        case TO_INT(FOR_PUBLIC):
            qDebug() << "member is public";
            this->on_constructing_newMember->setMemberPropertyType(NewMember::MEMBER_TYPE::FOR_PUBLIC);
            btn_caption = PUBLIC_STR;
            break;
        default:
            qDebug() << "FALLS ON UNKNOWN CLICK_TIME - CAPTION_STR! check it out!";
            qDebug() << "clickTimes" << clickTimes << " - "<< static_cast<int>(NewMember::MEMBER_TYPE::TYPE_INDEX_MAX);
            btn_caption = FUNC_STR;
        break;
        }
    }
    SET_PROPERTY_BTN_CAPTION;
    doPreview();
}


void NewMemberWindow::on_btn_static_clicked()
{
    is_static = !is_static;
    this->on_constructing_newMember->setStatic(is_static);
    SET_STATIC_BTN_CAPTION;
    doPreview();
}

void NewMemberWindow::on_btn_const_clicked()
{
    is_const = !is_const;
    this->on_constructing_newMember->setConst(is_const);
    SET_CONST_BTN_CAPTION;
    doPreview();
}


void NewMemberWindow::on_type_edit_textEdited(const QString &arg1)
{
    if(arg1.isEmpty() || this->ui->name_edit->text().isEmpty())
    {
        return;
    }
    else
    {
        doPreview();
    }
}


void NewMemberWindow::on_name_edit_textEdited(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        return;
    }
    else
    {
        QString res = lowerFirst(arg1);
        qDebug() << "Member name get:> " << res;
        REJECT_EMPTY_TEXT(res, "不可以为空", "哥们空的，搞不了的");
        switchNewMember(res, this->type);
        enableCommon();
        DISABLE_CLICK;
        enableSetVarProperty();
        ENABLE_CLICK;
        doPreview();
    }
}


void NewMemberWindow::on_comment_Edit_textChanged()
{
    doPreview();
}

