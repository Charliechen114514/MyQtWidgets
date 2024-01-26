#ifndef CMAKEUTILS_H
#define CMAKEUTILS_H

#define SCOPE_BEGIN         "("
#define SCOPE_END           ")"
#define TAB                 "   "
#define SPACE               " "
#define SLASH               "\n"


#define GET_VALUE_PREFIX    QString("${")
#define GET_VALUE_SUFFIX    "}"
#define GET_VALUE_STR(val)  GET_VALUE_PREFIX + val + GET_VALUE_SUFFIX

#define IF(statement)       "if(" + statement + ")"
#define ELSE                "else()"
#define ENDIF               "endif()"


#endif // CMAKEUTILS_H
