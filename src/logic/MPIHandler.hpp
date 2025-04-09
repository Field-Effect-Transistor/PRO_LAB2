// MPIHandler.hpp
#pragma once
#include <mpi.h>

class MPIHandler {
public:
    static MPIHandler& getInstance();

private:
    MPIHandler();
    ~MPIHandler();

    MPIHandler(const MPIHandler&) = delete;
    MPIHandler& operator=(const MPIHandler&) = delete;
};
