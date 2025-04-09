// main.cpp
#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    //MainWindow window;
    MenuWidget window;
    window.show();
    return app.exec();
}