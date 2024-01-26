#include "editmemberwindow.h"
#include "newmemberwindow.h"
#include "ui_editmemberwindow.h"
#include "newclass.h"
#include "newmember.h"
#include "mainwindowclassesmanager.h"
#include <QMessageBox>
#define DENY_UNACCESSIBLE if(bindingClass == nullptr) return
EditMemberWindow::EditMemberWindow(NewClass* _class_, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditMemberWindow)
{
    ui->setupUi(this);
    bindingClass = _class_;
    initManager();
}

void EditMemberWindow::initManager()
{
    DENY_UNACCESSIBLE;
    QList<NewMember*> static_member = bindingClass->getClassMember(NewClass::MEMBER_TYPE::M_STATIC);
    QList<NewMember*> public_member = bindingClass->getClassMember(NewClass::MEMBER_TYPE::PUBLIC);
    QList<NewMember*> private_member = bindingClass->getClassMember(NewClass::MEMBER_TYPE::PRIVATE);
    for(NewMember*& each : static_member)
        manager.createWidgetItem(this->ui->listsOfMember, each->getName());

    for(NewMember*& each : public_member)
        manager.createWidgetItem(this->ui->listsOfMember, each->getName());

    for(NewMember*& each : private_member)
        manager.createWidgetItem(this->ui->listsOfMember, each->getName());
}

NewMember* EditMemberWindow::getCurrentFocusMember()
{
    NewMember* find = MainWindowClassesManager::match<NewMember>
        (this->bindingClass->getClassMember(NewClass::MEMBER_TYPE::PUBLIC), manager.getCurrentItemText());
    if(!find)
    {
        find = MainWindowClassesManager::match<NewMember>
            (this->bindingClass->getClassMember(NewClass::MEMBER_TYPE::PRIVATE), manager.getCurrentItemText());
    }

    return find;
}

void EditMemberWindow::doPreview(){
    NewMember* m = getCurrentFocusMember();
    if(m == nullptr)
        return;

    ui->textBrowser->setText(m->getGeneratePreview());
}

bool EditMemberWindow::eraseFocusMember()
{
    bool s = this->bindingClass->dismissMember(this->getCurrentFocusMember());
    manager.eraseItem(this->manager.getCurrentItemFocus());
    return s;
}

EditMemberWindow::~EditMemberWindow()
{
    if(this->newMemberWindow != nullptr)
        delete this->newMemberWindow;
    this->newMemberWindow = nullptr;
    delete ui;
}

void EditMemberWindow::closeEvent(QCloseEvent*)
{
    emit this->emitFinishWork();
}


void EditMemberWindow::successReceiveNewMember(NewMember* m)
{
    qDebug() << "receive new member";
    if(isEdit)
    {
        isEdit = false;
        isOnEdit->copyConfig(m);
        manager.getCurrentItemFocus()->setText(isOnEdit->getName());
        delete m;
        m = nullptr;
        bindingClass->adjustMembers();
        isOnEdit = nullptr;
    }
    else
    {
        this->bindingClass->addMember(m);
        manager.createWidgetItem(this->ui->listsOfMember, m->getName());
    }
    if(this->newMemberWindow != nullptr)
        delete this->newMemberWindow;
    this->newMemberWindow = nullptr;
    doPreview();
}


void EditMemberWindow::switchTargetMember(QListWidgetItem* item)
{
    QString text = manager.getItemText(item);
    NewMember* find = MainWindowClassesManager::match<NewMember>
        (this->bindingClass->getClassMember(NewClass::MEMBER_TYPE::PUBLIC), text);
    if(!find)
    {
        find = MainWindowClassesManager::match<NewMember>
            (this->bindingClass->getClassMember(NewClass::MEMBER_TYPE::PRIVATE), text);
    }

    qDebug() << find->getName();

    if(find == nullptr)
        return;

    ui->textBrowser->setText(find->getGeneratePreview());

    manager.setCurrentItemFocus(item);
    doPreview();
}

void EditMemberWindow::showAddMember()
{
    if(this->newMemberWindow == nullptr)
        this->newMemberWindow = new NewMemberWindow(bindingClass->getName(), this);

    connect(this->newMemberWindow, &NewMemberWindow::success_construct_member,
            this, &EditMemberWindow::successReceiveNewMember);

    this->newMemberWindow->show();
}

void EditMemberWindow::showEditMember()
{
    isOnEdit = getCurrentFocusMember();
    if(isOnEdit == nullptr)
    {
        QMessageBox::critical(this, "选中成员！","没有选中成员");
        return;
    }
    isEdit = true;
    if(this->newMemberWindow == nullptr)
        this->newMemberWindow = new NewMemberWindow(bindingClass->getName(), this);

    connect(this->newMemberWindow, &NewMemberWindow::success_construct_member,
            this, &EditMemberWindow::successReceiveNewMember);

    this->newMemberWindow->setWindowTitle("你正在编辑: " + isOnEdit->getName());
    this->newMemberWindow->show();
}

void EditMemberWindow::on_listsOfMember_itemClicked(QListWidgetItem *item)
{
    switchTargetMember(item);
}


void EditMemberWindow::on_btn_addMember_clicked()
{
    showAddMember();
}


void EditMemberWindow::on_btn_editMember_clicked()
{
    showEditMember();
}

