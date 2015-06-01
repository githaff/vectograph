#include "MainWindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //создаём сцену
    //её размер = размеру view
    w.synchronizeSceneAndView();
    return a.exec();
}
