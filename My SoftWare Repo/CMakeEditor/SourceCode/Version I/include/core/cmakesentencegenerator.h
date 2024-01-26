#ifndef CMAKESENTENCEGENERATOR_H
#define CMAKESENTENCEGENERATOR_H
#include "cmakeprojectcoreinfo.h"
#include <QStringList>

class CMakeSentenceGenerator
{
public:
    virtual QString generate() = 0;
};

class CMakeSelfInfo;
class CMakeSelfInfoSentenseGenerator : public CMakeSentenceGenerator
{
public:
    // vars
    const QString KEY_CMAKE_MAJOR_VERSION   = "CMAKE_MAJOR_VERSION";
    const QString KEY_CMAKE_MIJOR_VERSION   = "CMAKE_MIJOR_VERSION";
    const QString KEY_CMAKE_VERSION_STRING  = "CMAKE_VERSION";
    const QString KEY_VERSION_LESS_STRING   = "VERSION_LESS";
    const QString KEY_VERISON_String        = "VERSION";

    // sentence keys
    const QString KEY_CMAKE_MINI_VERSION_REQUIRED = "cmake_minimum_required";

    // Must bind to a CMakeSelfInfo
    CMakeSelfInfoSentenseGenerator() = delete;
    CMakeSelfInfoSentenseGenerator(CMakeSelfInfo* self): infos(self){}
    const CMakeSelfInfoSentenseGenerator& operator=(const CMakeSelfInfoSentenseGenerator&) = delete;
    CMakeSelfInfoSentenseGenerator(const CMakeSelfInfoSentenseGenerator&) = delete;

    CMakeSelfInfoSentenseGenerator* bindCMakeSelfInfo(CMakeSelfInfo* self){
        infos = self;
        return this;
    }

    void set_required_auto_adjust_version(bool state){
        required_auto_adjust_version = state;
    }

    QString generate() override;

private:
    QString generateVersionSelection();
    QString generateVersionOfDetectingCMakeString();
    CMakeSelfInfo*  infos = nullptr;
    bool required_auto_adjust_version = true;
};






//  *******************************************************************
//  Class Name:> CMakeProjectGenerator
//  Usage and comments:> Generate the sentence like project()
//  currently including name, version, and the descriptions setting
//
//  *******************************************************************
class CMakeProjectGenerator : public CMakeSentenceGenerator
{
public:
    CMakeProjectGenerator() = delete;
    const CMakeProjectGenerator& operator=(const CMakeProjectGenerator&) = delete;
    CMakeProjectGenerator(const CMakeProjectGenerator&) = delete;
    CMakeProjectGenerator(CMakeProjectCoreInfo* info):info(info){}
    // static:
    // setters:
    // getters:
    void rebind(CMakeProjectCoreInfo* info){this->info = info;}
    QString generate() override;
protected:
    const QString KEY_Project       = "project";
    const QString KEY_VERSION       = "VERSION";
    const QString KEY_DESCRIPTION   = "DESCRIPTION";
    const QString KEY_LANGUAGE      = "LANGUAGES";
    QString nameString(){
        return info->getInfos().name + "\n";
    }

    QString versionString(){
        return KEY_VERSION + " " + \
               QString::number(info->getInfos().project_major_version) + "." + QString::number(info->getInfos().project_minor_version) + "\n";
    }

    QString descriptionString(){
        return KEY_DESCRIPTION + " \"" + info->getInfos().descriptions + "\"\n";
    }

    QString languageString(){
        return KEY_LANGUAGE + " \"" + info->getInfos().languages + "\"\n";
    }

private:
    CMakeProjectCoreInfo*           info = nullptr;
};

