#ifndef EDITCLASSWINDOW_H
#define EDITCLASSWINDOW_H

#include <QMainWindow>
#include "newclass.h"
#include <QCloseEvent>
class EditFunctionWindow;
class EditMemberWindow;
namespace Ui {
class EditClassWindow;
}

class EditClassWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditClassWindow(NewClass* _class, QWidget *parent = nullptr);
    EditClassWindow() = delete;
    ~EditClassWindow();
    void changeName(const QString& set){editClass->setClassName(set);}
    void doPreview();
    QString getOldName(){return this->oldName;}
    QString getFinalName(){return this->finalName;}
    bool getNameEdited(){return this->isClassNameEdit;}
signals:
    void successEdit();

private slots:

    void receiveEditClassesFromMemberChange();

    void receiveEditClassedFromFunctionChange();

    void closeEvent(QCloseEvent*);

    void on_lineEdit_textEdited(const QString &arg1);

    void on_btn_adjustMember_clicked();

    void on_btn_adjsutFunction_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::EditClassWindow     *ui;
    NewClass*               editClass = nullptr;
    EditFunctionWindow*     editFunctionWindow = nullptr;
    EditMemberWindow*       editMemberWindow = nullptr;
    bool                    isChange = false;
    QString                 oldName;
    bool                    isClassNameEdit = false;
    QString                 finalName;
};

#endif // EDITCLASSWINDOW_H
