#include "newmember.h"
#include "utils.h"
#define UNINIT_CHECK_UNALLOWED \
do{\
    if(this->memberName.isEmpty() || this->belongClass.isEmpty() || this->type.isEmpty())\
    {\
        state = false;\
        return "";\
    }\
}while(0)

#define STATE_FALSE_QUIT(state, failed_ret_back) if(!(state)) return (failed_ret_back)
#define CONST               QString("const ")
#define ISSTATIC_STR        QString((this->isStatic ? "static ":""))
#define ISCONST_STR         QString((this->isConst ? "const " : ""))
#define TAB                 "   "
#define AREA_WORKING_SYMBOL "::"
#define SPACE               " "
#define SLASH               "\n"
#define SENTENCE_END        ";"
#define SETTER_STATE_TYPE   QString("bool")
#define SETTER_PREFIX       "set"
#define DEF_RET_TRUE_SETTER TAB + "return true;"
#define GETTER_PREFIX       "get"
#define PARAMLIST_START     "("
#define PARAMLIST_END       ")"
#define FUNC_START          "{"
#define FUNC_END            "}"

#define FUNC_BODY_EMPTY     SLASH + FUNC_START + SLASH +SLASH + FUNC_END
#define FUNC_BODY_STH(str)  SLASH + FUNC_START + SLASH + (str) +SLASH + FUNC_END
#define DEF_INIT(name)      TAB + QString("this->") + name + "=" + name + SENTENCE_END + SLASH

void NewMember::copyConfig(NewMember* m)
{
    this->setMemberName(m->getName());
    this->setMemberType(m->getType());
    this->setMemberPropertyType(m->getMemberPropertyType());
    this->setComment(m->getComment());
    this->setConst(m->getIsConst());
    this->setStatic(m->getIsStatic());
}


QString NewMember::generateMemberPropertyString()
{
    switch (this->memberType) {
    case NewMember::MEMBER_TYPE::FOR_FUNC:
        return QString("function params");
    case MEMBER_TYPE::FOR_PRIVATE:
        return QString("private");
    case MEMBER_TYPE::FOR_PUBLIC:
        return QString("public");
    case MEMBER_TYPE::TYPE_INDEX_MAX:
        return QString("");
    }
    return QString("");
}

// *******************************************************************
//  generate templates are here:
//  ${type} ${memberName};
// *******************************************************************

QString NewMember::generateMemberAnnounceMent()
{
    bool state[[maybe_unused]];
    UNINIT_CHECK_UNALLOWED;
    qDebug()<< ISSTATIC_STR + ISCONST_STR +this->type + SPACE + this->memberName + SENTENCE_END;
    return ISSTATIC_STR + ISCONST_STR +this->type + SPACE + this->memberName + SENTENCE_END;
}

// *******************************************************************
//  generate templates are here:
//  bool    set${memberName}(const ${type} ${memberName})
// *******************************************************************
QString NewMember::generateMemberInitializer_SetterAnnounce()
{
    bool state[[maybe_unused]];
    UNINIT_CHECK_UNALLOWED;

    return  SETTER_STATE_TYPE + SPACE + SETTER_PREFIX + upperFirst(this->memberName) + \
           PARAMLIST_START + CONST + this->type + SPACE + this->memberName + PARAMLIST_END + SENTENCE_END;
}

// *******************************************************************
//  generate templates are here:
//  bool    set${memberName}(const ${type} ${memberName})
// *******************************************************************
QString NewMember::generateMemberInitializer_SetterDefine()
{
    bool state[[maybe_unused]];
    UNINIT_CHECK_UNALLOWED;

    return  SETTER_STATE_TYPE + SPACE + this->belongClass + AREA_WORKING_SYMBOL + SETTER_PREFIX + upperFirst(this->memberName) + \
           PARAMLIST_START + CONST + this->type + SPACE + this->memberName + PARAMLIST_END + \
           FUNC_BODY_STH
           (
                DEF_INIT(this->memberName) + \
                DEF_RET_TRUE_SETTER
           );
}


// *******************************************************************
//  generate templates are here:
//  ${type} get${memberName}()
// *******************************************************************
QString NewMember::generateMemberInitializer_GetterAnnounce()
{
    bool state[[maybe_unused]];
    UNINIT_CHECK_UNALLOWED;

    return  this->type + SPACE + GETTER_PREFIX + upperFirst(this->memberName) + \
           PARAMLIST_START + PARAMLIST_END + SENTENCE_END;
}


// *******************************************************************
//  generate templates are here:
//  ${type} get${memberName}()
//  {
//      return this->${memberName};
//  }
// *******************************************************************
QString NewMember::generateMemberInitializer_GetterDefine()
{
    bool state[[maybe_unused]];
    UNINIT_CHECK_UNALLOWED;

    return this->type + SPACE + this->belongClass + AREA_WORKING_SYMBOL + GETTER_PREFIX + upperFirst(this->memberName) + PARAMLIST_START + PARAMLIST_END + \
           FUNC_BODY_STH
           (
               "return this->" + this->memberName + SENTENCE_END
           );
}


// get all infos
QStringList NewMember::generateAll()
{
    QStringList res;
    bool state = true;
    res << generateMemberAnnounceMent();
    res << generateMemberInitializer_SetterAnnounce();
    res << generateMemberInitializer_SetterDefine();
    res << generateMemberInitializer_GetterAnnounce();
    res << generateMemberInitializer_GetterDefine();

    if(res.size() != static_cast<int>(NewMember::GenerateInfoIndex::GENERATE_MAX))
        STATE_FALSE_QUIT(state, QStringList());

    return res;
}

QString NewMember::getGeneratePreview()
{
    if(this->memberType == MEMBER_TYPE::FOR_FUNC)
    {
            return "// " +
            this->getComment() + "\n" +
            "this is a function param:> \n" +
               this->generateMemberAnnounceMent();
    }
    else
    {
            return "// " +
            this->getComment() +
            "this is a " + this->generateMemberPropertyString() + " member" + "\n" +
                   this->generateMemberAnnounceMent();
    }
}