//  *******************************************************************
//  Class Name:> CMakeGenerateFileSentenceGenerator
//  Usage and comments:> These generator eventually generate files generation cmake centenses,
//  so any generation generator is the son class of this class
//
//  *******************************************************************
class CMakeGenerateFileSentenceGenerator : public CMakeSentenceGenerator
{
public:
    CMakeGenerateFileSentenceGenerator() = delete;
    const CMakeGenerateFileSentenceGenerator& operator=(const CMakeGenerateFileSentenceGenerator&) = delete;
    CMakeGenerateFileSentenceGenerator(const CMakeGenerateFileSentenceGenerator&) = delete;
    CMakeGenerateFileSentenceGenerator(const QString generatedFileName):generatedFileName(generatedFileName){};
    // static:
    // setters:
    bool setGeneratedFileName(const QString generatedFileName);
    bool pushBackSources(const QString sources);
    bool removeSourceTarget(const QString sources);
    // getters:
    QString getGeneratedFileName();
    QStringList getSources();

    //  Public function:>
    QString generate()  = 0;
protected:
    //  Private Member:>
    QString generatedFileName;//  the file generated name

    QStringList sources;//  Add Comment Here

    QString getLinedSources();
};

//  *******************************************************************
//  Class Name:> CMakeAddExecutableGenerator
//  Usage and comments:> the son instance of CMakeGenerateFileSentenceGenerator,
//  using in generate the Executable Files
//
//  *******************************************************************
class CMakeAddExecutableGenerator : public CMakeGenerateFileSentenceGenerator
{
public:
    CMakeAddExecutableGenerator() = delete;
    const CMakeAddExecutableGenerator& operator=(const CMakeAddExecutableGenerator&) = delete;
    CMakeAddExecutableGenerator(const CMakeAddExecutableGenerator&) = delete;
    CMakeAddExecutableGenerator(const QString name):CMakeGenerateFileSentenceGenerator(name){};
    // static:
    // setters:
    // getters:
    //  Public function:>
    QString generate() override;
protected:
    const QString KEY_ADD_EXECUATBLE = "add_executable";
private:
};



//  *******************************************************************
//  Class Name:> CMakeAddLibraryGenerator
//  Usage and comments:> generate the sentence of add_library,
//  son instance of the CMakeGenerateFileGenerator
//
//  *******************************************************************
class CMakeAddLibraryGenerator: public CMakeGenerateFileSentenceGenerator
{
public:
    enum class LibraryState{
        STATIC,
        DYNAMIC,
        MODULE
    };

    CMakeAddLibraryGenerator() = delete;
    const CMakeAddLibraryGenerator& operator=(const CMakeAddLibraryGenerator&) = delete;
    CMakeAddLibraryGenerator(const CMakeAddLibraryGenerator&) = delete;

    CMakeAddLibraryGenerator(const QString name):CMakeGenerateFileSentenceGenerator(name){};

    // static:
    // setters:
    CMakeAddLibraryGenerator* setLibraryState(const LibraryState libraryState){
        this->libraryState = libraryState;
        return this;
    }
    // getters:
    LibraryState getLibraryState(){return libraryState;}

    QString generate() override;

protected:
    const QString   KEY_ADD_LIBRARY   = "add_library";
    const QString   KEY_STATIC        = "STATIC";
    const QString   KEY_SHARED        = "SHARED";
    const QString   KEY_MODULE        = "MODULE";

    // the var indicates the current state the lib wiill build
    LibraryState    libraryState       = LibraryState::STATIC;
    QString         getLibraryStateString();
private:
};

class CMakeAddWholeGenerator: public CMakeSentenceGenerator
{
public:
    CMakeAddWholeGenerator() = delete;
    CMakeAddWholeGenerator(const CMakeAddWholeGenerator&) = delete;
    const CMakeAddWholeGenerator& operator=(const CMakeAddWholeGenerator&) = delete;
    CMakeAddWholeGenerator(CMakeProjectCoreInfo* info):info(info){};

    QString generate() override;

private:
    CMakeProjectCoreInfo* info = nullptr;
};






#endif // CMAKESENTENCEGENERATOR_H
