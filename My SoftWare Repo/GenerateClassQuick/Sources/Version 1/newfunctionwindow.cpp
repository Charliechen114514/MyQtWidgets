#include "newfunctionwindow.h"
#include "ui_newfunctionwindow.h"
#include "utils.h"
#define DEF_RET_TYPE                    "void"
#define ADD_EXPLICIT_TEXT               "添加explicit限定符"
#define ERASE_EXPLICIT_TEXT             "取消explicit限定符"
#define ADD_CONST_TEXT                  "设置为const"
#define ERASE_CONST_TEXT                "取消const"
#define ADD_VIRTUAL_TEXT                "设置为virtual"
#define ERASE_VIRTUAL_TEXT              "取消virtual"
#define ADD_STATIC_TEXT                 "设置为static"
#define ERASE_STATIC_TEXT               "取消static"
#define SET_BIN_STATE_BTN_CAPTION(btn, state, tr_str, fl_str) \
                                        ui->btn->setText((state?tr_str:fl_str))

#define VALID_APPEND_CLICK      \
    clickTimes++;\
    if(clickTimes > static_cast<int>(NewFunction::FUNC_TYPE::TYPE_INDEX_MAX) - 1) \
        clickTimes = 0
#define TO_INT(type)                    static_cast<int>(NewFunction::FUNC_TYPE::type)
#define REFLECT_PROPERTY_TYPE           static_cast<NewFunction::FUNC_TYPE>(clickTimes)
#define PRIVATE_STR                     "作用域设置为private"
#define PUBLIC_STR                      "作用域设置为public"
#define PROTECTED_STR                   "作用域设置为protected"
#define SET_PROPERTY_BTN_CAPTION        ui->btn_varProperty->setText(btn_caption)



NewFunction::Arg* analazyFromMemberToArg(NewMember* member)
{
    return new NewFunction::Arg(member->getType(), member->getName(), member->getIsConst(), member->getComment());
}

void NewFunctionWindow::enableAll()
{
    ENABLE_WIDGET(btn_addExplicit);
    ENABLE_WIDGET(btn_addParam);
    ENABLE_WIDGET(btn_addVirtual);
    ENABLE_WIDGET(btn_add_const);
    ENABLE_WIDGET(btn_preview);
    ENABLE_WIDGET(btn_set_static);
    ENABLE_WIDGET(btn_varProperty);
}

void NewFunctionWindow::disableAll()
{
    DISABLE_WIDGET(btn_addExplicit);
    DISABLE_WIDGET(btn_addParam);
    DISABLE_WIDGET(btn_addVirtual);
    DISABLE_WIDGET(btn_add_const);
    DISABLE_WIDGET(btn_preview);
    DISABLE_WIDGET(btn_set_static);
    DISABLE_WIDGET(btn_varProperty);
}

void NewFunctionWindow::switchTheNewFunction(QString name, QString ret_type)
{
    if(this->newFunction == nullptr)
        this->newFunction = new NewFunction(this->belongClass, ret_type, name);

    this->newFunction->setFunctionName(name);
    this->newFunction->setFunctionRetType(ret_type);
}

void NewFunctionWindow::switchForTheNewType(QString newType)
{
    if(this->newFunction == nullptr)
        return;
    ret_type = newType;
}


NewFunctionWindow::NewFunctionWindow(QString belongClass, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewFunctionWindow)
{
    ui->setupUi(this);
    this->belongClass = belongClass;
    this->ret_type = DEF_RET_TYPE;
}


NewFunctionWindow::~NewFunctionWindow()
{
    delete ui;
}

void NewFunctionWindow::receive_setMember(NewMember* member)
{
    qDebug() << "Sucessfully get the param member";
    this->newFunction->enArg(analazyFromMemberToArg(member));
    if(this->paramWindow != nullptr)
    {
        disconnect(this->paramWindow, &NewMemberWindow::success_construct_param,
                   this, &NewFunctionWindow::receive_setMember);
        delete paramWindow;
        paramWindow = nullptr;
    }
    doPreview();
}

void NewFunctionWindow::collectInfo()
{
    // ret type
    newFunction->setFunctionRetType(ret_type);

    // comments
    newFunction->setFunctionComment(funcSelfComments);

    // type
}

void NewFunctionWindow::closeEvent(QCloseEvent* env)
{
    if(!isUserDecideHisDecision(this, "确定疑问","保留你的构建", "确定退出？所有记录都会消失", is_construct))
    {
        env->ignore(); // 不取消
        return;
    }

    if(is_construct)
    {
        emit this->signal_success_build_Function(this->newFunction);
    }
    else
    {
        qDebug() << "is_construct == false, 抛弃构建";
    }
}

void NewFunctionWindow::on_func_name_lineEdit_returnPressed()
{
    QString res = lowerFirst(ui->func_name_lineEdit->text());
    qDebug() << "Member name get:> " << res;
    REJECT_EMPTY_TEXT(res, "不可以为空", "哥们空的，搞不了的");
    switchTheNewFunction(res, ret_type);
    enableAll();
}

void NewFunctionWindow::doPreview()
{
    collectInfo();

    ui->textBrowser->setText(
        newFunction->getGeneratePreview()
    );
}


