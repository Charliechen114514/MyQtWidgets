#include "newclasswindow.h"
#include "ui_newclasswindow.h"
#include "utils.h"

NewClassWindow::NewClassWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewClassWindow)
{
    ui->setupUi(this);

}


void NewClassWindow::initStyles()
{
}


void NewClassWindow::switchTheNewClass(QString newName)
{
    // olds
//    if(this->constructing_newClass != nullptr)
//        delete this->constructing_newClass;
//    this->constructing_newClass = new NewClass(newName);
    if(this->constructing_newClass == nullptr)
        this->constructing_newClass = new NewClass(newName);
    else
        this->constructing_newClass->setClassName(newName);
}

void NewClassWindow::receive_setMember(NewMember* member)
{
    qDebug() << "Success receive the success_install_signal: set_up_member";
    this->constructing_newClass->addMember(member);
    ui->textBrowser->setText(this->constructing_newClass->generateHeader());
    if(this->memberWindow != nullptr)
    {
        disconnect(this->memberWindow, &NewMemberWindow::success_construct_member, \
                   this, &NewClassWindow::receive_setMember);
        delete this->memberWindow;
        this->memberWindow = nullptr;
    }
}

void NewClassWindow::receive_setFunction(NewFunction* func)
{
    qDebug() << "Success receive the success_install_signal: set_up_function";
    this->constructing_newClass->addFunction(func);
    ui->textBrowser->setText(this->constructing_newClass->generateHeader());
    if(this->functionWindow != nullptr)
    {
        disconnect(this->functionWindow, &NewFunctionWindow::signal_success_build_Function,
                   this, &NewClassWindow::receive_setFunction);
        delete this->functionWindow;
        this->functionWindow = nullptr;
    }
}

bool NewClassWindow::initConnectionWithNewMemberWindow()
{
    if(this->memberWindow == nullptr)
        return false;

    connect(this->memberWindow, &NewMemberWindow::success_construct_member, \
            this, &NewClassWindow::receive_setMember);

    return true;
}

bool NewClassWindow::initConnectionWithNewFunctionWindow()
{
    if(this->functionWindow == nullptr)
        return false;

    connect(this->functionWindow, &NewFunctionWindow::signal_success_build_Function,
            this, &NewClassWindow::receive_setFunction);

    return true;
}

void NewClassWindow::closeEvent(QCloseEvent* env)
{
    if(!isUserDecideHisDecision(this, "确定疑问","保留你的构建", "确定退出？所有记录都会消失", is_construct))
    {
        env->ignore(); // 不取消
        return;
    }
    if(is_construct)
    {
        emit this->success_setClass(this->constructing_newClass);
    }
    else
    {
        qDebug() << "is_construct == false, 抛弃构建";
    }
}

NewClassWindow::~NewClassWindow()
{
    delete ui;
}

void NewClassWindow::on_buttonBox_accepted()
{
    // 用户认为它需要此次构建
    if(this->constructing_newClass == nullptr)
    {
        on_buttonBox_rejected();
        return;
    }

    CONSIDER_RECEIVE;
    // 关闭之
    this->close();
}


void NewClassWindow::on_buttonBox_rejected()
{
    CONSIDER_REJECT;
    // 关闭之
    this->close();
}


void NewClassWindow::on_className_lineEdit_returnPressed()
{
    QString receive = upperFirst(this->ui->className_lineEdit->text());
    qDebug()<< "Getting the receiving name:> " + receive;
    REJECT_EMPTY_TEXT(receive, "不接受输入","哥们空的, 创建不了！");
    switchTheNewClass(receive);
    enableSettings();
}

void NewClassWindow::enableSettings()
{
    ENABLE_WIDGET(btn_preview);
    ENABLE_WIDGET(btn_add_function);
    ENABLE_WIDGET(btn_add_member);
    ENABLE_WIDGET(comboBox);
}

void NewClassWindow::disableSettings()
{
    DISABLE_WIDGET(btn_preview);
    DISABLE_WIDGET(btn_add_function);
    DISABLE_WIDGET(btn_add_member);
    DISABLE_WIDGET(comboBox);
}


void NewClassWindow::on_comboBox_activated(int index)
{
    qDebug() << "Construct def select index:> " << index << ": " << this->ui->comboBox->itemText(index);
    CONSTRUCTOR_INDEX choice = static_cast<CONSTRUCTOR_INDEX>(index);
    switch (choice) {
    case CONSTRUCTOR_INDEX::DISABLE_ALL_DEF_CONSTRUCT:
        this->constructing_newClass->setStates(false, false, false);
        break;
    case CONSTRUCTOR_INDEX::DISABLE_DEF_ONLY:
        this->constructing_newClass->setStates(false, true,true);
        break;
    case CONSTRUCTOR_INDEX::DISABLE_COPY_ONLY:
        this->constructing_newClass->setStates(true, false, false);
        break;
    case CONSTRUCTOR_INDEX::ALLOW_ALL:
        this->constructing_newClass->setStates(true, true, true);
        break;
    default:
        this->constructing_newClass->setStates(false, false, false);
        break;
    }
    doPreview();
}

void NewClassWindow::collectInfos()
{
    // ClassName already sets

    // Comments
    this->constructing_newClass->setComment(this->ui->comment_edit->toPlainText());

    // finish
}

void NewClassWindow::doPreview()
{
    // get relevent infos
    collectInfos();
    ui->textBrowser->setText(this->constructing_newClass->generateHeader());
}


void NewClassWindow::on_btn_preview_clicked()
{
    doPreview();
}


void NewClassWindow::on_btn_add_member_clicked()
{
    if(this->memberWindow == nullptr)
        this->memberWindow = new NewMemberWindow(this->constructing_newClass->getName(), this);

    if(!initConnectionWithNewMemberWindow())
    {
        CCSTDC_STDGUI_Only_Assert(false, "尚未构建", "newMemberWindow构建失败！");
        CCSTDC_Only_Assert(false);
        return;
    }

    this->memberWindow->show();
}


void NewClassWindow::on_btn_add_function_clicked()
{
    if(this->functionWindow == nullptr)
        this->functionWindow = new NewFunctionWindow(this->constructing_newClass->getName(), this);

    if(!initConnectionWithNewFunctionWindow())
    {
        CCSTDC_STDGUI_Only_Assert(false, "尚未构建", "newFunctionWindow构建失败！");
        CCSTDC_Only_Assert(false);
        return;
    }

    this->functionWindow->show();
}


void NewClassWindow::on_className_lineEdit_textEdited(const QString &arg1)
{
    qDebug() << "receive LineEdit signal";
    if(arg1.isEmpty())
    {
        disableSettings();
        return;
    }
    else
    {
        QString res = upperFirst(arg1);
        qDebug()<< "Getting the receiving name:> " + res;
        REJECT_EMPTY_TEXT(res, "不接受输入","哥们空的, 创建不了！");
        switchTheNewClass(res);
        enableSettings();
        doPreview();
    }
}


void NewClassWindow::on_comment_edit_textChanged()
{
    doPreview();
}

