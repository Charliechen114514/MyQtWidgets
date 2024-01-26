#include "mainwindow.h"
#include "newclass.h"
#include "newclasswindow.h"
#include "newfunctionwindow.h"
#include "newmemberwindow.h"
#include "ui_mainwindow.h"
#include "classlistandwidgetitemmanager.h"
#include "editclasswindow.h"
#include "utils.h"
#include "qssmanager.h"
#include <QFile>
#include "mainwindowclassesmanager.h"
#include "menuactionconnector.h"
#include "savingconfigwindow.h"
#include "savepathgenerator.h"
#define DEF_QSS_PATH ":/qdarkstyle/light/lightstyle.qss"
#define LIGHT_QSS_PATH  ":/qdarkstyle/light/lightstyle.qss"
#define DARK_QSS_PATH   ":/qdarkstyle/dark/darkstyle.qss"

#define DENY_UNCONFIG_EXPORT \
if(!isFinishConfig)\
{\
    QMessageBox::critical(this,"没有配置","请先配置好路径！");\
    return;\
}

#define CLEAR_LIST(list)    list.clear()
#define CLEAR_WINDOW(pWindow) if(pWindow != nullptr){delete pWindow; pWindow = nullptr;}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    loadDefaultStyleSheet();
    initClassesListWidget();
    initMenuConnections();
}

MainWindow::~MainWindow()
{
    CLEAR_WINDOW(this->editClassWindow);
    CLEAR_WINDOW(this->newMemberWindow);
    CLEAR_WINDOW(this->newFunctionWindow);
    for(NewClass* each : this->Classes)
        if(each)
        {
            delete each;
            each = nullptr;
        }

    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* env)
{
    qDebug() << "Catch Key Press";
    if(env->key() == Qt::Key_Right)
    {
        moveToNext();
    }
    else if (env->key() == Qt::Key_Left)
    {
        moveToPrevious();
    }
}


// null
void MainWindow::initClassesListWidget()
{}

void MainWindow::loadDefaultStyleSheet()
{
    QString src = QSSManager::getQSS(DEF_QSS_PATH);

    if(src.isEmpty())
        return;

    this->setStyleSheet(src);
    ui->textBrowser->setFont(QFont("Consola"));
}

void MainWindow::initMenuConnections()
{
    QList<QAction*> acts;
    QList<MenuActionConnector<MainWindow>::VVF> vvf;
    // make inits of view
    acts << ui->action_lightView << ui->action_darkView;
    vvf << &MainWindow::toLightView << &MainWindow::toDarkView;
    StyleSwitchMenuActionConnector<MainWindow>::connectAll(this, acts, vvf);
    CLEAR_LIST(acts);CLEAR_LIST(vvf);
    // creation
    acts << ui->action_setClass << ui->action_addMember << ui->action_addFunction;
    vvf << &MainWindow::buildClass << &MainWindow::buildMemberToClass << &MainWindow::buildFuncToClass;
    CreateMenuActionConnector<MainWindow>::connectAll(this, acts, vvf);
    CLEAR_LIST(acts);CLEAR_LIST(vvf);
    // view
    acts << ui->action_showCurrentHeader << ui->action_showCurrentSource
         << ui->action_toNext << ui->action_toPrevious;
    vvf  << &MainWindow::showCurrentClassHeader << &MainWindow::showCurrentClassSource
         << &MainWindow::moveToNext << &MainWindow::moveToPrevious;
    ViewMenuActionConnector<MainWindow>::connectAll(this, acts, vvf);
    CLEAR_LIST(acts);CLEAR_LIST(vvf);

    // config
    acts << ui->action_editConfigWindow << ui->action_viewExportSetting;
    vvf << &MainWindow::showConfigWindow << &MainWindow::previewSave;
    ExportSettingMenuActionConnector<MainWindow>::connectAll(this, acts, vvf);
    CLEAR_LIST(acts);CLEAR_LIST(vvf);

    // export
    acts << ui->action_export_Normal << ui->action_export_toCMake;
    vvf <<  &MainWindow::exportNormal << &MainWindow::exportToCmake;
    ExportBehaviourMenuActionConnector<MainWindow>::connectAll(this, acts, vvf);
    CLEAR_LIST(acts);CLEAR_LIST(vvf);
}


void MainWindow::switchQSS(QString path)
{
    QString src = QSSManager::getQSS(path);

    if(src.isEmpty())
        return;

    this->setStyleSheet(src);
    ui->textBrowser->setFont(QFont("Consola"));
}

