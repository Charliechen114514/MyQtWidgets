#include "editclasswindow.h"
#include "ui_editclasswindow.h"
#include <editfunctionwindow.h>
#include <editmemberwindow.h>
#include "utils.h"
#define DENY_UNACCESSIBLE if(editClass == nullptr) return;
#define SET_ORG_CLASSNAME ui->lineEdit->setText(editClass->getName())

EditClassWindow::EditClassWindow(NewClass* _class, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditClassWindow)
{
    ui->setupUi(this);

    editClass = _class;
    oldName = editClass->getName();
    doPreview();
    SET_ORG_CLASSNAME;
}

EditClassWindow::~EditClassWindow()
{
    delete ui;
}

void EditClassWindow::doPreview()
{
    DENY_UNACCESSIBLE;
    ui->textBrowser->setText(editClass->generateHeader());
}

void EditClassWindow::receiveEditClassesFromMemberChange()
{
    qDebug() << "receive member edited";
    doPreview();
    if(this->editMemberWindow != nullptr)
        delete this->editMemberWindow;
    this->editMemberWindow = nullptr;
}

void EditClassWindow::receiveEditClassedFromFunctionChange()
{
    qDebug() << "receive function edited";
    doPreview();
    if(this->editFunctionWindow != nullptr)
        delete this->editFunctionWindow;
    this->editFunctionWindow = nullptr;
}


void EditClassWindow::closeEvent(QCloseEvent*)
{
    this->finalName = editClass->getName();
    emit successEdit();
}


void EditClassWindow::on_lineEdit_textEdited(const QString &arg1)
{
    isChange = true;
    if(arg1.isEmpty())
        return;
    if(arg1 != oldName)
    {
        this->isClassNameEdit = true;
    }
    else
    {
        this->isClassNameEdit = false;
    }
    DENY_UNACCESSIBLE;
    changeName(arg1);
    doPreview();
}


void EditClassWindow::on_btn_adjustMember_clicked()
{
    isChange = true;
    if(this->editMemberWindow == nullptr)
        this->editMemberWindow = new EditMemberWindow(this->editClass, this);

    connect(this->editMemberWindow, &EditMemberWindow::emitFinishWork,
            this, &EditClassWindow::receiveEditClassesFromMemberChange);

    this->editMemberWindow->show();
}


void EditClassWindow::on_btn_adjsutFunction_clicked()
{
    isChange = true;
    if(this->editFunctionWindow == nullptr)
        this->editFunctionWindow = new EditFunctionWindow(this->editClass, this);

    connect(this->editFunctionWindow, &EditFunctionWindow::emitFinishWork,
            this, &EditClassWindow::receiveEditClassedFromFunctionChange);

    this->editFunctionWindow->show();
}


void EditClassWindow::on_buttonBox_accepted()
{
    if(!isUserDecideHisDecision(this, "确认","是否保存更改","确定不做出更改?",isChange))
    {
        return;
    }
    this->close();
}


void EditClassWindow::on_buttonBox_rejected()
{
    if(!isUserDecideHisDecision(this, "确认","是否保存更改","确定不做出更改?",isChange))
    {
        return;
    }
    this->close();
}

