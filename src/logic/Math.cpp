//  Math.cpp

#include "Math.hpp"

double** Math::createMatrix(int n) {
    double** matrix = new double*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[n] {0};
    }
    return matrix;
}

void Math::deleteMatrix(double** matrix, int n) {
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

double* Math::createVector(int n) {
    double* vector = new double[n] {0};
    return vector;
}

void Math::deleteVector(double* vector) {
    delete[] vector;
}

double** Math::multiply(const double*const* matrixA, const double*const* matrixB, int n) {
    double** result = createMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return result;
}

double* Math::multiply(const double*const* matrix, const double* vector, int n) {
    double* result = createVector(n);
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    return result;
}

double* Math::multiply(const double* vector, const double*const* matrix, int n) {
    double* result = createVector(n);
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += vector[j] * matrix[j][i];
        }
    }
    return result;
}

double* Math::multiply(const double* vectorA, double scalar, int n) {
    double* result = createVector(n);
    for (int i = 0; i < n; i++) {
        result[i] = vectorA[i] * scalar;
    }
    return result;
}

double* Math::add(const double* vectorA, const double* vectorB, int n) {
    double* result = createVector(n);
    for (int i = 0; i < n; i++) {
        result[i] = vectorA[i] + vectorB[i];
    }
    return result;
}

double* Math::add(const double* vectorA, double scalar, int n) {
    double* result = createVector(n);
    for (int i = 0; i < n; i++) {
        result[i] = vectorA[i] + scalar;
    }
    return result;
}

double* Math::sub(const double* vectorA, const double* vectorB, int n) {
    double* result = createVector(n);
    for (int i = 0; i < n; i++) {
        result[i] = vectorA[i] - vectorB[i];
    }
    return result;
}

double** Math::multiply(const double*const* matrix, const double scalar, int n) {
    double** result = createMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = matrix[i][j] * scalar;
        }
    }
    return result;
}

double** Math::add(const double*const* matrixA, const double*const* matrixB, int n) {
    double** result = createMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = matrixA[i][j] + matrixB[i][j];
        }
    }
    return result;
}

double Math::multiply(const double* vectorA, const double* vectorB, int n) {
    double result = 0;
    for (int i = 0; i < n; i++) {
        result += vectorA[i] * vectorB[i];
    }
    return result;
}