void MainWindow::toLightView()
{
    qDebug() << "换到亮主题";
    switchQSS(LIGHT_QSS_PATH);
}

void MainWindow::toDarkView()
{
    qDebug() << "换到暗主题";
    switchQSS(DARK_QSS_PATH);
}

void MainWindow::moveToNext()
{
    if(this->Classes.empty())
        return;

    NewClass* cur = getCurrentNewClass();
    if(this->Classes.last() == cur)
    {
        // 最后一个了，需要重置到第一个
        makePreview(this->Classes.first());
        managerGlobal.setCurrentItemFocus(managerGlobal.matchByName(this->Classes.first()->getName()));
        return;
    }
    else
    {
        int idx = this->Classes.indexOf(cur);
        NewClass* next = this->Classes.at(idx + 1);
        makePreview(next);
        managerGlobal.setCurrentItemFocus(managerGlobal.matchByName(next->getName()));
    }
}

void MainWindow::moveToPrevious()
{
    if(this->Classes.empty())
        return;
    NewClass* cur = getCurrentNewClass();
    if(this->Classes.first() == cur)
    {
        // 最后一个了，需要重置到第一个
        makePreview(this->Classes.last());
        managerGlobal.setCurrentItemFocus(managerGlobal.matchByName(this->Classes.last()->getName()));
        return;
    }
    else
    {
        int idx = this->Classes.indexOf(cur);
        NewClass* next = this->Classes.at(idx - 1);
        makePreview(next);
        managerGlobal.setCurrentItemFocus(managerGlobal.matchByName(next->getName()));
    }
}

void MainWindow::showCurrentClassHeader()
{
    NewClass* cur = getCurrentNewClass();
    if(cur == nullptr)
        return;

    this->ui->textBrowser->setText(cur->generateHeader());
}
void MainWindow::showCurrentClassSource()
{
    NewClass* cur = getCurrentNewClass();
    if(cur == nullptr)
        return;

    this->ui->textBrowser->setText(cur->generateSource());
}

bool MainWindow::initConnectionWithNewClassWindow()
{
    if(this->newClassWindow == nullptr)
        return false;

    connect(this->newClassWindow, &NewClassWindow::success_setClass, this, &MainWindow::successSetClass);

    return true;
}

bool MainWindow::initMemberWindow()
{
    if(this->newMemberWindow == nullptr)
        this->newMemberWindow = new NewMemberWindow(getCurrentNewClass()->getName(), this);

    connect(this->newMemberWindow, &NewMemberWindow::success_construct_member, this, &MainWindow::successSetMember);

    return true;
}

bool MainWindow::initFunctionWindow()
{
    if(this->newFunctionWindow == nullptr)
        this->newFunctionWindow = new NewFunctionWindow(getCurrentNewClass()->getName(), this);

    connect(this->newFunctionWindow, &NewFunctionWindow::signal_success_build_Function, \
            this, &MainWindow::successSetFunction);

    return true;
}


NewClass* MainWindow::getCurrentNewClass()
{
    return MainWindowClassesManager::match<NewClass>(this->Classes,managerGlobal.getCurrentItemText());
}


void MainWindow::successSetClass(NewClass* settingClass)
{
    qDebug() << "Success receive the success_install_signal: set_up_class";

    this->Classes.push_back(settingClass);
    this->ui->textBrowser->setText(this->Classes.last()->generateHeader());
    if(this->newClassWindow != nullptr)
    {
        disconnect(this->newClassWindow, &NewClassWindow::success_setClass, this, &MainWindow::successSetClass);
        delete newClassWindow;
        newClassWindow = nullptr;
    }
    qDebug() << this->Classes.size();
    qDebugTheNewClassInfo(this->Classes);

    addClassWidgetItemToList(this->Classes.last()->getName());
    qDebug() << managerGlobal.getItemCount();
}

void MainWindow::successSetMember(NewMember* member)
{
    qDebug() << "Success receive the success_install_signal: set_up_member";

    NewClass* tmp = MainWindowClassesManager::match<NewClass>(this->Classes,managerGlobal.getCurrentItemText());
    if(!tmp)
        return;

    tmp->addMember(member);

    if(this->newMemberWindow != nullptr)
    {
        disconnect(this->newMemberWindow, &NewMemberWindow::success_construct_member, this, &MainWindow::successSetMember);
        delete this->newMemberWindow;
        this->newMemberWindow = nullptr;
    }

    qDebug() << "add member success";

    makePreview(tmp);
}

