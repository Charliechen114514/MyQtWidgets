#ifndef MAINWINDOWCLASSESMANAGER_H
#define MAINWINDOWCLASSESMANAGER_H

#include "newclass.h"
class MainWindowClassesManager
{
public:
    MainWindowClassesManager() = default;

    template<typename findee>
    static findee* match(QList<findee*>& lists, QString className)
    {
        for(findee* each : lists)
            if(each->getName() == className)
                return each;

        return nullptr;
    }
};

#endif // MAINWINDOWCLASSESMANAGER_H
