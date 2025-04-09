// MPIHandler.cpp
#include "MPIHandler.hpp"
#include <iostream>

int MPIHandler::rank = -1;

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