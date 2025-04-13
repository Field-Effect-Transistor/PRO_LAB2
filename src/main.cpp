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
    } else { while (true) {
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
        
        //  for b
        if (MPIHandler::getDest(1) == MPIHandler::getRank()) {
            int n;
#ifdef DEBUG_MODE
            if (!MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) {
                std::cerr << "I am " << MPIHandler::getRank() << " and I received from" << 0 << std::endl;
            }
#else
            MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
            
            double* vector = Math::createVector(n);
            for (int i = 0; i < n; ++i) {
                vector[i] = 26 * pow(i + 1, 3);
            }
            stream << "Vector b1:" << std::endl;
            Math::print(stream, vector, n);
            MPIHandler::sendVector(vector, n, MPIHandler::getDest(4));
            Math::deleteVector(vector);
        }

        //  for C2
        if (MPIHandler::getDest(2) == MPIHandler::getRank()) {
            int n;
#ifdef DEBUG_MODE
            if (!MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) {
                std::cerr << "I am " << MPIHandler::getRank() << " and I received from" << 0 << std::endl;
            }
#else
            MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
            double** matrix = Math::createMatrix(n);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    matrix[i][j] = 1 / (pow(i + 1, 2) + j + 1);
                }
            }
            stream << "Matrix C2:" << std::endl;
            Math::print(stream, matrix, n);

            MPIHandler::sendMatrix(matrix, n, MPIHandler::getDest(6));
            Math::deleteMatrix(matrix, n);
        }

        //  for 26b1 - c1
        if (MPIHandler::getDest(3) == MPIHandler::getRank()) {
            int n;
            auto vectorB1 = MPIHandler::receiveVector(n, 0);
            auto vectorC1 = MPIHandler::receiveVector(n, 0);
            double* temp;
            temp = Math::multiply(vectorB1, 26, n);
            Math::deleteVector(vectorB1);
            vectorB1 = Math::sub(temp, vectorC1, n);
            MPIHandler::sendVector(vectorB1, n, MPIHandler::getDest(5));
            Math::deleteVector(vectorB1);
            Math::deleteVector(temp);
            Math::deleteVector(vectorC1);
        }

        //  for y1
        if (MPIHandler::getDest(4) == MPIHandler::getRank()) {
            int n;
            double** matrixA = MPIHandler::receiveMatrix(n, 0);
            double* vectorB = MPIHandler::receiveVector(n, MPIHandler::getDest(1));
            double* y1 = Math::multiply(matrixA, vectorB, n);
            stream << "Vector y1:" << std::endl;
            Math::print(stream, y1, n);
            MPIHandler::sendVector(y1, n, MPIHandler::getDest(9));
            MPIHandler::sendVector(y1, n, MPIHandler::getDest(12));
            MPIHandler::sendVector(y1, n, MPIHandler::getDest(13));
            MPIHandler::sendVector(y1, n, MPIHandler::getDest(16));
            Math::deleteVector(y1);
            Math::deleteMatrix(matrixA, n);
            Math::deleteVector(vectorB);
        }

        //  for y2
        if (MPIHandler::getDest(5) == MPIHandler::getRank()) {
            int n;
            double** matrixA1 = MPIHandler::receiveMatrix(n, 0);
            double* vectorB1 = MPIHandler::receiveVector(n, MPIHandler::getDest(3));
            double* y2 = Math::multiply(matrixA1, vectorB1, n);
            stream << "Vector y2:" << std::endl;
            Math::print(stream, y2, n);
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(10));
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(11));
            //MPIHandler::sendVector(y2, n, MPIHandler::getDest(13));
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(14));
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(15));
            Math::deleteVector(y2);
            Math::deleteMatrix(matrixA1, n);
            Math::deleteVector(vectorB1);
        }

        //  for (B2 + 26C2)
        if (MPIHandler::getDest(6) == MPIHandler::getRank()) {
            int n;
            double** matrixC2 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(2));
            double** matrixB2 = MPIHandler::receiveMatrix(n, 0);
            double** temp = Math::multiply(matrixB2, 26, n);
            Math::deleteMatrix(matrixB2, n);
            matrixB2 = Math::add(temp, matrixC2, n);
            Math::deleteMatrix(temp, n);
            Math::deleteMatrix(matrixC2, n);
            stream << "Matrix B2 + 26C2:" << std::endl;
            Math::print(stream, matrixB2, n);
            MPIHandler::sendMatrix(matrixB2, n, MPIHandler::getDest(7));
            Math::deleteMatrix(matrixB2, n);
        }

        //  for Y3
        if (MPIHandler::getDest(7) == MPIHandler::getRank()) {
            int n;
            double** matrixA2 = MPIHandler::receiveMatrix(n, 0);
            double** matrixB2 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(6));
            double** Y3 = Math::multiply(matrixA2, matrixB2, n);
            stream << "Matrix Y3:" << std::endl;
            Math::print(stream, Y3, n);
            MPIHandler::sendMatrix(Y3, n, MPIHandler::getDest(8));
            MPIHandler::sendMatrix(Y3, n, MPIHandler::getDest(9));
            MPIHandler::sendMatrix(Y3, n, MPIHandler::getDest(10));
            Math::deleteMatrix(Y3, n);
            Math::deleteMatrix(matrixA2, n);
            Math::deleteMatrix(matrixB2, n);
        }

        //  for Y3 * Y3
        if (MPIHandler::getDest(8) == MPIHandler::getRank()) {
            int n;
            double** Y3 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(7));
            double** Y3_2 = Math::multiply(Y3, Y3, n);
            MPIHandler::sendMatrix(Y3_2, n, MPIHandler::getDest(11));
            Math::deleteMatrix(Y3, n);
            Math::deleteMatrix(Y3_2, n);
        }

        //  y1` * Y3
        if (MPIHandler::getDest(9) == MPIHandler::getRank()) {
            int n;
            double* y1 = MPIHandler::receiveVector(n, MPIHandler::getDest(4));
            double** Y3 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(7));
            double* y1_2 = Math::multiply(y1, Y3, n);
            MPIHandler::sendVector(y1_2, n, MPIHandler::getDest(12));
            stream << "Vector y1` * Y3:" << std::endl;
            Math::print(stream, y1_2, n);
            Math::deleteVector(y1);
            Math::deleteMatrix(Y3, n);
            Math::deleteVector(y1_2);
        }

        //  Y3 * y2
        if (MPIHandler::getDest(10) == MPIHandler::getRank()) {
            int n;
            double** Y3 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(7));
            double* y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(5));
            double* y2_2 = Math::multiply(Y3, y2, n);
            MPIHandler::sendVector(y2_2, n, MPIHandler::getDest(13));
            stream << "Vector Y3 * y2:" << std::endl;
            Math::print(stream, y2_2, n);
            Math::deleteMatrix(Y3, n);
            Math::deleteVector(y2);
            Math::deleteVector(y2_2);
        }

        //  y2` * Y3_2
        if (MPIHandler::getDest(11) == MPIHandler::getRank()) {
            int n;
            double* y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(5));
            double** Y3_2 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(8));
            double* y2_2 = Math::multiply(y2, Y3_2, n);
            MPIHandler::sendVector(y2_2, n, MPIHandler::getDest(14));
            stream << "Vector y2` * Y3_2:" << std::endl;
            Math::print(stream, y2_2, n);
            Math::deleteVector(y2);
            Math::deleteMatrix(Y3_2, n);
            Math::deleteVector(y2_2);
        }

        //  y1` * Y3 * y1
        if (MPIHandler::getDest(12) == MPIHandler::getRank()) {
            int n;
            double* y1 = MPIHandler::receiveVector(n, MPIHandler::getDest(4));
            double* y1_2 = MPIHandler::receiveVector(n, MPIHandler::getDest(9));
            double y1_3 = Math::multiply(y1, y1_2, n);
            stream << "scalar y1` * Y3 * y1: " << y1_3 << std::endl;
            MPI_Send(&y1_3, 1, MPI_DOUBLE, MPIHandler::getDest(15), 0, MPI_COMM_WORLD);
            Math::deleteVector(y1);
            Math::deleteVector(y1_2);
        }

        // Y3 * y2 + y1
        if (MPIHandler::getDest(13) == MPIHandler::getRank()) {
            int n;
            double* y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(4));
            double* y2_2 = MPIHandler::receiveVector(n, MPIHandler::getDest(10));
            double* y2_3 = Math::add(y2_2, y2, n);
            stream << "vector Y3 * y2 + y1: " << std::endl;
            Math::print(stream, y2_3, n);
            MPIHandler::sendVector(y2_3, n, MPIHandler::getDest(17));
            Math::deleteVector(y2);
            Math::deleteVector(y2_2);
            Math::deleteVector(y2_3);
        }

        //  y2` * Y3_2 * y2
        if (MPIHandler::getDest(14) == MPIHandler::getRank()) {
            int n;
            double* y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(5));
            double* y2_2 = MPIHandler::receiveVector(n, MPIHandler::getDest(11));
            double y2_3 = Math::multiply(y2_2, y2, n);
            stream << "scalar y2` * Y3_2 * y2: " << y2_3 << std::endl;
            MPI_Send(&y2_3, 1, MPI_DOUBLE, MPIHandler::getDest(16), 0, MPI_COMM_WORLD);
            Math::deleteVector(y2);
            Math::deleteVector(y2_2);
        }

        //  y1` * Y3 * y1 + y2`
        if (MPIHandler::getDest(15) == MPIHandler::getRank()) {
            int n;
            double* y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(5));
            double y1_3;
