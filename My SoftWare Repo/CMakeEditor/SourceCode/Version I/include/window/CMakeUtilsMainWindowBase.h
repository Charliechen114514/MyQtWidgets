#ifndef CMAKEUTILSMAINWINDOWBASE_H
#define CMAKEUTILSMAINWINDOWBASE_H

class CMakeUtilsMainWindowBase{};
class QWidget;
class CMakeUtilsMainWindowProducer
{
public:

    enum class Window_TYPE{
        CMAKE_SELF_CONFIG_WINDOW,
        CMAKE_PROJECT_CONFIG_SELF_WINDOW,
        CMAKE_CONFIG_TARGET_WINDOW,
    };

    static CMakeUtilsMainWindowBase* makeWindow(QWidget* parent, Window_TYPE offset);
    virtual ~CMakeUtilsMainWindowProducer(){}
};


#endif // CMAKEUTILSMAINWINDOWBASE_H
