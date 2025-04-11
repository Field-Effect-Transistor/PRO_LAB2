// MPIHandler.hpp
#pragma once
#include <mpi.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;

class MPIHandler {
public:
    static MPIHandler& getInstance();
    static int getRank();
    static int getSize();
    static int getDest(int stmtRank);
//    static int getStmtRank();


    static void sendVector(const vector<double>& vector, int n, int destRank);
    static void sendMatrix(const matrix<double>& matrix, int n, int destRank);
    static vector<double> receiveVector(int& n, int sourceRank);
    static matrix<double> receiveMatrix(int& n, int sourceRank);

private:
    MPIHandler();
    ~MPIHandler();

    static int rank;
    static int size;

    MPIHandler(const MPIHandler&) = delete;
    MPIHandler& operator=(const MPIHandler&) = delete;
};
