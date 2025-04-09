// main.cpp
#include <QApplication>
#include <mpi.h>

#include "ui/MainWindow.hpp"
#include "logic/MPIHandler.hpp"

int main(int argc, char *argv[]) {
    int rank, toReturn = 0;
    QApplication app(argc, argv);

    MPIHandler::getInstance();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        MainWindow window;
        //MenuWidget window;
        //EnterWidget window;
        window.show();
        toReturn = app.exec();
    }

    return toReturn;
}
