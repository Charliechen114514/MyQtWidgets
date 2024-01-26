#include "cmaketargetconfigmainwindow.h"
#include "classlistandwidgetitemmanager.h"
#include "CMakeWindowUtils.h"
#include "qtablewidget.h"
#include "targetsourceeditingwindow.h"
#include "ui_cmaketargetconfigmainwindow.h"
#include "cmakeprojectcoreinfo.h"
#include <QInputDialog>
const char* CMAKE_TARGET_CONFIIG_MAINWINDOW_TITLE = "配置生成目标";
SaveMonitor cmake_target_config_mainwindow_savemonitor;

class AdapterOfTmpLibType_LibType{
public:
    static BuildLibrary::BuildLibType trans(CMakeTargetConfigMainWindow::tmpTargetLib::LibType l){
        switch(l){
        case CMakeTargetConfigMainWindow::tmpTargetLib::LibType::SHARED:
            return BuildLibrary::BuildLibType::SHAERD;
        case CMakeTargetConfigMainWindow::tmpTargetLib::LibType::STATIC:
            return BuildLibrary::BuildLibType::STATIC;
        case CMakeTargetConfigMainWindow::tmpTargetLib::LibType::MODULE:
            return BuildLibrary::BuildLibType::MODULE;
        }
    }

    static CMakeTargetConfigMainWindow::tmpTargetLib::LibType trans(BuildLibrary::BuildLibType l){
        switch(l){
        case BuildLibrary::BuildLibType::SHAERD:
            return CMakeTargetConfigMainWindow::tmpTargetLib::LibType::SHARED;
        case BuildLibrary::BuildLibType::STATIC:
            return CMakeTargetConfigMainWindow::tmpTargetLib::LibType::STATIC;
        case BuildLibrary::BuildLibType::MODULE:
            return CMakeTargetConfigMainWindow::tmpTargetLib::LibType::MODULE;
        }
    }

    static CMakeTargetConfigMainWindow::tmpTargetLib::LibType trans(QString type){
        if(type == STATIC_STR)
            return CMakeTargetConfigMainWindow::tmpTargetLib::LibType::STATIC;
        else if(type == MODULE_STR)
            return CMakeTargetConfigMainWindow::tmpTargetLib::LibType::MODULE;
        else
            return CMakeTargetConfigMainWindow::tmpTargetLib::LibType::SHARED;
    }

};


CMakeTargetConfigMainWindow::CMakeTargetConfigMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMakeTargetConfig)
{
    ui->setupUi(this);
    AutoDelete;
    SetName(CMAKE_TARGET_CONFIIG_MAINWINDOW_TITLE);
    this->listManager = new ClassListAndWidgetItemManager;
    ui->show_current_src->resizeColumnToContents(1);
}

void CMakeTargetConfigMainWindow::bindProjectCoreinfo(CMakeProjectCoreInfo* i)
{
    this->buildTarget = i;
    init();
}

void CMakeTargetConfigMainWindow::init()
{
    if(!this->buildTarget)
        return;

    for(BuildExecutable* each : this->buildTarget->getInfos().execs)
    {
        tmpTargetExe* l = new tmpTargetExe;
        l->name = each->getExecutableFileName();
        exes.push_back(l);
    }


    for(BuildLibrary* each : this->buildTarget->getInfos().libs)
    {
        tmpTargetLib* l = new tmpTargetLib;
        l->name = each->getLibName();
        lib.push_back(l);
    }
    initConnections();
    initMyUi();
}

void CMakeTargetConfigMainWindow::initConnections()
{
    connect(this, &CMakeTargetConfigMainWindow::updateCurrentNameShow, this, &CMakeTargetConfigMainWindow::updateTextBrowserNameShown);
}

void CMakeTargetConfigMainWindow::initMyUi()
{
    for(tmpTargetLib*& l : lib)
        listManager->createWidgetItem(ui->listbtn_switch_target, l->name);
    for(tmpTargetExe*& l : exes)
        listManager->createWidgetItem(ui->listbtn_switch_target, l->name);
}

void CMakeTargetConfigMainWindow::manageTableWidget(QStringList* l)
{
    if(l == nullptr)
        return;

    clearItems();
    initItems(l);
    ui->show_current_src->setRowCount(l->size());
    for(int i = 0; i < l->size(); i++)
    {
        ui->show_current_src->setItem(i, 0, items[i]);
        qDebug() << items[i]->text();
    }
    return;
}

void CMakeTargetConfigMainWindow::initItems(QStringList *l)
{
    if(!l || l->isEmpty())
        return;
    for(QString& e : *l)
    {
        QTableWidgetItem* item = new QTableWidgetItem(e);

        item->setToolTip(e);

        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        items.push_back(item);
    }
}

void CMakeTargetConfigMainWindow::clearItems()
{
    for(QTableWidgetItem*& e : items)
    {
        delete e;
        e = nullptr;
    }
    items.clear();
}

void CMakeTargetConfigMainWindow::addTargetAtClose()
{
    buildTarget->releaseAllBuildTarget();
    for(tmpTargetLib*& l : lib)
    {
        BuildLibrary* n = BuildingFactory::buildLib();
        n->setLibName(l->name);
        for(QString& each : l->src)
            n->setBuildSources(each);
        n->type = AdapterOfTmpLibType_LibType::trans(l->libType);
        buildTarget->getInfos().libs.push_back(n);
    }
    for(tmpTargetExe*& l : exes)
    {
        BuildExecutable* e = BuildingFactory::buildExe();
        e->setExecutableFileName(l->name);
        for(QString& each : l->src)
            e->pushBackBuildExecutableFileList(each);
        buildTarget->getInfos().execs.push_back(e);
    }
}

