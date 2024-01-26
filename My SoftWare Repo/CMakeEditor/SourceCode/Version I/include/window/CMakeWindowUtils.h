#ifndef CMAKEWINDOWUTILS_H
#define CMAKEWINDOWUTILS_H
#include <QMessageBox>
// configs

#define AutoDelete          this->setAttribute(Qt::WA_DeleteOnClose)
#define SetName(name)       this->setWindowTitle(name)

//  Window name
//  CMake自身配置设置主窗口




// SaveMonitor

class SaveMonitor
{
public:
    const char* QUERY_SAVE              = "所有的操作都会保存，你确定你所作的更改吗?";
    const char* QUERY_TITLE_SAVE        = "确定你所作的更改";
    SaveMonitor() = default;
    const SaveMonitor& operator=(const SaveMonitor&) = delete;
    SaveMonitor(const SaveMonitor&) = delete;
    void setShellSaveQuery(){this->isSave = false;};

    bool queryForLeave(QWidget* w = nullptr){
        if(!isSave) // 用户
        {
            if(QMessageBox::Yes == QMessageBox::question(w, QUERY_TITLE_SAVE, QUERY_SAVE))
            {
                return true;
            }
            else
                return false;
        }
        return true; // no modify
    }

private:
    bool isSave = true;
};

#include <QStandardPaths>
class UsablePath
{
public:
    static QString getDesktopPath(){
        return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }
};


#endif // CMAKEWINDOWUTILS_H
