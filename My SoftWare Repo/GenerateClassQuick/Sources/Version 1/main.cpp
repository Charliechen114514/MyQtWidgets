#include "mainwindow.h"

#include <QApplication>
#define MAINWIN_TITLE "CC类构建器"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(MAINWIN_TITLE);
    w.show();
    return a.exec();
}
