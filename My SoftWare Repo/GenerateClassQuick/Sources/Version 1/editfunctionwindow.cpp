#include "editfunctionwindow.h"
#include "ui_editfunctionwindow.h"
#include "newclass.h"
#include "newfunction.h"
#include "newfunctionwindow.h"
#include "mainwindowclassesmanager.h"
#include <QMessageBox>
#define DENY_UNACCESSIBLE if(bindingClass == nullptr) return


EditFunctionWindow::EditFunctionWindow(NewClass* class_,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditFunctionWindow)
{
    ui->setupUi(this);
    bindingClass = class_;
    initManager();
}

EditFunctionWindow::~EditFunctionWindow()
{
    delete ui;
}

void EditFunctionWindow::initManager()
{
    DENY_UNACCESSIBLE;

    QList<NewFunction*> public_lists = bindingClass->getClassFunc(NewClass::FUNC_TYPE::PUBLIC);
    QList<NewFunction*> protected_lists = bindingClass->getClassFunc(NewClass::FUNC_TYPE::PROTECTED);
    QList<NewFunction*> private_lists = bindingClass->getClassFunc(NewClass::FUNC_TYPE::PRIVATE);

    for(NewFunction*& each : public_lists)
        manager.createWidgetItem(this->ui->listsOfMember, each->getName());

    for(NewFunction*& each : protected_lists)
        manager.createWidgetItem(this->ui->listsOfMember, each->getName());

    for(NewFunction*& each : private_lists)
        manager.createWidgetItem(this->ui->listsOfMember, each->getName());
}

NewFunction* EditFunctionWindow::getCurrentFocusFunction()
{
    NewFunction* find = MainWindowClassesManager::match<NewFunction>
        (this->bindingClass->getClassFunc(NewClass::FUNC_TYPE::PUBLIC), manager.getCurrentItemText());
    if(!find)
    {
        find = MainWindowClassesManager::match<NewFunction>
            (this->bindingClass->getClassFunc(NewClass::FUNC_TYPE::PRIVATE), manager.getCurrentItemText());
        if(!find)
            find = MainWindowClassesManager::match<NewFunction>
                (this->bindingClass->getClassFunc(NewClass::FUNC_TYPE::PROTECTED), manager.getCurrentItemText());
    }

    return find;
}

void EditFunctionWindow::switchToCurrentFunction(QListWidgetItem* item)
{
    QString text = manager.getItemText(item);
    NewFunction* find = MainWindowClassesManager::match<NewFunction>
        (this->bindingClass->getClassFunc(NewClass::FUNC_TYPE::PUBLIC), text);
    if(!find)
    {
        find = MainWindowClassesManager::match<NewFunction>
            (this->bindingClass->getClassFunc(NewClass::FUNC_TYPE::PROTECTED), text);
        if(!find)
            find = MainWindowClassesManager::match<NewFunction>
                (this->bindingClass->getClassFunc(NewClass::FUNC_TYPE::PRIVATE), text);
    }

    qDebug() << find->getName();

    if(find == nullptr)
        return;

    ui->textBrowser->setText(find->getGeneratePreview());

    manager.setCurrentItemFocus(item);
    doPreview();
}

void EditFunctionWindow::doPreview()
{
    NewFunction* cur = getCurrentFocusFunction();
    if(cur == nullptr)
        return;
    ui->textBrowser->setText(cur->getGeneratePreview());
}

void EditFunctionWindow::showAddFunction()
{
    if(this->newFunctionWindow == nullptr)
        this->newFunctionWindow = new NewFunctionWindow(bindingClass->getName(), this);

    connect(this->newFunctionWindow, &NewFunctionWindow::signal_success_build_Function,
            this, &EditFunctionWindow::successReceiveNewFunction);

    this->newFunctionWindow->show();
}

void EditFunctionWindow::showEditFunction()
{
    NewFunction* cur = getCurrentFocusFunction();
    if(!cur)
    {
        QMessageBox::critical(this, "没选中","没有选中指定的函数");
        return;
    }
    isEdit = true;
    isOnEdit = cur;
    if(this->newFunctionWindow == nullptr)
        this->newFunctionWindow = new NewFunctionWindow(bindingClass->getName(), this);

    connect(this->newFunctionWindow, &NewFunctionWindow::signal_success_build_Function,
            this, &EditFunctionWindow::successReceiveNewFunction);

    this->newFunctionWindow->setWindowTitle("你正在编辑: " + isOnEdit->getName());

    this->newFunctionWindow->show();

}

void EditFunctionWindow::successReceiveNewFunction(NewFunction* f)
{
    qDebug() << "receive new member";
    if(isEdit)
    {
        isEdit = false;
        isOnEdit->copyConfig(f);
        manager.getCurrentItemFocus()->setText(isOnEdit->getName());
        delete f;
        f = nullptr;
        bindingClass->adjustFunctions();
        isOnEdit = nullptr;
    }
    else
    {
        this->bindingClass->addFunction(f);
        manager.createWidgetItem(this->ui->listsOfMember, f->getName());
    }
    if(this->newFunctionWindow != nullptr)
        delete this->newFunctionWindow;
    this->newFunctionWindow = nullptr;
    doPreview();
}

void EditFunctionWindow::on_btn_newMethod_clicked()
{
    showAddFunction();
}


void EditFunctionWindow::on_listsOfMember_itemClicked(QListWidgetItem *item)
{
    switchToCurrentFunction(item);
}

void EditFunctionWindow::closeEvent(QCloseEvent*)
{
    emit this->emitFinishWork();
}

void EditFunctionWindow::on_btn_editFunction_clicked()
{
    showEditFunction();
}

