// main.cpp
#include <QApplication>
#include <mpi.h>

#include "ui/MainWindow.hpp"
#include "logic/MPIHandler.hpp"

int main(int argc, char *argv[]) {
    int toReturn = 0;
    QApplication app(argc, argv);

    MPIHandler::getInstance();
    if (MPIHandler::getRank() == 0) {
        MainWindow window;
        window.show();
        toReturn = app.exec();
    }

    return toReturn;
}
