#include "commentgenerator.h"
#define SPACE                               " "
#define SLASH                               "\n"
#define COMMENT                             QString("//  ")
#define TAB                                 "   "
#define COMMENT_TAB                         COMMENT + TAB
#define AREA_WORKING_SYMBOL                 "::"
#define DEF_SEP_LINE                        QString("//  *******************************************************************") + SLASH
#define OPTIONAL_START                      QString("[")
#define OPTIONAL_END                        "]"
#define OPTION(state, what)                 QString(((state)?(OPTIONAL_START + SPACE + what + SPACE + OPTIONAL_END):""))

#define PUBLIC_MEMBER_LINE                  COMMENT + "Public Member:> " + SLASH
#define PUBLIC_FUNCTION_LINE                COMMENT + "Public function:> " + SLASH
#define PRIVATE_MEMBER_LINE                 COMMENT + "Private Member:> " + SLASH
#define PRIVATE_FUNCTION_LINE               COMMENT + "Private function:> " + SLASH

// class
#define CLASS_NAME(name)                    COMMENT + "Class Name:> " + name + SLASH
#define CLASS_COMMENT(comment)              COMMENT + "Usage and comments:> " + comment + SLASH
// members
#define CLASS_MEMBER_COMMENT(comment)       COMMENT + comment

// functions
#define ADD_FUNC_Modifier(isStatic, isVirtual, isExplicit) \
                                            OPTION(isStatic, "static") + \
                                            OPTION(isVirtual, "virtual") + \
                                            OPTION(isExplicit, "explicit")
#define FUNC_Modifier(isStatic, isVirtual, isExplicit) \
                                            ((!(isStatic) && !(isVirtual) && !(isExplicit)) ? \
                                            "" : COMMENT + ADD_FUNC_Modifier(isStatic, isVirtual, isExplicit))

#define BELONG_CLASS(Class)                 COMMENT + "belonging class:> " + Class + SPACE
#define FUNCTION_NAME_PREFIX                COMMENT + "function name:> "
#define FUNCTION_NAME(name)                 FUNCTION_NAME_PREFIX + name
#define PARAM_PREFIX                        COMMENT + "function params:>" + SLASH
#define PARAM_IS_CONST(isConst)             (isConst?"const " : "")
#define PARAM_MEMBER_TYPE(type)             type + SPACE
#define PARAM_MEMBER_NAME(name)             name + SPACE
#define PARAM_COMMENT(type, name, comment)  COMMENT + PARAM_MEMBER_TYPE(type) + PARAM_MEMBER_NAME(name) + comment
#define RET_TYPE_PREFIX                     COMMENT + "return type:>" + SLASH
#define RET_TYPE(type, comment)             RET_TYPE_PREFIX + COMMENT_TAB + (type) + SPACE + ":" + comment
#define FUNC_SELF_COMMENT_PREFIX            COMMENT + "usage and more related:> "
#define FUNC_SELF_COMMENT(comment)          FUNC_SELF_COMMENT_PREFIX + comment

#define SETTER_FUNC_RET_TYPE                QString("bool")
#define SETTER_FUNC_PREFIX                  "set"

// bonus comments
#define BONUS_COMMENT(bonus)                COMMENT + bonus

QString CommentGenerator::generateCommentSymbol()
{
    return COMMENT;
}

QString CommentGenerator::generatePublicFunctionLine()
{
    return PUBLIC_FUNCTION_LINE;
}
QString CommentGenerator::generatePrivateFunctionLine()
{
    return PRIVATE_FUNCTION_LINE;
}
QString CommentGenerator::generatePublicMemberLine()
{
    return PUBLIC_MEMBER_LINE;
}
QString CommentGenerator::generatePrivateMemberLine()
{
    return PRIVATE_MEMBER_LINE;
}

QString CommentGenerator::indicateFunctionType(NewFunction& f)
{
    if(f.getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PRIVATE)
        return "//  this is a priavte function";
    else if(f.getFunctionProperty() == NewFunction::FUNC_TYPE::FOR_PROTECTED)
        return "//  this is a protected function";
    else
        return "//  this is a public function";
}

//  *******************************************************************
//  generate templates are here:
//  //  *******************************************************************
//  //  Class Name:>
//  //  Usage and comments:>
//  //  *******************************************************************
//  *******************************************************************
QString CommentGenerator::generateClass(NewClass& Class, QString others)
{
    return
        DEF_SEP_LINE + \
           CLASS_NAME(Class.getName()) + \
        CLASS_COMMENT(Class.getComment()) + \
        BONUS_COMMENT(others) + SLASH + \
        DEF_SEP_LINE;
}

//  *******************************************************************
//  generate templates are here:
//  // [static] className::member ${comments}
//  *******************************************************************
QString CommentGenerator::generateMember(NewMember& member, QString others)
{
    return
        OPTION(member.getIsStatic(), "static") + SPACE + \
        member.getClass() + AREA_WORKING_SYMBOL+member.getName() + \
        CLASS_MEMBER_COMMENT(others);
}



//  *******************************************************************
//  generate templates are here: [used in source file]
//  //  function params:>
//  //      ${type} ${name} : ${comments}
//  //      etcs...
//  *******************************************************************
QString CommentGenerator::generateFuncParams(NewFunction& func)
{
    QString paramComment;
    QList<NewFunction::Arg*> args = func.getFunctionArgList();

    for(NewFunction::Arg*& arg : args)
    {
        paramComment += COMMENT_TAB + PARAM_IS_CONST(arg->isConst) + PARAM_MEMBER_TYPE(arg->type) + PARAM_MEMBER_NAME(arg->name) + \
                        ":" + arg->argComm + SLASH;
    }

    return  PARAM_PREFIX + paramComment;
}

//  *******************************************************************
//  generate templates are here: [used in source file]
//  //  *******************************************************************
//  //  [static] [virtual] [explicit]
//  //  belonging class:>   ${className}
//  //  function name:>     ${functionName}
//  //  function params:>
//  //      ${type} ${name} : ${comments}
//  //      etcs...
//  //  return type:>
//  //      ${ret_type} : ${comments}
//  //  usage and more related:> ${comments}
//  //  *******************************************************************
//  *******************************************************************
QString CommentGenerator::generateFunction(NewFunction& func)
{
    return
            DEF_SEP_LINE + \
            FUNC_Modifier(func.getIsStatic(), func.getIsVirtual(), func.getIsExplicit()) + SLASH + \
            BELONG_CLASS(func.getFunctionBelongClass()) + SLASH +\
           FUNCTION_NAME(func.getName()) + SLASH +\
            CommentGenerator::generateFuncParams(func) + \
            RET_TYPE(func.getFunctionRetType(), "") + SLASH +\
           FUNC_SELF_COMMENT(func.getFunctionComment()) + SLASH +\
            DEF_SEP_LINE;
}

//  *******************************************************************
//  generate templates are here: [used in source file]
//  // init memberName, comment
//  *******************************************************************
QString CommentGenerator::generateSetterFunction(NewMember& member, QString others)
{
    return COMMENT + "init " + member.getName() + ", " + others;
}

//  *******************************************************************
//  generate templates are here: [used in source file]
//  // get memberName, comment
//  *******************************************************************
QString CommentGenerator::generateGetterFunction(NewMember& member, QString others)
{
    return "get " + member.getName() + ", " + others;
}
