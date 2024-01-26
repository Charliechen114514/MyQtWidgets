#include "cmakesentencegenerator.h"
#include "cmakeselfinfo.h"
#include "CMakeUtils.h"
#include <QFileInfo>
class AdapterOfGeneratorAndCoreSources
{
public:
    static CMakeAddLibraryGenerator::LibraryState trans(BuildLibrary::BuildLibType t)
    {
        switch(t)
        {

        case BuildLibrary::BuildLibType::SHAERD:
            return CMakeAddLibraryGenerator::LibraryState::DYNAMIC;
        case BuildLibrary::BuildLibType::STATIC:
            return CMakeAddLibraryGenerator::LibraryState::STATIC;
        case BuildLibrary::BuildLibType::MODULE:
            return CMakeAddLibraryGenerator::LibraryState::MODULE;
        }
    }
};


//  if we need auto adjust version
//  if(${CMAKE_VERSION} VERSION_LESS major.minor)
//      cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
//      message(STATUS "version less major.minor, switch to ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}")
//  else
//      cmake_minimum_required(VERSION major.minor)
//      message(STATUS "version is good, no need to switch! current CMake version is major.minor\n")
//  endif()
//
//  if we don't, generate this directly
//      cmake_minimum_required(VERSION major.minor)


QString CMakeSelfInfoSentenseGenerator::generateVersionSelection()
{
    QString version_str = this->infos->CMake_Version_String();
    if(this->required_auto_adjust_version)
    {
        QString res;

        res += IF(GET_VALUE_STR(KEY_CMAKE_VERSION_STRING) + SPACE + KEY_VERSION_LESS_STRING + this->infos->CMake_Version_String());
        res += QString(SLASH) + TAB;
        res += KEY_CMAKE_MINI_VERSION_REQUIRED + SCOPE_BEGIN + KEY_VERISON_String + SPACE + generateVersionOfDetectingCMakeString() + SCOPE_END;
        res += QString(SLASH) + TAB;
        res += "message(STATUS \"version less " + version_str + ", switch to " + generateVersionOfDetectingCMakeString() + "\"\n";
        res += ELSE;
        res += QString(SLASH) + TAB;
        res += KEY_CMAKE_MINI_VERSION_REQUIRED + SCOPE_BEGIN + KEY_VERISON_String + SPACE +
               version_str + SCOPE_END;
        res += "message(STATUS \"version is good, no need to switch! current CMake version is " + this->infos->CMake_Version_String() + "\"\n";
        return res;
    }
    else
    {
        return KEY_CMAKE_MINI_VERSION_REQUIRED + SCOPE_BEGIN + KEY_VERISON_String + SPACE +
               version_str + SCOPE_END;
    }
}


QString CMakeSelfInfoSentenseGenerator::generateVersionOfDetectingCMakeString()
{
    return GET_VALUE_STR(KEY_CMAKE_MAJOR_VERSION) + "." + GET_VALUE_STR(KEY_CMAKE_MIJOR_VERSION);
}

QString CMakeSelfInfoSentenseGenerator::generate()
{
    return generateVersionSelection() + SLASH + SLASH;
}


// TMPLATES
//  project(
//      projectName
//      VERSION         version
//      DESCRIPTION     des
// )

QString CMakeProjectGenerator::generate()
{
    return \
        KEY_Project + SCOPE_BEGIN + SLASH +
        TAB + nameString() + \
        TAB + versionString() + \
        TAB + languageString() + \
        TAB + descriptionString() + \
        SCOPE_END + SLASH + SLASH + SLASH;
}


//  init generatedFileName,
bool CMakeGenerateFileSentenceGenerator::setGeneratedFileName(const QString generatedFileName)
{
    this->generatedFileName=generatedFileName;
    return true;
}


//  init sources,
bool CMakeGenerateFileSentenceGenerator::pushBackSources(const QString sources)
{
    this->sources.push_back(sources);
    return true;
}

//  remove sources
bool CMakeGenerateFileSentenceGenerator::removeSourceTarget(const QString sources)
{
    return this->sources.removeOne(sources);
}


// get generatedFileName,
QString CMakeGenerateFileSentenceGenerator::getGeneratedFileName()
{
    return this->generatedFileName;
}


// get sources,
QStringList CMakeGenerateFileSentenceGenerator::getSources()
{
    return this->sources;
}

// get Lined Sources
QString CMakeGenerateFileSentenceGenerator::getLinedSources()
{
    QString res;
    for(const QString& eachSources : this->sources)
    {
        res += QFileInfo(eachSources).fileName() + " ";
    }
    return res;
}

//  *******************************************************************

//  belonging class:> CMakeAddLibraryGenerator
//  function name:> getLibraryStateString
//  function params:>
//  return type:>
//     QString :
//  usage and more related:> get librarystate string anyway

//  *******************************************************************
QString CMakeAddLibraryGenerator::getLibraryStateString()
{
    switch (this->libraryState) {
    case LibraryState::STATIC:
        return KEY_STATIC;
    case LibraryState::DYNAMIC:
        return KEY_SHARED;
    case LibraryState::MODULE:
        return KEY_MODULE;
    default:
        return KEY_STATIC;
    }
}

// generate the add_executable
//  *******************************************************************

//  belonging class:> CMakeAddExecutableGenerator
//  function name:> generate
//  function params:>
//  return type:>
//     QString :
//  usage and more related:> generate the add_executable_sentences
//  templates:
//  add_executable(target_name sources)
//  *******************************************************************
QString CMakeAddExecutableGenerator::generate()
{
    return KEY_ADD_EXECUATBLE + SCOPE_BEGIN + this->generatedFileName + SPACE + \
           this->getLinedSources() + SCOPE_END;
}

//  *******************************************************************

//  belonging class:> CMakeAddLibraryGenerator
//  function name:> generate
//  function params:>
//  return type:>
//     QString :
//  usage and more related:>
//  templates:
//  add_library(target_name what_kind sources)
//  *******************************************************************
QString CMakeAddLibraryGenerator::generate()
{
    return KEY_ADD_LIBRARY + SCOPE_BEGIN + this->generatedFileName + SPACE + \
            getLibraryStateString() + SPACE + this->getLinedSources() + SCOPE_END;
}

QString CMakeAddWholeGenerator::generate()
{
    if(!info)
        return "";
    QString sentenceEach;

    for(BuildExecutable* each : info->getInfos().execs)
    {
        CMakeAddExecutableGenerator as(each->getExecutableFileName());
        for(QString& each_src : each->getBuildExecutableFileList())
            as.pushBackSources(each_src);
        sentenceEach += as.generate() + "\n\n";
    }

    for(BuildLibrary* each : info->getInfos().libs)
    {
        CMakeAddLibraryGenerator as(each->getLibName());
        as.setLibraryState(AdapterOfGeneratorAndCoreSources::trans(each->type));
        for(QString& each_src : each->getBuildSources())
            as.pushBackSources(each_src);
        sentenceEach += as.generate() + "\n\n";
    }

    return sentenceEach;
}



