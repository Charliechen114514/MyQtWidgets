#ifndef EDITTARGETMEMBER_H
#define EDITTARGETMEMBER_H

#include "newmemberwindow.h"

namespace Ui {
    class EditTargetMember;
}

class EditTargetMember : public NewMemberWindow
{
    Q_OBJECT

public:
    EditTargetMember() = delete;
    const EditTargetMember& operator=(const EditTargetMember&) = delete;
    EditTargetMember(const EditTargetMember&) = delete;
    explicit EditTargetMember(NewMember* edited, QString className, QWidget *parent = nullptr);

    void initBasic();
    void parseMember();
signals:
    void successEditMember();


private:
    Ui::EditTargetMember* ui;
    NewMember* edited = nullptr;
};

#endif // EDITTARGETMEMBER_H
