// MPIHandler.hpp
#pragma once
#include <mpi.h>

class MPIHandler {
public:
    static MPIHandler& getInstance();
    static int getRank();
private:
    MPIHandler();
    ~MPIHandler();

    static int rank;

    MPIHandler(const MPIHandler&) = delete;
    MPIHandler& operator=(const MPIHandler&) = delete;
};
