#include "cmakeprojectcoreinfo.h"


//  init executableFileName,
BuildExecutable* BuildExecutable::setExecutableFileName(const QString executableFileName)
{
    this->executableFileName=executableFileName;
    return this;
}


//  init buildExecutableFileList,
BuildExecutable* BuildExecutable::pushBackBuildExecutableFileList(const QString& name)
{
    this->buildExecutableFileList.push_back(name);
    return this;
}

bool BuildExecutable::removeTargetBuildingSource(const FileName& filename)
{
    return this->buildExecutableFileList.removeOne(filename);
}

//  get executableFileName,
QString BuildExecutable::getExecutableFileName()
{
    return this->executableFileName;
}

//  get buildExecutableFileList
QStringList BuildExecutable::getBuildExecutableFileList()
{
    return this->buildExecutableFileList;
}

//  init libName,
BuildLibrary* BuildLibrary::setLibName(const QString& libName)
{
    this->libName=libName;
    return this;
}


//  init buildSources,
BuildLibrary* BuildLibrary::setBuildSources(const QString& buildSources)
{
    this->buildSources.push_back(buildSources);
    return this;
}


// get libName,
QString BuildLibrary::getLibName()
{
    return this->libName;
}


// get buildSources,
QStringList BuildLibrary::getBuildSources()
{
    return this->buildSources;
}


//  *******************************************************************

//  belonging class:> BuildLibrary
//  function name:> removeTargetBuildSource
//  function params:>
//     const QString& fileName :Add Comment Here

//  return type:>
//     bool :
//  usage and more related:> remove target build source

//  *******************************************************************
bool BuildLibrary::removeTargetBuildSource(const QString& fileName)
{
    return this->buildSources.removeOne(fileName);
}

CMakeProjectCoreInfo::CMakeProjectCoreInfo(QString&& projectName){
    this->infos.name = projectName;
}




