void NewFunctionWindow::on_btn_preview_clicked()
{
    doPreview();
}

void NewFunctionWindow::on_btn_addExplicit_clicked()
{
    isExplicit = !isExplicit;
    this->newFunction->setFuncState(isStatic, isExplicit, isVirtual, isConst);
    SET_BIN_STATE_BTN_CAPTION(btn_addExplicit, isExplicit, ADD_EXPLICIT_TEXT, ERASE_EXPLICIT_TEXT);
}

void NewFunctionWindow::on_btn_addVirtual_clicked()
{
    isVirtual = !isVirtual;
    this->newFunction->setFuncState(isStatic, isExplicit, isVirtual, isConst);
    SET_BIN_STATE_BTN_CAPTION(btn_addVirtual, isVirtual, ADD_VIRTUAL_TEXT, ERASE_VIRTUAL_TEXT);
    doPreview();
}

void NewFunctionWindow::on_btn_set_static_clicked()
{
    isStatic = !isStatic;
    this->newFunction->setFuncState(isStatic, isExplicit, isVirtual, isConst);
    SET_BIN_STATE_BTN_CAPTION(btn_set_static, isStatic, ADD_STATIC_TEXT, ERASE_STATIC_TEXT);
    doPreview();
}

void NewFunctionWindow::on_btn_add_const_clicked()
{
    isConst = !isConst;
    this->newFunction->setFuncState(isStatic, isExplicit, isVirtual, isConst);
    SET_BIN_STATE_BTN_CAPTION(btn_add_const, isConst, ADD_CONST_TEXT, ERASE_CONST_TEXT);
    doPreview();
}

void NewFunctionWindow::on_buttonBox_accepted()
{
    CONSIDER_RECEIVE;

    this->close();
}


void NewFunctionWindow::on_buttonBox_rejected()
{
    CONSIDER_REJECT;

    this->close();
}


void NewFunctionWindow::on_btn_varProperty_clicked()
{
    VALID_APPEND_CLICK;
    QString btn_caption;

    switch (clickTimes) {
    case TO_INT(FOR_PRIVATE):
        btn_caption = PRIVATE_STR;
        break;
    case TO_INT(FOR_PUBLIC):
        btn_caption = PUBLIC_STR;
        break;
    case TO_INT(FOR_PROTECTED):
        btn_caption = PROTECTED_STR;
        break;
    default:
        qDebug() << "FALLS ON UNKNOWN CLICK_TIME - CAPTION_STR! check it out!";
        qDebug() << "clickTimes" << clickTimes << " - "<< TO_INT(TYPE_INDEX_MAX);
        break;
    }

    SET_PROPERTY_BTN_CAPTION;
    this->newFunction->setFunctionProperty(REFLECT_PROPERTY_TYPE);
    doPreview();
}

bool NewFunctionWindow::initConnectionWithparamWindow()
{
    if(this->paramWindow == nullptr)
        return false;

    connect(this->paramWindow, &NewMemberWindow::success_construct_param,
            this, &NewFunctionWindow::receive_setMember);

    return true;
}


void NewFunctionWindow::on_btn_addParam_clicked()
{
    if(this->paramWindow == nullptr)
        this->paramWindow = new NewMemberWindow(this->belongClass, this);

    if(!initConnectionWithparamWindow())
    {
        CCSTDC_STDGUI_Only_Assert(false, "尚未构建", "newMemberWindow构建失败！");
        CCSTDC_Only_Assert(false);
        return;
    }

    this->paramWindow->setIsClassMember(false);
    this->paramWindow->show();
    doPreview();
}

void NewFunctionWindow::on_func_ret_combo_box_currentIndexChanged(int index[[maybe_unused]])
{
    ui->func_ret_type_line_edit->setText(ui->func_ret_combo_box->currentText());
    this->ret_type = ui->func_ret_combo_box->currentText();
    doPreview();
}

void NewFunctionWindow::on_func_ret_type_line_edit_textEdited(const QString &arg1)
{
    if(arg1.isEmpty())
        return;
    int currentCount = ui->func_ret_combo_box->count();
    int matchStringIndex = -1;
    for(int i = 0; i < currentCount; i++)
    {
        if(arg1 == ui->func_ret_combo_box->itemText(i))
            matchStringIndex = i;
    }

    if(matchStringIndex == -1)
        ui->func_ret_combo_box->setCurrentText("自定义");
    else
        ui->func_ret_combo_box->setCurrentIndex(matchStringIndex);

    ui->func_ret_type_line_edit->setText(arg1);

    switchForTheNewType(arg1);

    doPreview();
}

void NewFunctionWindow::on_func_name_lineEdit_textEdited(const QString &arg1[[maybe_unused]])
{
    QString res = lowerFirst(ui->func_name_lineEdit->text());
    qDebug() << "Member name get:> " << res;
    if(res.isEmpty())
    {
        disableAll();
        return;
    }

    switchTheNewFunction(res, ret_type);
    enableAll();
    doPreview();
}

void NewFunctionWindow::on_comment_for_func_textChanged()
{
    if(this->newFunction == nullptr)
        return;
    funcSelfComments = ui->comment_for_func->toPlainText();
    doPreview();
}




