#ifndef EXPORTMANAGER_H
#define EXPORTMANAGER_H
#include <QStringList>
#include <QFile>
class NewClass;


// Method
// To CMake
// To normal
class ExportBuilderMethod
{
public:
    enum class BuilderMethod{
        CMAKE,
        OTHER
    };
    virtual BuilderMethod getBuilderMethod()  = 0;
    virtual bool processConfigFile(QStringList files) = 0;
};
class ExportPlaceMethod
{
public:
    enum class PlaceMethod{
        HEADER_SOURCE_PAIR,
        HEADER_SOURCE_SEPERATE,
        ALL_IN_PLACE
    };
    virtual PlaceMethod getMethod() = 0;
    virtual QStringList getPath() = 0;
    virtual void setPath(QStringList pathList){paths = pathList;}
    virtual bool enableSave(int classCount) = 0;
protected:
    QStringList paths;
};

class CMakeMethod : public ExportBuilderMethod
{
public:
    CMakeMethod() = delete;
    CMakeMethod(QString path):
        path(path){setCMakePath(path);}
    virtual bool processConfigFile(QStringList files) override;
    ExportBuilderMethod::BuilderMethod getBuilderMethod() override{
        return method;
    }
    void    setCMakePath(QString path){this->path = path;getOriginalCMakeFile(this->path);}
    QString getOriginalCMakeFile(QString CMakePath);
    QString getEditedCMakeFile(QStringList file);

    const ExportBuilderMethod::BuilderMethod method = ExportBuilderMethod::BuilderMethod::CMAKE;
private:
    QString readingFromCmake;
    QString afterEdit;
    QString path;
};

class NormalMethod : public ExportBuilderMethod
{
public:
    virtual bool processConfigFile(QStringList files[[maybe_unused]]) override{return true;}
    ExportBuilderMethod::BuilderMethod getBuilderMethod() override{
        return method;
    }
    const ExportBuilderMethod::BuilderMethod method = ExportBuilderMethod::BuilderMethod::OTHER;
};

class HeaderSourcePair : ExportPlaceMethod
{
public:
    const int headerIndex = 0;
    const int sourceIndex = 1;
    virtual QStringList getPath() override{return this->paths;}
    ExportPlaceMethod::PlaceMethod getMethod() override{
        return method;
    }
    const ExportPlaceMethod::PlaceMethod method = ExportPlaceMethod::PlaceMethod::HEADER_SOURCE_PAIR;
};



class ALLONE : ExportPlaceMethod
{
public:
    virtual QStringList getPath() override{return this->paths;};
    ExportPlaceMethod::PlaceMethod getMethod() override{
        return method;
    }
    const ExportPlaceMethod::PlaceMethod method = ExportPlaceMethod::PlaceMethod::ALL_IN_PLACE;
};

class ExportManagerWriteFile
{
public:
    static bool writeFile(QStringList& oncePath, QStringList& onceText);
};



class ExportManager
{
public:
    struct ExportInfoStruct{
        ExportInfoStruct() = delete;
        ExportInfoStruct(QString className, QString source, QString header,
            QString sourcePath, QString headerPath):
            className(className), source(source), header(header),
            sourcePath(sourcePath), headerPath(headerPath){}
        QString className;
        QString source;
        QString header;
        QString sourcePath;
        QString headerPath;
    };

    enum class GET_PATH{
        GET_ALL_HEADER_SOURCE,
        GET_HEADER,
        GET_SOURCE,
        GET_ONLY_SOURCES_FILE_NAME
    };

    ExportManager() = default;
    ExportManager(const ExportManager&) = delete;
    const ExportManager& operator=(const ExportManager&) = delete;
    ~ExportManager();

    int classesCount(){return exportClasses.size();}

    void exportToTarget(QList<NewClass*>& classes, ExportBuilderMethod& method);

    QStringList getHeaderList();

    QStringList getSourceList();

    QStringList getFileSavePath(GET_PATH pathSelection);

private:
    QList<ExportInfoStruct*> exportClasses;
    QStringList              saving;
    QStringList              sources;
    void processClassify();
    void collectClasses(QList<NewClass*>& classes);
    void processSave();
};




#endif // EXPORTMANAGER_H
