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

void MPIHandler::sendVector(const vector<double>& vector, int n, int destRank) {
    double* data = new double[n];
    for (int i = 0; i < n; i++) {
        data[i] = vector[i];
    }
    MPI_Send(data, n, MPI_DOUBLE, destRank, 0, MPI_COMM_WORLD);
    delete[] data;
}

void MPIHandler::sendMatrix(const matrix<double>& matrix, int n, int destRank) {
    double* data = new double[n * n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            data[i * n + j] = matrix(i, j);
        }
    }
    MPI_Send(data, n * n, MPI_DOUBLE, destRank, 0, MPI_COMM_WORLD);
    delete[] data;
}

vector<double> MPIHandler::receiveVector(int n, int sourceRank) {
    double* data = new double[n];
    MPI_Recv(data, n, MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    vector<double> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = data[i];
    }
    delete[] data;
    return result;
}

matrix<double> MPIHandler::receiveMatrix(int n, int sourceRank) {
    double* data = new double[n * n];
    MPI_Recv(data, n * n, MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    matrix<double> result(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result(i, j) = data[i * n + j];
        }
    }
    delete[] data;
    return result;
}