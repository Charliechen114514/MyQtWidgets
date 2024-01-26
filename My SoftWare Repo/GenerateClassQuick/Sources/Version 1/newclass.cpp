#include "newclass.h"
#include "commentgenerator.h"
#include <QDebug>
void clearMembers(QList<NewMember*>& lists)
{
    for(NewMember*& m : lists)
    {
        if(m)
            delete m;
        m = nullptr;
    }
}

void clearFunction(QList<NewFunction*>& lists)
{
    for(NewFunction*& m : lists)
    {
        if(m)
            delete m;
        m = nullptr;
    }
}

NewClass::~NewClass()
{
    // clear members
    clearMembers(this->staticMembers);
    clearMembers(this->privateMembers);
    clearMembers(this->publicMembers);

    // clear functions
    clearFunction(this->privateFunctions);
    clearFunction(this->protectedFunctions);
    clearFunction(this->publicFunctions);
}

void NewClass::adjustMembers()
{
    // members
    QList<NewMember*> newPublic;
    QList<NewMember*> newStatic;
    QList<NewMember*> newPrivate;
    for(NewMember*& each : this->staticMembers)
    {
        if(each->getIsStatic())
        {
            newStatic << each;
        }
    }

    for(NewMember*& each : this->privateMembers)
    {
        if(each->getMemberPropertyType() == NewMember::MEMBER_TYPE::FOR_PRIVATE)
            newPrivate << each;
        if(each->getMemberPropertyType() == NewMember::MEMBER_TYPE::FOR_PUBLIC)
            newPublic << each;
    }

    for(NewMember*& each : this->publicMembers)
    {
        if(each->getMemberPropertyType() == NewMember::MEMBER_TYPE::FOR_PRIVATE)
            newPrivate << each;
        if(each->getMemberPropertyType() == NewMember::MEMBER_TYPE::FOR_PUBLIC)
            newPublic << each;
    }

    this->privateMembers = newPrivate;
    this->publicMembers = newPublic;
    this->staticMembers = newStatic;
}

void NewClass::adjustFunctions()
{
    QList<NewFunction*> funcpublic;
    QList<NewFunction*> funcprivate;
    QList<NewFunction*> funcprotected;

    for(NewFunction* f : this->publicFunctions)
    {
        if(f->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PUBLIC)
            funcpublic << f;
        if(f->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PRIVATE)
            funcprivate << f;
        if(f->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PROTECTED)
            funcprotected << f;
    }


    for(NewFunction* f : this->privateFunctions)
    {
        if(f->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PUBLIC)
            funcpublic << f;
        if(f->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PRIVATE)
            funcprivate << f;
        if(f->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PROTECTED)
            funcprotected << f;
    }

    for(NewFunction* f : this->protectedFunctions)
    {
        if(f->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PUBLIC)
            funcpublic << f;
        if(f->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PRIVATE)
            funcprivate << f;
        if(f->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PROTECTED)
            funcprotected << f;
    }

    this->publicFunctions = funcpublic;
    this->privateFunctions = funcprivate;
    this->protectedFunctions = funcprotected;

}

void NewClass::addMember(NewMember *member)
{
    if(member->getIsStatic())
    {
        this->staticMembers.push_back(member);
    }
    else
    {
        if(member->getMemberPropertyType() == NewMember::MEMBER_TYPE::FOR_PRIVATE)
            this->privateMembers.push_back(member);
        else
        {
            this->publicMembers.push_back(member);
        }
    }

}

void NewClass::addFunction(NewFunction* function)
{
    if(function->getIsStatic())
    {
        this->staticFunctions.push_back(function);
    }
    else
    {
        if(function->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PRIVATE)
            this->privateFunctions.push_back(function);
        else if(function->getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PROTECTED)
            this->protectedFunctions.push_back(function);
        else
            this->publicFunctions.push_back(function);
    }
}

bool NewClass::dismissMember(NewMember* m)
{
    if(this->publicMembers.removeOne(m))
    {
        if(m != nullptr)
            delete m;
        return true;
    }

    if(this->privateMembers.removeOne(m))
    {
        if(m != nullptr)
            delete m;
        return true;
    }
    else
        return false;
}

bool NewClass::dismissFunction(NewFunction* f)
{
    if(this->publicFunctions.removeOne(f))
    {
        if(f != nullptr)
            delete f;
        return true;
    }

    if(this->protectedFunctions.removeOne(f))
    {
        if(f != nullptr)
            delete f;
        return true;
    }

    if(this->privateFunctions.removeOne(f))
    {
        if(f != nullptr)
            delete f;
        return true;
    }

    return false;
}


