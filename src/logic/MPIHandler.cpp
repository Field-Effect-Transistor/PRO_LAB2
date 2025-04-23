// MPIHandler.cpp
#include "MPIHandler.hpp"
#include <iostream>
#include <chrono>

int MPIHandler::rank = -1;
int MPIHandler::size = -1;

#ifndef MAXN
    #define MAXN 1000
#endif

#define CHUNK_SIZE (MAXN * MAXN)

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
    MPI_Send(&n, 1, MPI_INT, destRank, 0, MPI_COMM_WORLD);  // Відправка розміру вектора
    int chunks = n / CHUNK_SIZE;
    int lastChunkSize = n % CHUNK_SIZE;

    // Відправка частин вектора
    for (int i = 0; i < chunks; ++i) {
        MPI_Send(vector + i * CHUNK_SIZE, CHUNK_SIZE, MPI_DOUBLE, destRank, 0, MPI_COMM_WORLD);
        std::cerr << "I am " << getRank() << " and I sent vector`s chunk to " << destRank << std::endl;
    }

    // Відправка останньої частини
    if (lastChunkSize > 0) {
        MPI_Send(vector + chunks * CHUNK_SIZE, lastChunkSize, MPI_DOUBLE, destRank, 0, MPI_COMM_WORLD);
        std::cerr << "I am " << getRank() << " and I sent last vector`s chunk to " << destRank << std::endl;
    }
}

void MPIHandler::sendMatrix(const double* const* matrix, int n, int destRank) {
    MPI_Send(&n, 1, MPI_INT, destRank, 0, MPI_COMM_WORLD);
    int size = n * n;
    int chunks = size / CHUNK_SIZE;
    int lastChunkSize = size % CHUNK_SIZE;
    for (int i = 0; i < chunks; i++) {
        MPI_Send(*matrix + i * CHUNK_SIZE, CHUNK_SIZE, MPI_DOUBLE, destRank, 0, MPI_COMM_WORLD);
        std::cerr << "I am " << getRank() << " and I sent matrix`s chunk to " << destRank << std::endl;
    }
    if (lastChunkSize) {
        MPI_Send(*matrix + chunks * CHUNK_SIZE, lastChunkSize, MPI_DOUBLE, destRank, 0, MPI_COMM_WORLD);
        std::cerr << "I am " << getRank() << " and I sent last matrix`s chunk to " << destRank << std::endl;
    }
    //MPI_Send(*matrix, n * n, MPI_DOUBLE, destRank, 0, MPI_COMM_WORLD);
}

double* MPIHandler::receiveVector(int& n, int sourceRank) {
    MPI_Recv(&n, 1, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Отримання розміру вектора
    double* data = new double[n];
    int chunks = n / CHUNK_SIZE;
    int lastChunkSize = n % CHUNK_SIZE;

    // Отримання частин вектора
    for (int i = 0; i < chunks; ++i) {
        MPI_Recv(data + i * CHUNK_SIZE, CHUNK_SIZE, MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cerr << "I am " << getRank() << " and I received vector`s chunk from " << sourceRank << std::endl;
    }

    // Отримання останньої частини
    if (lastChunkSize > 0) {
        MPI_Recv(data + chunks * CHUNK_SIZE, lastChunkSize, MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cerr << "I am " << getRank() << " and I received last vector`s chunk from " << sourceRank << std::endl;
    }

    return data;
}


double** MPIHandler::receiveMatrix(int& n, int sourceRank) {
    auto start = std::chrono::high_resolution_clock::now(); 
    MPI_Recv(&n, 1, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Отримання розміру матриці
    double** matrix = new double*[n];
    double* data = new double[n * n];
    int chunks = (n * n) / CHUNK_SIZE;
    int lastChunkSize = (n * n) % CHUNK_SIZE;

    // Отримання частин матриці
    for (int i = 0; i < chunks; ++i) {
        MPI_Recv(data + i * CHUNK_SIZE, CHUNK_SIZE, MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cerr << "I am " << getRank() << " and I received matrix`s chunk from " << sourceRank << std::endl;
    }

    // Отримання останньої частини матриці
    if (lastChunkSize > 0) {
        MPI_Recv(data + chunks * CHUNK_SIZE, lastChunkSize, MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cerr << "I am " << getRank() << " and I received last matrix`s chunk from " << sourceRank << std::endl;
    }

    // Формування матриці з отриманих даних
    for (int i = 0; i < n; ++i) {
        matrix[i] = data + i * n;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "I am " << getRank() << "received from " << sourceRank << std::endl;
    for (int i = 0; i < ((n < 10)? n : 10); i++) {
        std::cout << matrix[0][i] << " ";
    }
    std::cout << "Час виконання: " << diff.count() << " секунд\n";


    return matrix;
}

int MPIHandler::getDest(int stmtRank)  { return (stmtRank - 1) % (size - 1) + 1; }