void CMakeTargetConfigMainWindow::addTarget(QString name, TargetType type)
{
    cmake_target_config_mainwindow_savemonitor.setShellSaveQuery();
    if(type == TargetType::LIB)
    {
        QStringList libTypesEnable;
        ENABLE_BUILD_LIB_TYPE(libTypesEnable);
        QString libType = QInputDialog::getItem(this, "选择lib类型", "选择lib类型", libTypesEnable);
        if(libType == "")
            libType = STATIC_STR;
        tmpTargetLib* l = new tmpTargetLib;
        l->name = name;
        l->libType = AdapterOfTmpLibType_LibType::trans(libType);
        this->lib.push_back(l);
    }
    else
    {
        tmpTargetExe* l = new tmpTargetExe;
        l->name = name;
        this->exes.push_back(l);
    }

    listManager->createWidgetItem(ui->listbtn_switch_target, name);

}
void CMakeTargetConfigMainWindow::removeTarget(QString name, TargetType type)
{
    if(type == TargetType::LIB)
    {
        this->lib.removeIf([name](const tmpTargetLib* l)->bool{return *l == name;});
    }
    else
    {
        this->exes.removeIf([name](const tmpTargetExe* l)->bool{return *l == name;});
    }

    listManager->eraseItem(listManager->matchByName(name));
}

void CMakeTargetConfigMainWindow::closeEvent(QCloseEvent*)
{
    if(!cmake_target_config_mainwindow_savemonitor.queryForLeave(this))
        return;
    addTargetAtClose();
}

void CMakeTargetConfigMainWindow::updateViewSourceUi()
{
    QString config = listManager->getCurrentItemText();
    if(config == ""){
        QMessageBox::critical(this, "没有选中资源","没有选中资源");
        return;
    }

    for(tmpTargetExe* e : exes)
    {
        if(e->name == config)
        {
            manageTableWidget(&e->src);
            return;
        }
    }

    for(tmpTargetLib* e : lib)
    {
        if(e->name == config)
        {
            manageTableWidget(&e->src);
            return;
        }
    }
}


CMakeTargetConfigMainWindow::~CMakeTargetConfigMainWindow()
{
    clearItems();
    delete ui;
}

void CMakeTargetConfigMainWindow::on_btn_add_src_clicked()
{
    QString name = QInputDialog::getText(this, "设置名称","设置生成对象的名称", QLineEdit::Normal, "Default");
    if(name == ""){
        return;
    }
    QStringList enableType;
    ENABLE_BUILD(enableType);
    QString type = QInputDialog::getItem(this, "设置对象","设置对象的种类", enableType);
    if(type == "")
        return;
    if(type == LIB_STR)
        addTarget(name, TargetType::LIB);
    else
        addTarget(name, TargetType::EXE);
    listManager->setCurrentItemFocus(listManager->matchByName(name));
    emit updateCurrentNameShow(name);
}

void CMakeTargetConfigMainWindow::updateTextBrowserNameShown(QString name)
{
    ui->show_current_Target->setText(QString("现在编辑的是:> ") + name);
}

void CMakeTargetConfigMainWindow::showConfigSourceWindow(QStringList *l)
{
    this->targetConfigWindow = new TargetSourceEditingWindow(this);
    connect(this->targetConfigWindow, &TargetSourceEditingWindow::destroyed,
            this, &CMakeTargetConfigMainWindow::handleTheDestroyWindow);

    targetConfigWindow->bindBuildSrc(l);
    targetConfigWindow->show();
}

void CMakeTargetConfigMainWindow::handleTheDestroyWindow(){
    updateViewSourceUi();
    this->targetConfigWindow = nullptr;
}


void CMakeTargetConfigMainWindow::on_listbtn_switch_target_itemClicked(QListWidgetItem *item)
{
    listManager->setCurrentItemFocus(item);
    emit updateCurrentNameShow(item->text());
    updateViewSourceUi();
}

void CMakeTargetConfigMainWindow::on_btn_remove_src_clicked()
{
    QString erase = listManager->getCurrentItemText();
    if(erase == ""){
        QMessageBox::critical(this, "没有选中资源","没有选中资源");
        return;
    }
    for(tmpTargetExe*& exe : exes)
    {
        if(exe->name == erase){
            removeTarget(erase, TargetType::EXE);
            return;
        }
    }
    removeTarget(erase, TargetType::LIB);
}


void CMakeTargetConfigMainWindow::on_btn_config_source_clicked()
{
    QString config = listManager->getCurrentItemText();
    if(config == ""){
        QMessageBox::critical(this, "没有选中资源","没有选中资源");
        return;
    }
    for(tmpTargetExe*& exe : exes)
    {
        if(exe->name == config){
            showConfigSourceWindow(&exe->src);
            return ;
        }
    }
    for(tmpTargetLib* l : lib)
    {
        if(l->name == config){
            showConfigSourceWindow(&l->src);;
            return ;
        }
    }
}


void CMakeTargetConfigMainWindow::on_buttonBox_accepted()
{
    this->close();
}


void CMakeTargetConfigMainWindow::on_buttonBox_rejected()
{
    this->close();
}

