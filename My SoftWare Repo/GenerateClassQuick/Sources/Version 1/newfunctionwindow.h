#ifndef NEWFUNCTIONWINDOW_H
#define NEWFUNCTIONWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "newmemberwindow.h"
#include "newfunction.h"
#include "newmember.h"

namespace Ui {
class NewFunctionWindow;
}

class NewFunctionWindow : public QMainWindow
{
    Q_OBJECT

public:
    NewFunctionWindow() = delete;
    explicit NewFunctionWindow(QString belongClass, QWidget *parent = nullptr);
    ~NewFunctionWindow();

signals:
    void signal_success_build_Function(NewFunction* newFunc);

private slots:
    void receive_setMember(NewMember* member);

    void on_func_name_lineEdit_returnPressed();

    void on_btn_preview_clicked();

    void closeEvent(QCloseEvent*);

    void on_btn_addExplicit_clicked();

    void on_btn_addVirtual_clicked();

    void on_btn_set_static_clicked();

    void on_btn_add_const_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_btn_varProperty_clicked();

    void on_btn_addParam_clicked();

    void on_func_ret_combo_box_currentIndexChanged(int index);

    void on_func_ret_type_line_edit_textEdited(const QString &arg1);

    void on_func_name_lineEdit_textEdited(const QString &arg1);

    void on_comment_for_func_textChanged();

private:
    Ui::NewFunctionWindow   *ui;
    NewMemberWindow*        paramWindow     = nullptr;
    NewFunction*            newFunction     = nullptr;
    bool                    is_construct    = false;
    QString                 belongClass;
    QString                 funcSelfComments;
    QString                 ret_type;
    QStringList             nameLists;
    bool                    isStatic        = false;
    bool                    isExplicit      = false;
    bool                    isVirtual       = false;
    bool                    isConst         = false;
    int                     clickTimes      = 0;
    void                    enableAll();
    void                    disableAll();
    void                    switchTheNewFunction(QString name, QString ret_type);
    void                    switchForTheNewType(QString newType);
    void                    collectInfo();
    bool                    initConnectionWithparamWindow();
    void                    doPreview();
};

#endif // NEWFUNCTIONWINDOW_H
