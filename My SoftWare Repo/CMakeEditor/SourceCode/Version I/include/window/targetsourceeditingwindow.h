#ifndef TARGETSOURCEEDITINGWINDOW_H
#define TARGETSOURCEEDITINGWINDOW_H

#include <QMainWindow>
namespace Ui {
class TargetSourceEditingWindow;
}

class QTableWidgetItem;

class TargetSourceEditingWindow : public QMainWindow
{
    Q_OBJECT

public:
    const char* enableFile = "C++文件(*.cpp);;C文件(*.c);;头文件(*.h);;模板文件(*.hpp)";
    explicit    TargetSourceEditingWindow(QWidget *parent = nullptr);
    void        bindBuildSrc(QStringList* srcList);
    QStringList selectByFileDialog();
    void        configSource(QStringList l, bool need_refresh);
    ~TargetSourceEditingWindow();
    void        setFocusItems(QTableWidgetItem* p){currentFocus = p;}

    void        closeEvent(QCloseEvent*);

private slots:
    void on_show_config_itemClicked(QTableWidgetItem *item);

    void on_btn_append_src_clicked();

    void on_btn_remove_src_clicked();

    void on_btn_resetSources_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::TargetSourceEditingWindow*      ui;
    QStringList*                        shownLists = nullptr;
    QList<QTableWidgetItem*>            tableItems;
    QTableWidgetItem*                   currentFocus = nullptr;
    void                                initConnection();
    void                                updateUi();
    void                                scanAndGenerateItems();
    void                                clearItems();
    void                                clearListsSource(){if(shownLists)shownLists->clear();}
    void                                removeSingleSource(QString what){if(shownLists) shownLists->removeOne(what);}
};

#endif // TARGETSOURCEEDITINGWINDOW_H
