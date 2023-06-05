#include "Matr.hpp"
#include <cstddef>
#include <cstdint>

#ifndef MATRIX_DIMENSIONS_NOT_EQUAL
#define MATRIX_DIMENSIONS_NOT_EQUAL
#endif

template<typename dtype>
qce::linalg::Matr<dtype>::Matr(const std::size_t n, const std::size_t m): n{n}, m{m} {
    this->matrix = new dtype[n];
    for (std::size_t i = 0; i < n; i++) {
        this->matrix[i] = new dtype[m];
    }
}

template<typename dtype>
qce::linalg::Matr<dtype>::Matr(const Matr& other): n{other.n}, m{other.m} {
    this->matrix = other.matrix;
}

template<typename dtype>
qce::linalg::Matr<dtype>::Matr(const std::size_t n, const std::size_t m, dtype** matrix): n{n}, m{m} {
    this->matrix = matrix;
}

template<typename dtype>
qce::linalg::Matr<dtype>::~Matr() {
    std::size_t matrixSize = sizeof(this->matrix) / sizeof(dtype);
    for (std::size_t i = 0; i < this->matrix; i++) {
        delete[] this->matrix[i];
    }

    delete[] this->matrix;
}

template<typename dtype>
const qce::linalg::Matr<dtype>& qce::linalg::Matr<dtype>::add(const Matr& other) {
    if (this->n != other.n || this->m != other.m) {
        throw MATRIX_DIMENSIONS_NOT_EQUAL;
    }

    using namespace qce::linalg;
    Matr<dtype> result(this->n, this->m);

    for (std::size_t i = 0; i < this->n; i++) {
        for (std::size_t j = 0; j < this->m; j++) {
            result.matrix[i][j] = this->matrix[i][j] + other.matrix[i][j];
        }
    }

    return static_cast<const Matr<dtype>&>(result);
}