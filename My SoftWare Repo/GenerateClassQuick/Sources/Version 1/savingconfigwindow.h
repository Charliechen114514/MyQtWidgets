#ifndef SAVINGCONFIGWINDOW_H
#define SAVINGCONFIGWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
class QTreeWidgetItem;
namespace Ui {
class SavingConfigWindow;
}

class SavingConfigWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum class SAVE_OPTION{
        ALL,
        SRIN,
        PREVIEW,
        CANCLE
    };
    SAVE_OPTION getSaveOption(){return this->saveOption;}
    void     setAsPreView(){this->saveOption = SAVE_OPTION::PREVIEW;}
    explicit SavingConfigWindow(QWidget *parent = nullptr);
    QString  getConfigPathStringAll(){return this->allPath;}
    QString  getHeaderPath(){return this->headerPath;}
    QString  getSourcePath(){return this->sourcePath;}
    void     makePreview(QMap<QString, QStringList>* map);
    void     disAbleAll();
    void     moveToDefault();
    ~SavingConfigWindow();

signals:
    void finishEditSavingConfig();

private slots:

    void closeEvent(QCloseEvent*);

    void on_btn_setSRIN_clicked();

    void on_btn_setAll_clicked();

    void on_btn_setHeader_clicked();

    void on_btn_setSource_clicked();

    void on_btn_setAllPath_clicked();

private:
    Ui::SavingConfigWindow  *ui;
    SAVE_OPTION             saveOption = SAVE_OPTION::SRIN;
    QList<QTreeWidgetItem*> items;
    // someRelevant sources
    QString                 headerPath;
    QString                 sourcePath;
    QString                 allPath;
    void                    clearItems();
};

#endif // SAVINGCONFIGWINDOW_H
