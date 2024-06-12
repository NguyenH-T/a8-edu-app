#include "mainwindow.h"

#include <QApplication>
#include "physicsworld.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PhysicsWorld world;
    Model model;
    MainWindow w(world, model);
    w.show();
    return a.exec();
}
