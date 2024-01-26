#include "CMakeUtilsMainWindowBase.h"
#include "cmakeconfigmainwindow.h"
#include "cmakeversionmainwindow.h"
#include "cmaketargetconfigmainwindow.h"


CMakeUtilsMainWindowBase* CMakeUtilsMainWindowProducer::makeWindow(QWidget* parent, Window_TYPE type)
{
    switch (type) {
    case Window_TYPE::CMAKE_SELF_CONFIG_WINDOW:
        return new CMakeVersionMainWindow(parent);
    case Window_TYPE::CMAKE_PROJECT_CONFIG_SELF_WINDOW:
        return new CMakeConfigMainWindow(parent);
    case Window_TYPE::CMAKE_CONFIG_TARGET_WINDOW:
        return new CMakeTargetConfigMainWindow(parent);
    }
}
