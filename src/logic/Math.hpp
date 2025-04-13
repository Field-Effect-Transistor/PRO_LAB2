//  Math.hpp

#pragma once

#include <ostream>

class Math {
    Math() = delete;
    Math(const Math&) = delete;
    Math& operator=(const Math&) = delete;

    ~Math() = delete;

public:
    static double** createMatrix(int n);
    static double** createMatrix(int n, double value);
    static void deleteMatrix(double** matrix, int n);

    static double* createVector(int n);
    static double* createVector(int n, double value);
    static void deleteVector(double* vector);

    static double** multiply(const double*const* matrixA, const double*const* matrixB, int n);
    static double** multiply(const double*const* matrix, const double scalar, int n);
    static double* multiply(const double*const* matrix, const double* vector, int n);
    static double* multiply(const double* vector, const double*const* matrix, int n);
    static double* multiply(const double* vectorA, const double scalar, int n);
    static double* add(const double* vectorA, const double* vectorB, int n);
    static double* add(const double* vectorA, double scalar, int n);
    static double** add(const double*const* matrixA, const double*const* matrixB, int n);
    static double* sub(const double* vectorA, const double* vectorB, int n);
    static double multiply(const double* vectorA, const double* vectorB, int n);

    static void print(std::ostream& stream, const double* vector, int n);
    static void print(std::ostream& stream, const double*const* matrix, int n);
};
