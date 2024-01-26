QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    classlistandwidgetitemmanager.cpp \
    commentgenerator.cpp \
    editclasswindow.cpp \
    editfunctionwindow.cpp \
    editmemberwindow.cpp \
    exportmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowclassesmanager.cpp \
    menuactionconnector.cpp \
    newclass.cpp \
    newclasswindow.cpp \
    newfunction.cpp \
    newfunctionwindow.cpp \
    newmember.cpp \
    newmemberwindow.cpp \
    qssmanager.cpp \
    savepathgenerator.cpp \
    savingconfigwindow.cpp \
    utils.cpp

HEADERS += \
    CCSTDC_Assert.h \
    classlistandwidgetitemmanager.h \
    commentgenerator.h \
    editclasswindow.h \
    editfunctionwindow.h \
    editmemberwindow.h \
    exportmanager.h \
    mainwindow.h \
    mainwindowclassesmanager.h \
    menuactionconnector.h \
    newclass.h \
    newclasswindow.h \
    newfunction.h \
    newfunctionwindow.h \
    newmember.h \
    newmemberwindow.h \
    qssmanager.h \
    savepathgenerator.h \
    savingconfigwindow.h \
    ui_editTargetWindow.h \
    utils.h

FORMS += \
    editclasswindow.ui \
    editfunctionwindow.ui \
    editmemberwindow.ui \
    mainwindow.ui \
    newclasswindow.ui \
    newfunctionwindow.ui \
    newmemberwindow.ui \
    savingconfigwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    lightstyle.qrc \
    rc2/darkstyle.qrc

DISTFILES += \
    lightstyle.qss \
    rc2/darkstyle.qss