#define COMMENT                                     "// "
#define SENTENCE_END                                 ";"
#define SLASH                                       "\n"
#define CLASS_DEFINE_BEGIN                          "{" + SLASH
#define CLASS_DEFINE_END                             "}" + SENTENCE_END + SLASH
#define SPACE                                       " "
#define TAB                                         "   "
#define START_AREA                                  ":"
#define INCLUDE_WHITE                               SLASH + SLASH + SLASH
#define FUNC_SPACE                                  QString(SLASH) + SLASH + SLASH
#define CALLER                                      "()"
#define DELETE                                      " = delete"
#define IS_DELETE(state)                            ((state)?"":DELETE)
#define DEF_BODY                                    "{\n\n}\n"
// header
#define AVOID_MULTI_BEG(ClassName)                  QString("#ifndef _") + ClassName.toUpper() + "_H_" + SLASH + \
                                                    QString("#define _") + ClassName.toUpper() + "_H_" + SLASH
#define AVOID_MULTI_END                             QString("#endif") + SLASH

#define CLASS_KEY(ClassName)                        QString("class")        + SPACE + ClassName
#define PUBLIC_KEY                                  QString("public")       + START_AREA + SLASH
#define PRIVATE_KEY                                 QString("private")      + START_AREA + SLASH
#define PROTECT_KEY                                 QString("protected")    + START_AREA + SLASH
#define STATIC_KEY                                  QString("// static:") + SLASH
#define STATIC                                      QString("static") + SPACE
#define SETTER_KEY                                  QString("// setters: ") + SLASH
#define GETTER_KEY                                  QString("// getters: ") + SLASH
#define DESTRUCTOR(className)                       "~" + className + CALLER + SENTENCE_END + SLASH
// source
#define INCLUDE_NECCESS(ClassName)                  QString("#include\"") + ClassName + ".h\"" + SLASH
#define SET_FUNC_RET_TYPE                           "bool"
#define SET_FUNC_NAME(name)                         QString("set") + UpperFirst(name)


// *******************************************************************
//  generate templates are here:
//  #ifndef _${className.upper}_H_
//  #define _${className.upper}_H_
//
//
//
// *******************************************************************
QString NewClass::generateAvoidMultiDef()
{
    return AVOID_MULTI_BEG(this->ClassName) +INCLUDE_WHITE;
}

//  *******************************************************************
//  {className}() [= delete]
//  const {className}& operator=(const {className}&) [= delete]
//  {className}(const {className}&) [= delete]
//  ~{className}()
//  *******************************************************************
QString NewClass::generateCopyConstructorAnnouncement()
{
    // def
    return TAB + this->ClassName + CALLER + IS_DELETE(this->requiredDef) + SENTENCE_END + SLASH +
    // =
    TAB + "const" + SPACE + this->ClassName + "& operator=" + "(const " + this->ClassName + "&)" + \
           IS_DELETE(this->requiredCopy) +  SENTENCE_END + SLASH +
    // copy
    TAB + this->ClassName + "(const " + this->ClassName + "&)" + \
           IS_DELETE(this->requiredEqualCopy) + SENTENCE_END + SLASH + \
    // distructor
    TAB + DESTRUCTOR(this->ClassName);
}

//  *******************************************************************
//  {className}() {} or ""
//  const {className}& operator=(const {className}&) {} or ""
//  {className}(const {className}&) {} or ""
//  ~{className}()
//  *******************************************************************
QString NewClass::generateCopyConstructorDefinition()
{
    QString res;
    if(requiredDef)
        res += this->ClassName + CALLER + DEF_BODY + SLASH;
    if(requiredCopy)
        res += this->ClassName + "(const " + this->ClassName + "&)" + DEF_BODY + SLASH;
    if(requiredEqualCopy)
        res += QString("const") + SPACE + this->ClassName + "& operator=" + "(const " + this->ClassName + "&)" + \
               DEF_BODY + SLASH;

    return res;
}

//  *******************************************************************
//  generate templates are here:
//  bool set${memberName.Upper}(${type} ${memberName});
//  etc...
//  *******************************************************************
QString NewClass::generateMemberSettersAnnounceMents()
{
    QString res = SETTER_KEY;

    for(NewMember*& mem : this->privateMembers)
    {
        res += TAB + mem->generateMemberInitializer_SetterAnnounce() + SLASH;
    }

    return res;
}