#ifdef DEBUG_MODE
            if (!MPI_Recv(&y1_3, 1, MPI_DOUBLE, MPIHandler::getDest(12), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) {
                std::cerr << "I am " << MPIHandler::getRank() << " and I received from" << MPIHandler::getDest(12) << std::endl;
            }
#else
            MPI_Recv(&y1_3, 1, MPI_DOUBLE, MPIHandler::getDest(12), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
            for (int i = 0; i < n; ++i) {
                y2[i] += y1_3;
            }
            stream << "y1` * Y3 * y1 + y2`: " << std::endl;
            Math::print(stream, y2, n);
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(18));
            Math::deleteVector(y2);
        }

        //  y1 * (y2` * Y3_2 * y2)
        if (MPIHandler::getDest(16) == MPIHandler::getRank()) {
            int n;
            double* y1 = MPIHandler::receiveVector(n, MPIHandler::getDest(4));
            double y2_3;
#ifdef DEBUG_MODE
            if (!MPI_Recv(&y2_3, 1, MPI_DOUBLE, MPIHandler::getDest(14), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) {
                std::cerr << "I am " << MPIHandler::getRank() << " and I received from" << MPIHandler::getDest(14) << std::endl;
            }
#else
            MPI_Recv(&y2_3, 1, MPI_DOUBLE, MPIHandler::getDest(14), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
            double* temp = Math::multiply(y1, y2_3, n);
            stream << "y1 * (y2` * Y3_2 * y2): " << std::endl;
            Math::print(stream, temp, n);
            MPIHandler::sendVector(temp, n, MPIHandler::getDest(17));
            Math::deleteVector(y1);
            Math::deleteVector(temp);
        }

        // Y3 * y2 + y1...
        if (MPIHandler::getDest(17) == MPIHandler::getRank()) {
            int n;
            double* y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(13));
            double* y2_2 = MPIHandler::receiveVector(n, MPIHandler::getDest(16));
            double* y2_3 = Math::add(y2_2, y2, n);
            MPIHandler::sendVector(y2_3, n, MPIHandler::getDest(18));
            stream << "Y3 * y2 + y1... : " << std::endl;
            Math::print(stream, y2_3, n);
            Math::deleteVector(y2);
            Math::deleteVector(y2_2);
            Math::deleteVector(y2_3);
        }

        //  x
        if (MPIHandler::getDest(18) == MPIHandler::getRank()) {
            std::cerr << "dadadsadasd " << MPIHandler::getRank();
            int n;
            double* left = MPIHandler::receiveVector(n, MPIHandler::getDest(15));
            double* right = MPIHandler::receiveVector(n, MPIHandler::getDest(17));
            double x = Math::multiply(left, right, n);
            MPI_Send(&x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            stream << "x: " << x << std::endl;
            Math::deleteVector(left);
            Math::deleteVector(right);
        }
    }}

exit:
#else
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    toReturn = app.exec();
#endif


    return toReturn;
}
