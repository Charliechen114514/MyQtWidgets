#include "exportmanager.h"
#include "newclass.h"
#include "qmessagebox.h"
#define EXECUTABLE_SETTING  "add_executable"
#define GET_FILE_NAME(className) className + ".cpp"

bool ExportManagerWriteFile::writeFile(QStringList& oncePath, QStringList& onceText){
    if(oncePath.size() != onceText.size())
        return false;
    int currentWirte = 0;
    for(QString& each : oncePath)
    {
        QFile file(each);
        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(nullptr, "访问错误","无法访问目标地址:> " + each);
            continue;
        }

        file.write(onceText[currentWirte].toStdString().c_str());
        file.close();
        currentWirte++;
    }
    return true;
}

ExportManager::~ExportManager()
{
    for(ExportInfoStruct*& each : this->exportClasses)
        if(each)
        {
            delete each;
            each = nullptr;
        }
}

void ExportManager::collectClasses(QList<NewClass*>& classes)
{
    for(NewClass*& each : classes)
    {
        this->exportClasses.push_back(new ExportInfoStruct(each->getName(),
                                                           each->generateSource(),
                                                           each->generateHeader(),
                                                           each->getSourceSavePlace(),
                                                           each->getHeaderSavePlace()));
    }

}

void ExportManager::processClassify()
{
    saving.clear();
    sources.clear();
    for(ExportInfoStruct*& each : this->exportClasses)
    {
        saving  << each->sourcePath << each->headerPath;
        sources << each->source << each->header;
    }
}
void ExportManager::exportToTarget(QList<NewClass*>& classes, ExportBuilderMethod& method)
{
    collectClasses(classes);
    processClassify();
    this->processSave();
    method.processConfigFile(getFileSavePath(GET_PATH::GET_ONLY_SOURCES_FILE_NAME));
    return;
}

QStringList ExportManager::getHeaderList()
{
    QStringList res;
    for(ExportInfoStruct*& each : this->exportClasses)
        res << each->header;

    return res;
}

QStringList ExportManager::getSourceList()
{
    QStringList res;
    for(ExportInfoStruct*& each : this->exportClasses)
        res << each->source;

    return res;
}

QStringList ExportManager::getFileSavePath(GET_PATH pathSelection)
{
    QStringList res;
    switch(pathSelection)
    {
    case GET_PATH::GET_ALL_HEADER_SOURCE:
        for(ExportInfoStruct*& each : this->exportClasses)
            res << each->headerPath;
        for(ExportInfoStruct*& each : this->exportClasses)
            res << each->sourcePath;
        break;
    case GET_PATH::GET_HEADER:
        for(ExportInfoStruct*& each : this->exportClasses)
            res << each->headerPath;
        break;
    case GET_PATH::GET_SOURCE:
        for(ExportInfoStruct*& each : this->exportClasses)
            res << each->sourcePath;
        break;
    case GET_PATH::GET_ONLY_SOURCES_FILE_NAME:
        for(ExportInfoStruct*& each : this->exportClasses)
            res << each->className + ".cpp";
        break;
    }
    qDebug() << res;
    return res;
}

bool CMakeMethod::processConfigFile(QStringList file)
{
    getEditedCMakeFile(file);
    QString writing = this->afterEdit;
    QFile editCmakeFile(path);
    if(editCmakeFile.open(QIODevice::WriteOnly))
    {
        editCmakeFile.write(writing.toStdString().c_str());
        return true;
    }
    else
    {
        QMessageBox::critical(nullptr,"出错了","无法打开指定的Cmake文件");
        return false;
    }
}

QString CMakeMethod::getOriginalCMakeFile(QString CMakePath)
{
    QFile cmakeFile(CMakePath);
    if(cmakeFile.open(QIODevice::ReadOnly))
    {
        this->readingFromCmake =  cmakeFile.readAll();
        return this->readingFromCmake;
    }
    else
    {
        QMessageBox::critical(nullptr,"出错了","无法打开指定的Cmake文件");
        return "";
    }
}

QString CMakeMethod::getEditedCMakeFile(QStringList file)
{
    QString tryEdit = this->readingFromCmake;

    tryEdit += QString("\n\n\n") + EXECUTABLE_SETTING + "(";
    for(QString& each : file)
        tryEdit += each + " ";
    tryEdit += ")";
    this->afterEdit = this->readingFromCmake + tryEdit;
    return this->afterEdit;
}


void ExportManager::processSave()
{
    ExportManagerWriteFile::writeFile(saving, sources);
}



