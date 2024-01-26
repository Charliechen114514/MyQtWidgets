#ifndef NEWFUNCTION_H
#define NEWFUNCTION_H
#include <QStringList>
#include <QPair>

class NewFunction
{
public:
    using       ArgType = QString;
    using       ArgName = QString;
    using       ArgComm = QString;
    using       ArgConst= bool;
    // TODO: update arg as struct
    struct Arg{
        Arg(ArgType type, ArgName name, ArgConst is_const, ArgComm comm):
            type(type), name(name), isConst(is_const), argComm(comm){}
        ArgType         type;
        ArgName         name;
        ArgConst        isConst = false;
        ArgComm         argComm;
        bool isSame(Arg& arg){return arg.name == this->name;}
    };
    enum class FUNC_TYPE{
        FOR_PRIVATE,
        FOR_PUBLIC,
        FOR_PROTECTED,
        TYPE_INDEX_MAX
    };

    enum class FUNCTION_INFO_INDEX{
        FUNCTION_INFO_ANNOUNCE,
        FUNCTION_INFO_DEFINITION,
        FUNCTION_INFO_MAX
    };


    static QString parseArg(Arg* arg);
    NewFunction() = delete;
    NewFunction(const NewFunction&) = default;
    const NewFunction& operator=(const NewFunction&){return *this;}
    NewFunction(QString belongClass, QString retType, QString funcName):
        belongClass(belongClass), ret_type(retType), funcName(funcName){}
    NewFunction(QString belongClass, QString retType, QString funcName ,QList<Arg*> args):
        belongClass(belongClass), ret_type(retType), funcName(funcName), args(args){}

    ~NewFunction();

    void        setFunctionBelongClass(QString Class){this->belongClass = Class;}
    void        setFunctionName(QString name){this->funcName = name;}
    void        setFunctionRetType(QString type){this->ret_type = type;}
    void        setFunctionArgList(QList<Arg*> args);
    void        setFuncState(bool isStatic, bool isExplicit, bool isVirtual, bool isConst);
    void        setFunctionProperty(FUNC_TYPE type){this->type = type;}
    void        setFunctionComment(QString com){this->comments = com;}

    FUNC_TYPE   getFunctionProperty(){return this->type;}
    QString     getFunctionPropertyString();
    bool        getIsStatic()   {return this->isStatic;}
    bool        getIsVirtual()  {return this->isVirtual;}
    bool        getIsExplicit() {return this->isExplicit;}
    bool        getIsConst()    {return this->isConst;}
    QString     getFunctionBelongClass(){return this->belongClass;}
    QString     getName(){return this->funcName;}
    QString     getFunctionRetType(){return this->ret_type;}
    QList<Arg*> getFunctionArgList(){return this->args;}
    QString     getFunctionComment(){return this->comments;}
    QString     getGeneratePreview();


    void        copyConfig(NewFunction* f);
    void        enArg(Arg* arg){this->args.push_back(arg);}

    bool    isSame(const NewFunction& func){return func.funcName == this->funcName;}
    bool    operator==(const NewFunction& func){return func.funcName == this->funcName;}
    // functional
    QString getFunctionAnnounceMent();
    QString getFunctionDefinition();
    QStringList getAll();

private:
    FUNC_TYPE                           type = FUNC_TYPE::FOR_PUBLIC;
    QString                             parseArgsLists();
    QString                             belongClass;
    QString                             ret_type;
    QString                             funcName;
    QList<Arg*>                         args;
    QString                             comments;
    bool                                isStatic    = false;
    bool                                isVirtual   = false;
    bool                                isExplicit  = false;
    bool                                isConst     = false;
};

#endif // NEWFUNCTION_H
