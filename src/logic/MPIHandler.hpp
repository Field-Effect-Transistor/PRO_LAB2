// MPIHandler.hpp
#pragma once
#include <mpi.h>

class MPIHandler {
public:
    static MPIHandler& getInstance();
    static int getRank();
    static int getSize();

private:
    MPIHandler();
    ~MPIHandler();

    static int rank;
    static int size;

    MPIHandler(const MPIHandler&) = delete;
    MPIHandler& operator=(const MPIHandler&) = delete;
};