void MainWindow::successSetFunction(NewFunction* func)
{
    qDebug() << "Success receive the success_install_signal: set_up_function";
    NewClass* tmp = MainWindowClassesManager::match<NewClass>(this->Classes,managerGlobal.getCurrentItemText());
    if(!tmp)
        return;

    tmp->addFunction(func);

    if(this->newFunctionWindow != nullptr)
    {
        disconnect(this->newFunctionWindow, &NewFunctionWindow::signal_success_build_Function, \
                   this, &MainWindow::successSetFunction);
        delete this->newFunctionWindow;
        this->newFunctionWindow = nullptr;
    }

    qDebug() << "add functions success";

    makePreview(tmp);
}

void MainWindow::successGetEditedClass()
{
    qDebug() << "Success receive the success_install_signal: edit Classed";
    if(this->editClassWindow->getNameEdited())
    {
        managerGlobal.getCurrentItemFocus()->setText(this->editClassWindow->getFinalName());
    }

    NewClass* tmp = MainWindowClassesManager::match<NewClass>(this->Classes,managerGlobal.getCurrentItemText());
    if(tmp == nullptr)
        return;
    makePreview(tmp);
}

void MainWindow::addClassWidgetItemToList(QString name)
{
    managerGlobal.createWidgetItem(ui->classList, name);
}

void MainWindow::makePreview(NewClass* whatClass)
{
    if(whatClass == nullptr)
        return;

    this->ui->textBrowser->setText(whatClass->generateHeader());
}

void MainWindow::eraseCurrentClass()
{
    if(this->Classes.empty())
    {
        QMessageBox::critical(this, "空的","不要尝试删除空气！");
        return;
    }

    NewClass* find = MainWindowClassesManager::match<NewClass>(this->Classes, managerGlobal.getCurrentItemText());

    qDebug() << find->getName();

    if(find == nullptr)
    {
        QMessageBox::critical(this, "没有选中","你没有选中一个目标类");
        return;
    }

    if(!isUserDecideHisDecision(this, "确定", "你确定你要删除当前你选中的类 " + find->getName() + " 嘛?",\
                                 "你确定你要删除当前你选中的类 " + find->getName() + " 嘛?", true))
        return;

    int index = this->Classes.indexOf(find);
    if(index == -1)
    {
        qDebug() << "Can not find target";
        return;
    }
    this->Classes.removeAt(index);

    managerGlobal.eraseItem(managerGlobal.getCurrentItemFocus());
    managerGlobal.debugText();
    // toNew
    if(!this->Classes.empty())
    {
        makePreview(this->Classes.first());
        managerGlobal.setCurrentItemFocus(managerGlobal.matchByName(this->Classes.first()->getName()));
    }
    else
    {
        this->ui->textBrowser->setText("");
    }
}

void MainWindow::buildClass()
{
    // if not inits, do create it
    if(this->newClassWindow == nullptr)
        this->newClassWindow = new NewClassWindow(this);

    if(!initConnectionWithNewClassWindow())
    {
        CCSTDC_STDGUI_Only_Assert(false, "尚未构建", "newClassWindow构建失败！");
        CCSTDC_Only_Assert(false);
        return;
    }

    // make it appear
    this->newClassWindow->show();
}
void MainWindow::buildMemberToClass()
{
    if(!managerGlobal.isFocusAnyItem())
    {
        QMessageBox::critical(this, "你尚未选择类对象","选择一个类再做这件事情！");
        return;
    }

    initMemberWindow();

    if(this->newMemberWindow != nullptr)
        this->newMemberWindow->show();
}


void MainWindow::buildFuncToClass()
{
    if(!managerGlobal.isFocusAnyItem())
    {
        QMessageBox::critical(this, "你尚未选择类对象","选择一个类再做这件事情！");
        return;
    }

    initFunctionWindow();

    if(this->newFunctionWindow != nullptr)
        this->newFunctionWindow->show();
}

void MainWindow::switchTargetClass(QListWidgetItem* item)
{
    QString text = managerGlobal.getItemText(item);

    NewClass* find = MainWindowClassesManager::match<NewClass>(this->Classes, text);

    qDebug() << find->getName();

    if(find == nullptr)
        return;

    ui->textBrowser->setText(find->generateHeader());
    managerGlobal.setCurrentItemFocus(item);
}

void MainWindow::beginExport()
{
    if(this->manager == nullptr)
        this->manager = new ExportManager;
}

