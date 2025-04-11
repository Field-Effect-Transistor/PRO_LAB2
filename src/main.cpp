// main.cpp
#include <QApplication>
#include <mpi.h>
#include <boost/numeric/ublas/vector.hpp>
#include <cmath>

#include "ui/MainWindow.hpp"
#include "ui/NotificationWidget.hpp"
#include "logic/MPIHandler.hpp"

//#define DEBUG_MODE
/*

    You stupid idiot
    15 and 17 sends their info to 0, not to 18

    Also try to use synchro signal to return processes to start after x gained
    Or connect end slot to MPI Abort

*/

using namespace boost::numeric::ublas;

int main(int argc, char *argv[]) {
    int toReturn = 0;

    MPIHandler::getInstance();
    // ui
    if (MPIHandler::getRank() == 0) {
        std::cerr << MPIHandler::getSize() << std::endl;
        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        toReturn = app.exec();
    } else { while (true) {
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
            
            vector<double> vector(n);
            for (int i = 0; i < n; ++i) {
                vector[i] = 26 * pow(i + 1, 3);
            }
            MPIHandler::sendVector(vector, n, MPIHandler::getDest(4));
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
            matrix<double> matrix(n, n);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    matrix(i, j) = 1 / (pow(i + 1, 2) + j + 1);
                }
            }
            MPIHandler::sendMatrix(matrix, n, MPIHandler::getDest(6));
        }

        //  for 26b1 - c1
        if (MPIHandler::getDest(3) == MPIHandler::getRank()) {
            int n;
            auto vectorB1 = MPIHandler::receiveVector(n, 0);
            auto vectorC1 = MPIHandler::receiveVector(n, 0);
            vectorB1 *= 26;
            vectorB1 -= vectorC1;
            MPIHandler::sendVector(vectorB1, n, MPIHandler::getDest(5));
        }

        //  for y1
        if (MPIHandler::getDest(4) == MPIHandler::getRank()) {
            int n;
            matrix<double> matrixA = MPIHandler::receiveMatrix(n, 0);
            vector<double> vectorB = MPIHandler::receiveVector(n, MPIHandler::getDest(1));
            vector<double> y1 = prod(matrixA, vectorB);
            MPIHandler::sendVector(y1, n, MPIHandler::getDest(9));
            MPIHandler::sendVector(y1, n, MPIHandler::getDest(12));
            MPIHandler::sendVector(y1, n, MPIHandler::getDest(13));
            MPIHandler::sendVector(y1, n, MPIHandler::getDest(16));
        }

        //  for y2
        if (MPIHandler::getDest(5) == MPIHandler::getRank()) {
            int n;
            matrix<double> matrixA1 = MPIHandler::receiveMatrix(n, 0);
            vector<double> vectorB1 = MPIHandler::receiveVector(n, MPIHandler::getDest(3));
            vector<double> y2 = prod(matrixA1, vectorB1);
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(10));
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(11));
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(13));
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(14));
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(15));
        }

        //  for (B2 + 26C2)
        if (MPIHandler::getDest(6) == MPIHandler::getRank()) {
            int n;
            matrix<double> matrixC2 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(2));
            matrix<double> matrixB2 = MPIHandler::receiveMatrix(n, 0);
            matrixB2 += 26 * matrixC2;
            MPIHandler::sendMatrix(matrixB2, n, MPIHandler::getDest(7));
        }

        //  for Y3
        if (MPIHandler::getDest(7) == MPIHandler::getRank()) {
            int n;
            matrix<double> matrixA2 = MPIHandler::receiveMatrix(n, 0);
            matrix<double> matrixB2 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(6));
            matrix<double> Y3 = prod(matrixA2, matrixB2);
            MPIHandler::sendMatrix(Y3, n, MPIHandler::getDest(8));
            MPIHandler::sendMatrix(Y3, n, MPIHandler::getDest(9));
            MPIHandler::sendMatrix(Y3, n, MPIHandler::getDest(10));
        }

        //  for Y3 * Y3
        if (MPIHandler::getDest(8) == MPIHandler::getRank()) {
            int n;
            matrix<double> Y3 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(7));
            matrix<double> Y3_2 = prod(Y3, Y3);
            MPIHandler::sendMatrix(Y3_2, n, MPIHandler::getDest(11));
        }

        //  y1` * Y3
        if (MPIHandler::getDest(9) == MPIHandler::getRank()) {
            int n;
            vector<double> y1 = MPIHandler::receiveVector(n, MPIHandler::getDest(4));
            matrix<double> Y3 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(7));
            vector<double> y1_2 = prod(trans(y1), Y3);
            MPIHandler::sendVector(y1_2, n, MPIHandler::getDest(12));
        }

        //  Y3 * y2
        if (MPIHandler::getDest(10) == MPIHandler::getRank()) {
            int n;
            matrix<double> Y3 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(7));
            vector<double> y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(5));
            vector<double> y2_2 = prod(Y3, y2);
            MPIHandler::sendVector(y2_2, n, MPIHandler::getDest(13));
        }

        //  y2` * Y3_2
        if (MPIHandler::getDest(11) == MPIHandler::getRank()) {
            int n;
            vector<double> y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(5));
            matrix<double> Y3_2 = MPIHandler::receiveMatrix(n, MPIHandler::getDest(8));
            vector<double> y2_2 = prod(trans(y2), Y3_2);
            MPIHandler::sendVector(y2_2, n, MPIHandler::getDest(14));
        }

        //  y1` * Y3 * y1
        if (MPIHandler::getDest(12) == MPIHandler::getRank()) {
            int n;
            vector<double> y1 = MPIHandler::receiveVector(n, MPIHandler::getDest(4));
            vector<double> y1_2 = MPIHandler::receiveVector(n, MPIHandler::getDest(9));
            double y1_3 = inner_prod(y1_2, y1);
            MPI_Send(&y1_3, 1, MPI_DOUBLE, MPIHandler::getDest(15), 0, MPI_COMM_WORLD);
        }

        // Y3 * y2 + y1
        if (MPIHandler::getDest(13) == MPIHandler::getRank()) {
            int n;
            vector<double> y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(5));
            vector<double> y2_2 = MPIHandler::receiveVector(n, MPIHandler::getDest(10));
            vector<double> y2_3 = y2_2 + y2;
            MPIHandler::sendVector(y2_3, n, MPIHandler::getDest(17));
        }

        //  y2` * Y3_2 * y2
        if (MPIHandler::getDest(14) == MPIHandler::getRank()) {
            int n;
            vector<double> y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(5));
            vector<double> y2_2 = MPIHandler::receiveVector(n, MPIHandler::getDest(11));
            double y2_3 = inner_prod(y2_2, y2);
            MPI_Send(&y2_3, 1, MPI_DOUBLE, MPIHandler::getDest(16), 0, MPI_COMM_WORLD);
        }

        //  y1` * Y3 * y1 + y2`
        if (MPIHandler::getDest(15) == MPIHandler::getRank()) {
            int n;
            vector<double> y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(5));
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
            MPIHandler::sendVector(y2, n, MPIHandler::getDest(18));
        }

        //  y1 * (y2` * Y3_2 * y2)
        if (MPIHandler::getDest(16) == MPIHandler::getRank()) {
            int n;
            vector<double> y1 = MPIHandler::receiveVector(n, MPIHandler::getDest(4));
            double y2_3;
#ifdef DEBUG_MODE
            if (!MPI_Recv(&y2_3, 1, MPI_DOUBLE, MPIHandler::getDest(14), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) {
                std::cerr << "I am " << MPIHandler::getRank() << " and I received from" << MPIHandler::getDest(14) << std::endl;
            }
#else
            MPI_Recv(&y2_3, 1, MPI_DOUBLE, MPIHandler::getDest(14), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
            y1 *= y2_3;
            MPIHandler::sendVector(y1, n, MPIHandler::getDest(17));
        }

        // Y3 * y2 + y1...
        if (MPIHandler::getDest(17) == MPIHandler::getRank()) {
            int n;
            vector<double> y2 = MPIHandler::receiveVector(n, MPIHandler::getDest(13));
            vector<double> y2_2 = MPIHandler::receiveVector(n, MPIHandler::getDest(16));
            y2_2 += y2;
            MPIHandler::sendVector(y2_2, n, MPIHandler::getDest(18));
        }

        //  x
        if (MPIHandler::getDest(18) == MPIHandler::getRank()) {
            std::cerr << "dadadsadasd " << MPIHandler::getRank();
            int n;
            vector<double> left = MPIHandler::receiveVector(n, MPIHandler::getDest(15));
            vector<double> right = MPIHandler::receiveVector(n, MPIHandler::getDest(17));
            double x = inner_prod(left, right);
            MPI_Send(&x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }}

    return toReturn;
}
