#ifndef CCSTDC_ASSERT_H
#define CCSTDC_ASSERT_H
#include <qdebug.h>
#include <QMessageBox>
#ifdef QT_DEBUG // This is a config macro in qt, it will be defined when u r in Debug Complition mode!
// Do Spin and Busy-Terminate in current process, view it as the PANIC in Linux
#define do_CCSTDC_Spin while(1)

// Basic Assert when print info to the console
#define _CCSTDC_Assert(condition, need_Spin) \
do{\
        if(!(condition))\
    {\
            qDebug() << "Catch Exception:" << #condition;\
            qDebug() << "Exception is occurred in File:> " << __FILE__;\
            qDebug() << "in function" << __FUNCTION__ << ", in line" << __LINE__;\
    }\
        if(need_Spin)\
    {\
            qDebug() << "Process Spining...";\
            do_CCSTDC_Spin;\
    }\
}while(0)

#define CCSTDC_GUI_Assert_QuickMSG(condition) QString("Catch Exception: ") + #condition +  "\n" + \
    "Exception is occurred in File:> " + __FILE__ + "\n"\
    "in function:> " + __FUNCTION__ + "\n" + \
    "in line:> " + QString::number(__LINE__) + "\n"

// Do print info in the MSGBX Way
// can config into the console printable
#define _CCSTDC_MSGBOX_Assert(condition, need_Spin, MSG_TITLE, MSG_INFO, ParentWidget_Ptr, need_Print_to_Console) \
    do{\
        if(!(condition))\
    {\
            QMessageBox::critical(ParentWidget_Ptr, MSG_TITLE, CCSTDC_GUI_Assert_QuickMSG(condition) + MSG_INFO);\
    }\
        if(need_Print_to_Console)\
    {\
            qDebug() << "Catch Exception:" << #condition;;\
            qDebug() << "Exception is occurred in File:> " << __FILE__;\
            qDebug() << "in function" << __FUNCTION__ << ", in line" << __LINE__;\
            if(need_Spin)\
            qDebug() << "Process Spining..."; \
    }\
        if(need_Spin)\
        do_CCSTDC_Spin;\
}while(0)


#else
// Following blocks are supposed to be None when it is releasing model and is able to be optimized by compiler
#define _CCSTDC_Assert(condition, need_Spin) (void)0
#define _CCSTDC_MSGBOX_Assert(condition, need_Spin, MSG_TITLE, MSG_INFO, ParentWidget_Ptr, need_Print_to_Console) (void)0
#define CCSTDC_GUI_Assert_QuickMSG QString("")
#define do_CCSTDC_Spin (void)0
#endif

#define CCSTDC_Only_Assert(condition) _CCSTDC_Assert(condition, false)

#define CCSTDC_Spin_Assert(condition) _CCSTDC_Assert(condition, true)

#define CCSTDC_NSPIN_MSGBOX_Assert(condition, MSG_TITLE, MSG_INFO, need_Print_to_Console) \
    _CCSTDC_MSGBOX_Assert(condition, false, MSG_TITLE, MSG_INFO, nullptr, need_Print_to_Console)

#define CCSTDC_SPIN_MSGBOX_Assert(condition, MSG_TITLE, MSG_INFO, need_Print_to_Console) \
        _CCSTDC_MSGBOX_Assert(condition, true, MSG_TITLE, MSG_INFO, nullptr, need_Print_to_Console)

#define CCSTDC_STDGUI_Only_Assert(condition, MSG_TITLE, MSG_INFO)\
        _CCSTDC_MSGBOX_Assert(condition, false, MSG_TITLE, MSG_INFO, nullptr, true)

#define CCSTDC_STDGUI_Spin_Assert(condition, MSG_TITLE, MSG_INFO)\
        _CCSTDC_MSGBOX_Assert(condition, true, MSG_TITLE, MSG_INFO, nullptr, true)

// Common in Releasing
#define _CCSTDC_Default_Existed_Policy return
#define _CCSTDC_STDGUI_Error_Occurred(TITLE, TEXT, PTR_ParentWidget, IsAllowedMoveOn) \
        do{\
            QMessageBox::critical(PTR_ParentWidget, TITLE, TEXT);\
            if(IsAllowedMoveOn)\
            return;\
    }while(0)
#define CCSTDC_No_Select_Choice _CCSTDC_Default_Existed_Policy
#define CCSTDC_Error_Occured(TITLE, TEXT) _CCSTDC_STDGUI_Error_Occurred(TITLE, TEXT, nullptr, true)
#define CCSTDC_Warning_Occured(TITLE, TEXT) _CCSTDC_STDGUI_Error_Occurred(TITLE, TEXT, nullptr, false)


#endif // CCSTDC_ASSERT_H