//  *******************************************************************
//  generate templates are here:
//  bool set${memberName.Upper}(${type} ${memberName});
//  etc...
//  *******************************************************************
QString NewClass::generateMemberGettersAnnounceMents()
{
    QString res = GETTER_KEY;

    for(NewMember*& mem : this->privateMembers)
    {
        res += TAB + mem->generateMemberInitializer_GetterAnnounce() + SLASH;
    }

    return res;
}

//  *******************************************************************
//  generate templates are here:
//  bool set${memberName.Upper}(${type} ${memberName})
//  {
//      this->memberName = memberName;
//  }
//  etc
//  *******************************************************************
QString NewClass::generateMemberSettersDefinition()
{
    QString res;
    for(NewMember*& mem : this->privateMembers)
    {
        // comments
        res +=  CommentGenerator::generateSetterFunction(*mem, "") + SLASH;
        // definitions
        res +=  mem->generateMemberInitializer_SetterDefine();
        // gap
        res +=  FUNC_SPACE;
    }

    return res;
}

//  *******************************************************************
//  generate templates are here:
//  ${type} get${memberName.Upper}(${type} ${memberName})
//  {
//      return this->memberName;
//  }
//  etc
//  *******************************************************************
QString NewClass::generateMemberGettersDefinition()
{
    QString res;
    for(NewMember*& mem : this->privateMembers)
    {
        // comments
        res +=  CommentGenerator::generateGetterFunction(*mem, "")+ SLASH;
        // definitions
        res +=  mem->generateMemberInitializer_GetterDefine();
        // gap
        res +=  FUNC_SPACE;
    }

    return res;
}

//  *******************************************************************
//  generate templates are here:
//  static ${type} ${memberName}
//  *******************************************************************
QString NewClass::generateStaticMembers()
{
    QString res;
    if(this->staticMembers.empty())
        return "";

    for(NewMember*& mem : this->staticMembers)
    {
        res += TAB + STATIC + mem->generateMemberAnnounceMent() + CommentGenerator::generateCommentSymbol() \
               + mem->getComment() + SENTENCE_END + SLASH;
    }
    return res;
}

//  *******************************************************************
//  generate templates are here:
//  static ${rettype} ${funcName}(params);
//  *******************************************************************
QString NewClass::generateStaticFunction()
{
    QString res;
    if(this->staticFunctions.empty())
        return "";

    for(NewFunction*& func : this->staticFunctions)
    {
        res += TAB + STATIC + func->getFunctionAnnounceMent() + SLASH;
    }

    return res;
}

//  *******************************************************************
//  generate templates are here:
//  // statics:
//      {members};
//      {functions};
//  *******************************************************************
QString NewClass::generateStaticArea()
{
    return STATIC_KEY + generateStaticMembers() + generateStaticMembers();
}

//  *******************************************************************
//  generate templates are here:
//  ${type} ${memberName}
//  *******************************************************************
QString NewClass::generatePublicMembers()
{
    QString res = CommentGenerator::generatePublicMemberLine();
    if(this->publicMembers.empty())
        return "";

    for(NewMember*& mem : this->publicMembers)
    {
        res += TAB + mem->generateMemberAnnounceMent() + CommentGenerator::generateCommentSymbol() \
               + mem->getComment() + SLASH;
    }
    return res;
}
QString NewClass::generatePrivateMembers()
{
    QString res = CommentGenerator::generatePrivateMemberLine();
    if(this->privateMembers.empty())
        return "";

    for(NewMember*& mem : this->privateMembers)
    {
        res += TAB + mem->generateMemberAnnounceMent() + CommentGenerator::generateCommentSymbol() \
               + mem->getComment() + SLASH;
    }
    return res;
}

//  *******************************************************************
//  generate templates are here:
//  ${rettype} ${funcName}(params);
//  *******************************************************************
QString NewClass::generatePublicFunction()
{
    QString res = CommentGenerator::generatePublicFunctionLine();
    if(this->publicFunctions.empty())
        return "";

    for(NewFunction*& func : this->publicFunctions)
    {
        res += TAB + func->getFunctionAnnounceMent() + SLASH;
    }

    return res;
}
QString NewClass::generatePrivateFunction()
{
    QString res = CommentGenerator::generatePrivateFunctionLine();
    if(this->privateFunctions.empty())
        return "";

    for(NewFunction*& func : this->privateFunctions)
    {
        res += TAB + func->getFunctionAnnounceMent() + SLASH;
    }

    return res;
}

