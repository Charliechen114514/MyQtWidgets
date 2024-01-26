#include "newfunction.h"
#define UNINIT_CHECK_UNALLOWED(FALSE_RET) \
do{\
    if(this->belongClass.isEmpty() || \
        this->ret_type.isEmpty() || \
        this->funcName.isEmpty())\
        return FALSE_RET;\
}while(0)

#define AREA_WORKING_SYMBOL "::"
#define SPACE               " "
#define SLASH               "\n"
#define SENTENCE_END        ";"
#define SETTER_STATE_TYPE   QString("bool")
#define SETTER_PREFIX       "set"
#define DEF_RET_TRUE_SETTER "return true;"
#define GETTER_PREFIX       "get"
#define PARAMLIST_START     "("
#define PARAMLIST_END       ")"
#define FUNC_START          "{"
#define FUNC_END            "}"
#define SEPERATOR           ", "
#define FUNC_BODY_EMPTY     SLASH + FUNC_START + SLASH +SLASH + FUNC_END
#define FUNC_BODY_STH(str)  SLASH + FUNC_START + SLASH + (str) +SLASH + FUNC_END
#define ISSTATIC(state)     (state?"static ":"")
#define ISVIRTUAL(state)    (state?"virtual ":"")
#define ISEXPLICIT(state)   (state?"explicit ":"")
#define ISCONST(state)      (state?" const" : "")

NewFunction::~NewFunction()
{
    for(Arg*& each : this->args)
        if(each)
        {
            delete each;
            each = nullptr;
        }
}

void NewFunction::setFunctionArgList(QList<Arg*> args)
{
    for(Arg*& each : this->args)
        if(each)
        {
            delete each;
            each = nullptr;
        }
    this->args.clear();

    for(Arg* each : args)
    {
        this->enArg(new Arg(each->type, each->name, each->isConst, each->argComm));
    }
}

void NewFunction::copyConfig(NewFunction* f)
{
    this->setFunctionArgList(f->getFunctionArgList());
    this->setFunctionComment(f->getFunctionComment());
    this->setFunctionName(f->getName());
    this->setFunctionProperty(f->getFunctionProperty());
    this->setFunctionRetType(f->getFunctionRetType());
    this->setFuncState(f->getIsStatic(),f->getIsVirtual(),f->getIsVirtual(), f->getIsConst());
}

QString NewFunction::getFunctionPropertyString()
{
    switch(this->type)
    {
    case FUNC_TYPE::FOR_PRIVATE:
        return "private ";
    case FUNC_TYPE::FOR_PUBLIC:
        return "public ";
    case FUNC_TYPE::FOR_PROTECTED:
        return "protected ";
    default:
        return "";
    }
}
QString NewFunction::getGeneratePreview()
{
    return
           "//  " +
           this->getFunctionComment() + "\n" +
           "this is a: " + ISSTATIC(isStatic) + ISCONST(isConst) + ISVIRTUAL(isVirtual) + ISEXPLICIT(isExplicit) +
           this->getFunctionPropertyString() + " function\n" +
           this->getFunctionAnnounceMent();
}



// *******************************************************************
//  generate templates are here:
//  ${type} ${memberName};
// *******************************************************************
QString NewFunction::parseArg(Arg *arg)
{
    return (arg->isConst? "const " : "") + arg->type + SPACE + arg->name;
}

void NewFunction::setFuncState(bool isStatic, bool isExplicit, bool isVirtual, bool isConst)
{
    this->isStatic = isStatic;
    this->isExplicit = isExplicit;
    this->isVirtual = isVirtual;
    this->isConst = isConst;
}

// *******************************************************************
//  generate templates are here:
//  (${type} ${memberName}, ...);
// *******************************************************************
QString NewFunction::parseArgsLists()
{
    UNINIT_CHECK_UNALLOWED("");
    QString argsString = PARAMLIST_START;
    int count = args.size();
    int cur = 0;
    for(NewFunction::Arg*& eachArg : args)
    {
        argsString += NewFunction::parseArg(eachArg);
        if(cur < count - 1)
        {
            argsString += SEPERATOR;
            cur++;
        }
    }
    argsString += PARAMLIST_END;
    return argsString;
}

// *******************************************************************
//  generate templates are here:
//  [static/virtual] [explicit]${this->ret_type) ${this->funcName}(${type} ${memberName}, ...);
// *******************************************************************
QString NewFunction::getFunctionAnnounceMent()
{
    UNINIT_CHECK_UNALLOWED("");
    QString announcement;
    if(!isStatic)
        announcement += ISVIRTUAL(isVirtual);
    else
        announcement += ISSTATIC(isStatic);

    announcement += ISEXPLICIT(isExplicit);


    setFuncState(isStatic, isExplicit, isVirtual, isConst);

    return announcement + this->ret_type + SPACE + this->funcName + parseArgsLists() + ISCONST(isConst) + SENTENCE_END;
}

// *******************************************************************
//  generate templates are here:
//  ${this->ret_type) ${this->funcName}(${type} ${memberName}, ...)
//  {
//
//  }
// *******************************************************************
QString NewFunction::getFunctionDefinition()
{
    UNINIT_CHECK_UNALLOWED("");

    return this->ret_type + SPACE + this->belongClass + AREA_WORKING_SYMBOL + this->funcName + \
           parseArgsLists() + ISCONST(this->isConst) \
           + FUNC_BODY_EMPTY;
}

//  *******************************************************************
//  get the all infos:>
//  AnnounceMents
//  Definitions
//  *******************************************************************
QStringList NewFunction::getAll()
{
    UNINIT_CHECK_UNALLOWED(QStringList());
    QStringList s;
    s << getFunctionAnnounceMent();
    s << getFunctionDefinition();

    if(s.size() != static_cast<int>(FUNCTION_INFO_INDEX::FUNCTION_INFO_MAX) || s[0] == "" || s[1] == "")
        return QStringList();

    return s;
}


