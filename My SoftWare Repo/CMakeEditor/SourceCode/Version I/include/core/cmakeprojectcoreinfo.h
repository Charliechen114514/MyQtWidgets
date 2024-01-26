#ifndef CMAKEPROJECTCOREINFO_H
#define CMAKEPROJECTCOREINFO_H

#include <QStringList>

#ifndef _BUILDEXECUTABLE_H_
#define _BUILDEXECUTABLE_H_
#define STATIC_STR  "static"
#define DYNAMIC_STR "shared"
#define MODULE_STR  "module"
#define EXE_STR     "executable"
#define LIB_STR     "lib"
#define ENABLE_BUILD(l) l.clear();l << EXE_STR << LIB_STR
#define ENABLE_BUILD_LIB_TYPE(l) l.clear(); l << STATIC_STR << DYNAMIC_STR  << MODULE_STR
class BuildTarget
{
public:
    enum class BuildType{
        EXE,
        LIB
    };
    void setType(BuildType t){type = t;}
    BuildType& getType(){return this->type;}
    virtual ~BuildTarget(){};
protected:
    friend class BuildingFactory;
    BuildTarget() = default;
    BuildType type = BuildType::EXE;
};


//  *******************************************************************
//  Class Name:> BuildExecutable
//  Usage and comments:> This is a class that creates executable files
//
//  *******************************************************************
class BuildExecutable : public BuildTarget{
public:
    using FileName = QString;
    using FileNameList = QStringList;
    ~BuildExecutable(){};
    const BuildExecutable& operator=(const BuildExecutable&) = delete;
    BuildExecutable(const BuildExecutable&) = delete;
    // static:
    // setters:
    BuildExecutable* setExecutableFileName(const FileName executableFileName);
    BuildExecutable* pushBackBuildExecutableFileList(const FileName& fileName);
    bool removeTargetBuildingSource(const FileName& filename);
    // getters:
    QString     getExecutableFileName();
    QStringList getBuildExecutableFileList();

    friend bool operator==(const BuildExecutable& l1, const BuildExecutable& l2){
        return l1.executableFileName == l2.executableFileName;
    }
protected:
    friend class BuildingFactory;
    BuildExecutable() = default;

private:
    //  Private Member:>
    QString executableFileName;//  the target we will get
    QStringList buildExecutableFileList;//  a lists contains the whole building sources
};
#endif

//  *******************************************************************
//  Class Name:> BuildLibrary
//  Usage and comments:> This is a class aims to build the library generated
//
//  *******************************************************************
class BuildLibrary : public BuildTarget{
public:
    enum class BuildLibType{
        SHAERD,
        STATIC,
        MODULE
    }type = BuildLibType::STATIC;
    ~BuildLibrary(){};
    // static:
    // setters:
    BuildLibrary* setLibName(const QString& libName);
    BuildLibrary* setBuildSources(const QString& buildSource);
    // getters:
    QString getLibName();
    QStringList getBuildSources();
    //  Public function:>
    bool removeTargetBuildSource(const QString& fileName);

    friend bool operator==(const BuildLibrary& l1, const BuildLibrary& l2){
        return l1.libName == l2.libName;
    }

protected:
    BuildLibrary() = default;
private:
    friend class BuildingFactory;
    //  Private Member:>    BuildLibrary() = default;
    QString libName;//  the target lib name
    QStringList buildSources;//  the libs contains the sources

};
class CMakeProjectCoreInfo
{
private:
    struct CMakeCore_Info{
        QString                     name;
        QList<BuildExecutable*>     execs;
        QList<BuildLibrary*>        libs;
        QString                     languages{"CXX"};
        short                       project_major_version = 1;
        short                       project_minor_version = 0;
        QString                     descriptions{""};
    } infos;

public:
    const short DEFAULT_PROJECT_MAJOR_VERSION   = 1;
    const short DEFAULT_PROJECT_MINOR_VERSION   = 0;
    const QString DEFAULT_LANG                  = "CXX";
    CMakeProjectCoreInfo() = delete;
    CMakeProjectCoreInfo(const CMakeProjectCoreInfo&) = delete;
    const CMakeProjectCoreInfo& operator=(const CMakeProjectCoreInfo&) = delete;
    CMakeProjectCoreInfo(QString&& projectName);
    ~CMakeProjectCoreInfo(){
        releaseAllBuildTarget();
    }
    CMakeCore_Info& getInfos(){return this->infos;}

    void setProjectName(const QString& name){infos.name = name;}

    void setProjectEnableLanguage(const QString& langs){
        infos.languages = langs;
    }

    void setProjectVersion(short major, short minor){
        infos.project_major_version = major;
        infos.project_minor_version = minor;
    }

    void addExecutable(BuildExecutable* const add){infos.execs.push_back(add);}
    void addLibrary(BuildLibrary* const add){infos.libs.push_back(add);}
    bool removeLib(BuildLibrary*  add){
        return infos.libs.removeOne(add);
    }
    bool removeExe(BuildExecutable* add){
        return infos.execs.removeOne(add);
    }

    void releaseAllBuildTarget(){
        for(BuildLibrary* l : this->getInfos().libs)
            delete l;
        for(BuildExecutable* l : this->getInfos().execs)
            delete l;
        this->infos.execs.clear();
        this->infos.libs.clear();
    }

};

class BuildingFactory
{
public:
    static BuildExecutable* buildExe(){
        return new BuildExecutable();
    }
    static BuildLibrary* buildLib(){
        return new BuildLibrary();
    }
};

#endif // CMAKEPROJECTCOREINFO_H
