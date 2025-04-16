// MPIHandler.cpp
#include "MPIHandler.hpp"
#include <iostream>

int MPIHandler::rank = -1;
int MPIHandler::size = -1;

MPIHandler& MPIHandler::getInstance() {
    static MPIHandler instance;
    return instance;
}

MPIHandler::MPIHandler() {
    int isInitialized;
    MPI_Initialized(&isInitialized);
    if (!isInitialized) {
        MPI_Init(nullptr, nullptr);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        std::cerr << "MPI Initialized in program with " << rank << " rank\n";
    }
}

MPIHandler::~MPIHandler() {
    int isFinalized;
    MPI_Finalized(&isFinalized);
    if (!isFinalized) {
        MPI_Finalize();
        std::cerr << "MPI Finalized in program with " << rank << " rank\n";
    }
}

int MPIHandler::getRank() {
    return rank;
}

int MPIHandler::getSize() {
    return size;
}

void MPIHandler::sendVector(const double* vector, int n, int destRank) {
    MPI_Send(&n, 1, MPI_INT, destRank, 0, MPI_COMM_WORLD);
    MPI_Send(vector, n, MPI_DOUBLE, destRank, 0, MPI_COMM_WORLD);
}

void MPIHandler::sendMatrix(const double*const* matrix, int n, int destRank) {
    MPI_Send(&n, 1, MPI_INT, destRank, 0, MPI_COMM_WORLD);
    for (int i = 0; i < n; ++i) {
        MPI_Send(matrix[i], n, MPI_DOUBLE, destRank, 0, MPI_COMM_WORLD);
    }
}

double* MPIHandler::receiveVector(int& n,int sourceRank) {
#ifdef DEBUG_MODE
    if (!MPI_Recv(&n, 1, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) {
        std::cerr << "I am " << MPIHandler::getRank() << " and I received from" << sourceRank << std::endl;
    }
#else
    MPI_Recv(&n, 1, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
    double* data = new double[n];

#ifdef DEBUG_MODE
    if (!MPI_Recv(data, n, MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) {
        std::cerr << "I am " << MPIHandler::getRank() << " and I received from" << sourceRank << std::endl;
    }
#else
    MPI_Recv(data, n, MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
    return data;
}

double** MPIHandler::receiveMatrix(int& n, int sourceRank) {
    MPI_Recv(&n, 1, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    double** matrix = new double*[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new double[n];
        MPI_Recv(matrix[i], n, MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    return matrix;
}

int MPIHandler::getDest(int stmtRank)  { return (stmtRank - 1) % (size - 1) + 1; }