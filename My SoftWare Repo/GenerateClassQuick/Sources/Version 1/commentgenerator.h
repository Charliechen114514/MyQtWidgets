#ifndef COMMENTGENERATOR_H
#define COMMENTGENERATOR_H
#include "newclass.h"
class CommentGenerator
{
public:
    CommentGenerator() = default;
    const CommentGenerator& operator=(const CommentGenerator&) = delete;
    CommentGenerator(const CommentGenerator&) = delete;

    static QString generateCommentSymbol();
    static QString generatePublicFunctionLine();
    static QString generatePrivateFunctionLine();
    static QString generatePublicMemberLine();
    static QString generatePrivateMemberLine();
    static QString generateMember(NewMember& member, QString others);
    static QString generateFuncParams(NewFunction& func);
    static QString generateFunction(NewFunction& func);
    static QString generateClass(NewClass& Class, QString others);

    static QString indicateFunctionType(NewFunction& f);

    static QString generateSetterFunction(NewMember& member, QString others);
    static QString generateGetterFunction(NewMember& member, QString others);
};

#endif // COMMENTGENERATOR_H
