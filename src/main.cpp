// main.cpp
#include <QApplication>
#include <cmath>
#include <iostream>

#include "ui/MainWindow.hpp"
#include "ui/NotificationWidget.hpp"
#include "logic/Math.hpp"

#ifdef MPI_ON
    #include <mpi.h>
#endif

//#define DEBUG_MODE

int main(int argc, char *argv[]) {
    int toReturn = 0;
#ifdef MPI_ON
    MPIHandler::getInstance();
    // ui
    if (MPIHandler::getRank() == 0) {
        std::cerr << MPIHandler::getSize() << std::endl;
        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        toReturn = app.exec();
    } else {
        while (true) {
            bool isExit = false;
#ifdef DEBUG_MODE
            if (!MPI_Recv(&isExit, 1, MPI_CXX_BOOL, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) {
                std::cerr << "I am " << MPIHandler::getRank() << " and I received from" << 0 << std::endl;
            }
#else
            MPI_Recv(&isExit, 1, MPI_CXX_BOOL, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
            if (isExit)
                goto exit;
            std::string path = "result_" + std::to_string(MPIHandler::getRank()) + "_" + std::to_string(((int)time(NULL) >> 2) % 1000);
            std::fstream stream(path, std::ios::out | std::ios::app);
        
            if (1 == MPIHandler::getRank()) {
                int n;
                double** matrixA = MPIHandler::receiveMatrix(n, 0);
                double* vectorB = Math::createVector(n);
                for (int i = 0; i < n; ++i) {
                    vectorB[i] = 26 * pow(i, 3);
                }
                stream << "Vector b:" << std::endl;
                Math::print(stream, vectorB, n);

                double* y1 = Math::multiply(matrixA, vectorB, n);
                stream << "Vector y1:" << std::endl;
                Math::print(stream, y1, n);

                MPIHandler::sendVector(y1, n, 0);
                MPIHandler::sendVector(y1, n, 2);

                double** Y3 = MPIHandler::receiveMatrix(n, 2);
                double* y1_Y3 = Math::multiply(y1, Y3, n);
                double y1_Y3_y1 = Math::multiply(y1_Y3, y1, n);
                stream << "Scalar y1_Y3_y1:" << y1_Y3_y1 << std::endl;

                double* y2 = MPIHandler::receiveVector(n, 0);
                double* y1_Y3_y1_y2 = Math::add(y2, y1_Y3_y1, n);
                stream << "Vector y1_Y3_y1_y2:" << std::endl;
                Math::print(stream, y1_Y3_y1_y2, n);

                MPIHandler::sendVector(y1_Y3_y1_y2, n, 0);

                Math::deleteMatrix(matrixA, n);
                Math::deleteVector(vectorB);
                Math::deleteVector(y1);
                Math::deleteMatrix(Y3, n);
                Math::deleteVector(y1_Y3);
                Math::deleteVector(y2);
                Math::deleteVector(y1_Y3_y1_y2);
            }

            if (2 == MPIHandler::getRank()) {
                int n;
                double** matrixB2 = MPIHandler::receiveMatrix(n, 0);
                //  for C2
                double** matrixC2 = Math::createMatrix(n);
                for(int i = 0; i < n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        matrixC2[i][j] = 1 / (pow(i + 1, 2) + j + 1);
                    }
                }
                stream << "Matrix C2:" << std::endl;
                Math::print(stream, matrixC2, n);

                double** matrixA2 = MPIHandler::receiveMatrix(n, 0);
                double** matrix26C2 = Math::multiply(matrixC2, 26, n);
                double** matrixB2_26C2 = Math::multiply(matrixB2, matrix26C2, n);
                stream << "Matrix B2_26C2:" << std::endl;
                Math::print(stream, matrixB2_26C2, n);

                double** Y3 = Math::multiply(matrixA2, matrixB2_26C2, n);
                stream << "Matrix Y3:" << std::endl;
                Math::print(stream, Y3, n);

                MPIHandler::sendMatrix(Y3, n, 0);
                MPIHandler::sendMatrix(Y3, n, 1);

                double** Y3_2 = Math::multiply(Y3, Y3, n);
                stream << "Matrix Y3_2:" << std::endl;
                Math::print(stream, Y3_2, n);

                double* y2 = MPIHandler::receiveVector(n, 0);
                double* y2_Y3_2 = Math::multiply(y2, Y3_2, n);
                stream << "Vector y2_Y3_2:" << std::endl;
                Math::print(stream, y2_Y3_2, n);

                double y2_Y3_2_y2 = Math::multiply(y2_Y3_2, y2, n);
                stream << "Scalar y2_Y3_2_y2:" << y2_Y3_2_y2 << std::endl;

                double* y1 = MPIHandler::receiveVector(n, 1);
                double* y1_y2_Y3_2_y2 = Math::multiply(y1, y2_Y3_2_y2, n);
                stream << "Vector y1_y2_Y3_2_y2:" << std::endl;
                Math::print(stream, y1_y2_Y3_2_y2, n);

                MPIHandler::sendVector(y1_y2_Y3_2_y2, n, 0);

                Math::deleteMatrix(matrixB2, n);
                Math::deleteMatrix(matrixC2, n);
                Math::deleteMatrix(matrixA2, n);
                Math::deleteMatrix(matrix26C2, n);
                Math::deleteMatrix(matrixB2_26C2, n);
                Math::deleteMatrix(Y3, n);
                Math::deleteMatrix(Y3_2, n);
                Math::deleteVector(y2);
                Math::deleteVector(y2_Y3_2);
                Math::deleteVector(y1);
                Math::deleteVector(y1_y2_Y3_2_y2);
            }
        }
    }

exit:
#else
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    toReturn = app.exec();
#endif


    return toReturn;
}
