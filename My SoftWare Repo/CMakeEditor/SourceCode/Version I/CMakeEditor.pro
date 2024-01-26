QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    source/core/classlistandwidgetitemmanager.cpp \
    source/core/cmakeprojectcoreinfo.cpp \
    source/core/cmakeselfinfo.cpp \
    source/core/cmakesentencegenerator.cpp \
    source/core/main.cpp \
    source/core/qssmanager.cpp \
    source/window/CMakeUtilsMainWindowBase.cpp \
    source/window/cmakeconfigmainwindow.cpp \
    source/window/cmaketargetconfigmainwindow.cpp \
    source/window/cmakeversionmainwindow.cpp \
    source/window/mainwindow.cpp \
    source/window/targetsourceeditingwindow.cpp


HEADERS += \
    include/core/CMakeUtils.h \
    include/core/classlistandwidgetitemmanager.h \
    include/core/cmakeprojectcoreinfo.h \
    include/core/cmakeselfinfo.h \
    include/core/cmakesentencegenerator.h \
    include/core/qssmanager.h \
    include/window/CMakeUtilsMainWindowBase.h \
    include/window/CMakeWindowUtils.h \
    include/window/cmakeconfigmainwindow.h \
    include/window/cmaketargetconfigmainwindow.h \
    include/window/cmakeversionmainwindow.h \
    include/window/mainwindow.h \
    include/window/targetsourceeditingwindow.h


FORMS += \
    cmakeconfigmainwindow.ui \
    cmaketargetconfigmainwindow.ui \
    cmakeversionmainwindow.ui \
    mainwindow.ui \
    targetsourceeditingwindow.ui

INCLUDEPATH += ./include/window/ \
               ./include/core/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Direct/dark/darkstyle.qrc \
    Direct/light/lightstyle.qrc \
    QSS_I.qrc
