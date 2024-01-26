#ifndef NEWCLASS_H
#define NEWCLASS_H
#include <QString>
#include "newmember.h"
#include "newfunction.h"

class NewClass
{
public:
    // Only enable the class when it appears
    NewClass() = delete;
    const NewClass& operator=(const NewClass&){return *this;}
    NewClass(const NewClass&) = default;
    ~NewClass();
    NewClass(QString ClassName):ClassName(ClassName){}
    void            addMember(NewMember* member);
    void            addFunction(NewFunction* function);

    bool            dismissMember(NewMember* member);
    bool            dismissFunction(NewFunction* function);
    friend  bool    operator==(const NewClass& a1, const NewClass& a2){return a1.ClassName == a2.ClassName;}

    void            setStates(bool def, bool copydef, bool equalcopy)
                    {this->requiredDef = def; this->requiredCopy = copydef; this->requiredEqualCopy = equalcopy;}
    bool            setClassName(QString name){this->ClassName = name;return true;}
    bool            setComment(QString comm){this->classComments = comm;return true;}
    void            setSourceSavePlace(QString p){this->sourceSavePlace = p;}
    void            setHeaderSavePlace(QString p){this->headerSavePlace = p;}

    bool            getDefState(){return this->requiredDef;}
    bool            getCopyState(){return this->requiredCopy;}
    bool            getCopyEqualState(){return this->requiredEqualCopy;}
    void            adjustMembers();
    void            adjustFunctions();

    QString         getComment(){return this->classComments;}
    QString         getName(){return this->ClassName;}
    QString         getSourceSavePlace(){return this->sourceSavePlace;}
    QString         getHeaderSavePlace(){return this->headerSavePlace;}

    QString         generateHeader();
    QString         generateSource();

    enum class MEMBER_TYPE{
        M_STATIC,
        PRIVATE,
        PUBLIC
    };

    enum class FUNC_TYPE{
        PROTECTED,
        PRIVATE,
        PUBLIC,
    };

    QList<NewMember*>&   getClassMember(MEMBER_TYPE type);
    QList<NewFunction*>& getClassFunc(FUNC_TYPE type);

private:
    QString             ClassName;
    QString             classComments;
    QString             sourceSavePlace;
    QString             headerSavePlace;

    QList<NewMember*>   publicMembers;
    QList<NewFunction*> publicFunctions;

    QList<NewFunction*> protectedFunctions;

    QList<NewMember*>   privateMembers;
    QList<NewFunction*> privateFunctions;

    QList<NewMember*>   staticMembers;
    QList<NewFunction*> staticFunctions;


    bool            requiredDef;
    bool            requiredCopy;
    bool            requiredEqualCopy;

    // header
    QString         generateCopyConstructorAnnouncement();
    QString         generateCopyConstructorDefinition();

    QString         generateMemberSettersAnnounceMents();
    QString         generateMemberSettersDefinition();
    QString         generateMemberGettersAnnounceMents();
    QString         generateMemberGettersDefinition();

    // 生成前缀
    QString         generateAvoidMultiDef();
    // 静态函数，静态成员-》StaticArea
    QString         generateStaticMembers();
    QString         generateStaticFunction();
    QString         generateStaticArea();
    // 公共函数，成员，，初始化setter, getter(MustPublic)和其他函数(OtherPublic) -》Public Area
    QString         generatePublicMembers();
    QString         generatePublicFunction();
    QString         generateMustPublicArea();
    QString         generateOtherPublicArea();
    QString         generatePublicArea();
    // protected部分
    QString         generateProtectedFunctionArea();

    // 私有函数，成员 -》Private Area
    QString         generatePrivateMembers();
    QString         generatePrivateFunction();
    QString         generatePrivateArea();

};

#endif // NEWCLASS_H
