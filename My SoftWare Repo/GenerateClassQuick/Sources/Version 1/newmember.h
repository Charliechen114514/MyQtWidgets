#ifndef NEWMEMBER_H
#define NEWMEMBER_H
#include <QStringList>
class NewMember
{
public:
    enum class MEMBER_TYPE{
        FOR_FUNC,
        FOR_PRIVATE,
        FOR_PUBLIC,
        TYPE_INDEX_MAX
    };

    enum class GenerateInfoIndex{
        GENERATE_ANNOUNCEMENT,
        GENERATE_INITIALIZER_SETTER_ANNOUNCE,
        GENERATE_INITIALIZER_SETTER_DEFINE,
        GENERATE_INITIALIZER_GETTER_ANNOUNCE,
        GENERATE_INITIALIZER_GETTER_DEFINE,
        GENERATE_MAX
    };

    NewMember() = delete;
    const NewMember& operator=(const NewMember&){return *this;}
    NewMember(const NewMember&) = default;
    NewMember(QString belongClass, QString name, QString type):belongClass(belongClass),memberName(name),type(type){}

    void    copyConfig(NewMember* other);
    void    setMemberBelongClass(QString Class)     {this->belongClass = Class;}
    void    setMemberName(QString name)             {this->memberName = name;}
    void    setMemberType(QString type)             {this->type = type;}
    void    setComment(QString comments)            {this->relatedComment = comments;}
    void    setStatic(bool state)                   {isStatic = state;}
    void    setConst(bool state)                    {isConst = state;}
    void    setMemberPropertyType(MEMBER_TYPE type) {this->memberType = type;}
    MEMBER_TYPE getMemberPropertyType()             {return this->memberType;}
    QString getName()                               {return this->memberName;}
    QString getType()                               {return this->type;}
    QString getClass()                              {return this->belongClass;}
    QString getComment()                            {return this->relatedComment;}
    bool    getIsStatic()                           {return this->isStatic;}
    bool    getIsConst()                            {return this->isConst;}
    // functional
    // this function do generate announcement
    bool    isSame(const NewMember& name)   {return this->memberName == name.memberName;}
    friend bool operator==(const NewMember& name, const QString& name2)
                                            {return name.memberName == name2;}
    QString generateMemberPropertyString();
    QString generateMemberAnnounceMent();
    // this function do generate initializer
    QString generateMemberInitializer_SetterAnnounce();
    QString generateMemberInitializer_SetterDefine();
    QString generateMemberInitializer_GetterAnnounce();
    QString generateMemberInitializer_GetterDefine();
    QString getGeneratePreview();
    // generateAll
    QStringList generateAll();

private:
    QString         belongClass;
    QString         memberName;
    QString         type;
    QString         relatedComment;
    MEMBER_TYPE     memberType;
    bool            isStatic = false;
    bool            isConst = false;
};

#endif // NEWMEMBER_H