//  *******************************************************************
//  generate templates are here:
//  private:
//      {members};
//      {functions};
//  *******************************************************************
QString NewClass::generatePrivateArea()
{
    return PRIVATE_KEY + generatePrivateMembers() + generatePrivateFunction();
}

//  *******************************************************************
//  generate templates are here:
//  //  statics:
//      {members};
//      {functions};
//  //  setters and getters
//      {functions};
//  *******************************************************************
QString NewClass::generateMustPublicArea()
{
    return PUBLIC_KEY + \
           generateCopyConstructorAnnouncement() + \
           generateStaticArea() + \
           generateMemberSettersAnnounceMents()+ \
           generateMemberGettersAnnounceMents();
}

//  *******************************************************************
//  generate templates are here:
//  public:
//      {members};
//      {functions};
//  *******************************************************************
QString NewClass::generateOtherPublicArea()
{
    return generatePublicMembers() + generatePublicFunction();
}

QString NewClass::generatePublicArea()
{
    return generateMustPublicArea() + generateOtherPublicArea();
}


//  *******************************************************************
//  generate templates are here:
//  protected:
//      {functions};
//      etc
//  *******************************************************************
QString NewClass::generateProtectedFunctionArea()
{
    QString res = PROTECT_KEY;

    for(NewFunction*& f : this->protectedFunctions)
        res += f->getFunctionAnnounceMent() + SLASH;

    return res;
}

// *******************************************************************
//  generate templates are here:
//  #ifndef _${className.upper}_H_
//  #define _${className.upper}_H_
//
//
//
//  class　{className} {
//  // initers

//  public:
//  //  statics:
//      {members};
//      {functions};
//  //  setters and getters
//      {functions};
//  //  others
//      {members};
//      {functions};
//  protected:
//      {members};
//      {functions};
//  private:
//      {members};
//      {functions};
//  }
//  #endif
// *******************************************************************
QString NewClass::generateHeader()
{
    return
        generateAvoidMultiDef() + \
        CommentGenerator::generateClass(*this, "")   + \
        CLASS_KEY(this->ClassName) + CLASS_DEFINE_BEGIN + \
        generatePublicArea() + \
        generateProtectedFunctionArea() +
        generatePrivateArea() + \
        CLASS_DEFINE_END + AVOID_MULTI_END;
}
// *******************************************************************
//  generate templates are here:
//  #include<${className}.h>
//
//  // Comments
//  // functions
//
//
//  // followings are:
//  //      [initilizer]
//  //      [getter] ${type} get${memberName}()
//  //      [setter] bool    set${memberName}(${type} ${memberName})
//  //      other functions
// *******************************************************************
QString NewClass::generateSource()
{
    QString sources;

    sources += INCLUDE_NECCESS(this->ClassName) + INCLUDE_WHITE;

    // initializer
    sources += generateCopyConstructorDefinition();

    // setter && getter
    sources += generateMemberSettersDefinition();
    sources += generateMemberGettersDefinition();// getter and setter

    for(NewFunction*& f : staticFunctions)
    {
        sources += CommentGenerator::generateFunction(*f);
        sources += f->getFunctionDefinition() + SLASH;
    }

    for(NewFunction*& f : privateFunctions)
    {
        sources += CommentGenerator::generateFunction(*f);
        sources += f->getFunctionDefinition() + SLASH;
    }

    for(NewFunction*& f : publicFunctions)
    {
        sources += CommentGenerator::generateFunction(*f);
        sources += f->getFunctionDefinition() + SLASH;
    }

    return sources;
}


QList<NewMember*>&   NewClass::getClassMember(MEMBER_TYPE type)
{
    switch(type)
    {
    case MEMBER_TYPE::M_STATIC:
        return this->staticMembers;
    case MEMBER_TYPE::PRIVATE:
        return this->privateMembers;
    case MEMBER_TYPE::PUBLIC:
        return this->publicMembers;
    }
    return this->publicMembers;
}


QList<NewFunction*>& NewClass::getClassFunc(FUNC_TYPE type)
{
    switch(type)
    {

    case FUNC_TYPE::PROTECTED:
        return this->protectedFunctions;
    case FUNC_TYPE::PRIVATE:
        return this->privateFunctions;
    case FUNC_TYPE::PUBLIC:
        return this->publicFunctions;
    }
    return this->publicFunctions;
}