void MainWindow::endExport()
{
    if(this->manager != nullptr)
    {
        delete this->manager;
        this->manager = nullptr;
    }
}

void MainWindow::makeExport(ExportBuilderMethod& method)
{
    beginExport();
    if(this->manager == nullptr)
        return;

    manager->exportToTarget(this->Classes, method);

    endExport();

    QMessageBox::information(this,"导出完成！","导出完成！");
}


void MainWindow::showEditClassWindow()
{
    NewClass* current = this->getCurrentNewClass();
    if(current == nullptr)
    {
        QMessageBox::critical(this, "没选中类","请选中类之后在编辑");
        return;
    }
    if(this->editClassWindow == nullptr)
        this->editClassWindow = new EditClassWindow(current, this);

    connect(this->editClassWindow, &EditClassWindow::successEdit, this, &MainWindow::successGetEditedClass);
    this->editClassWindow->show();
}

void MainWindow::processSetClassExportPath()
{
    switch(this->configWindow->getSaveOption())
    {
    case SavingConfigWindow::SAVE_OPTION::ALL:
        SavePathGenerator::generatorPathForLists_ALL_IN_ONE(this->Classes, configWindow->getConfigPathStringAll());
        this->isFinishConfig = true;
        break;
    case SavingConfigWindow::SAVE_OPTION::SRIN:
        SavePathGenerator::generatorPathForLists_PAIR_SRIN(this->Classes, configWindow->getHeaderPath(), configWindow->getSourcePath());
        this->isFinishConfig = true;
        break;
    case SavingConfigWindow::SAVE_OPTION::CANCLE:
        QMessageBox::critical(this,"取消了配置！","取消了配置！");
        break;
    case SavingConfigWindow::SAVE_OPTION::PREVIEW:
        break;
    default:
        QMessageBox::critical(this,"保存配置出错！","保存配置出错！");
        break;
    }
}

void MainWindow::showConfigWindow()
{
    if(this->configWindow == nullptr)
        this->configWindow = new SavingConfigWindow(this);

    connect(this->configWindow, &SavingConfigWindow::finishEditSavingConfig,
            this, &MainWindow::processSetClassExportPath);
    this->configWindow->moveToDefault();
    this->configWindow->show();
}

void MainWindow::previewSave()
{
    if(this->configWindow == nullptr)
    {
        QMessageBox::critical(this,"出错","请先编辑配置！");
        return;
    }

    QMap<QString, QStringList>* config = SavePathGenerator::generatePairlyPreview(this->Classes);
    this->configWindow->makePreview(config);
    this->configWindow->disAbleAll();
    this->configWindow->setAsPreView();
    this->configWindow->show();
}

void MainWindow::exportToCmake()
{
    DENY_UNCONFIG_EXPORT;
    QString CmakePath;
    if(!getPath(this, "选择CmakeLists", CmakePath))
        return;
    CMakeMethod method(CmakePath);
    makeExport(method);
}

void MainWindow::exportNormal()
{
    DENY_UNCONFIG_EXPORT;
    NormalMethod m;
    makeExport(m);
}


/// slots


void MainWindow::on_btn_setClassFirst_clicked()
{
    buildClass();
}


void MainWindow::on_btn_showHeader_clicked()
{
    if(this->Classes.isEmpty())
        return;

    this->ui->textBrowser->setText(this->Classes.last()->generateHeader());
}


void MainWindow::on_btn_showSource_clicked()
{
    if(this->Classes.isEmpty())
        return;
    this->ui->textBrowser->setText(this->Classes.last()->generateSource());
}


void MainWindow::on_classList_itemClicked(QListWidgetItem *item)
{
    switchTargetClass(item);
}


void MainWindow::on_btn_add_member_clicked()
{
    buildMemberToClass();
}


void MainWindow::on_btn_add_function_clicked()
{
    buildFuncToClass();
}


void MainWindow::on_btn_clear_current_clicked()
{
    this->eraseCurrentClass();
}


void MainWindow::on_btn_next_clicked()
{
    moveToNext();
}

void MainWindow::on_btn_pre_clicked()
{
    moveToPrevious();
}


void MainWindow::on_btn_composeCurrentClass_clicked()
{
    showEditClassWindow();
}


void MainWindow::on_btn_refresh_clicked()
{
    NewClass* cur = getCurrentNewClass();
    if(cur == nullptr)
        return;
    makePreview(cur);
}

