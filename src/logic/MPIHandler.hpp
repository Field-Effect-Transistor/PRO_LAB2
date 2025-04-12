// MPIHandler.hpp
#pragma once
#include <mpi.h>

#include "Math.hpp"

class MPIHandler {
public:
    static MPIHandler& getInstance();
    static int getRank();
    static int getSize();
    static int getDest(int stmtRank);
//    static int getStmtRank();


    static void sendVector(const double* vector, int n, int destRank);
    static void sendMatrix(const double*const* matrix, int n, int destRank);
    static double* receiveVector(int& n, int sourceRank);
    static double** receiveMatrix(int& n, int sourceRank);

private:
    MPIHandler();
    ~MPIHandler();

    static int rank;
    static int size;

    MPIHandler(const MPIHandler&) = delete;
    MPIHandler& operator=(const MPIHandler&) = delete;
};
