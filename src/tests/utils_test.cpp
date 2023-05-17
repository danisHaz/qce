#include <cassert>
#include <algorithm>
#include <complex>
#include <iostream>
#include <Eigen/Dense>
#include <set>

#include "Utils.hpp"

void integer_log_test() {
    assert(qce::utils::integerLog(8) == 4);
}

void binpow_test() {
    assert(qce::utils::binpow(2, 8) == 256);
    assert(qce::utils::binpow(3, 5) == 243);
    assert(abs(qce::utils::binpow(28.123L, 7LL) - 13913344161.630627) < 1e-4);
}

void kronecker_product_test() {
    Eigen::MatrixXcd matrix1 = Eigen::MatrixXcd::Identity(2, 2);
    Eigen::MatrixXcd matrix2 = Eigen::MatrixXcd::Identity(2, 2);
    Eigen::MatrixXcd result = Eigen::MatrixXcd::Identity(4, 4);
    assert(qce::utils::kroneckerProduct(matrix1, matrix2) == result);

    // matrix1 = Eigen::MatrixXcd::Identity(2, 2);
    // matrix2 = Eigen::MatrixXcd::Identity(2, 2);
    result = (Eigen::MatrixXcd(4, 4) << 1,0,0,0,
                                        0,1,0,0,
                                        0,0,1,0,
                                        0,0,1,0).finished();
    assert(qce::utils::kroneckerProduct(matrix1, matrix2) != result);

    using namespace std::complex_literals;
    result = (Eigen::MatrixXcd(4, 4) << 1,0,0,0,
                                        0,1,0,0,
                                        0,0,1,0,
                                        0,0,0,1i).finished();
    assert(qce::utils::kroneckerProduct(matrix1, matrix2) != result);
}

void rrandomChoice_test() {
    auto probs = (Eigen::Matrix<double, -1, 1>(8) << 0, 1./8, 3./8, 0, 0, 0, 5./16, 3./16).finished();
    auto possibleOutcomes = std::set<unsigned>{1, 2, 6, 7};

    for (std::size_t i = 0; i < 100; i++) {
        auto result = qce::utils::probabilityRandomChoice(probs);
        assert(0 <= result && result < probs.size() && possibleOutcomes.find(result) != possibleOutcomes.end());
    }
}

void rrandomChoice_test1() {
    auto probs = (Eigen::Matrix<double, -1, 1>(8) << 0, 1, 0, 0, 0, 0, 0, 0).finished();
    auto possibleOutcome = 1;

    for (std::size_t i = 0; i < 100; i++) {
        auto result = qce::utils::probabilityRandomChoice(probs);
        assert(0 <= result && result < probs.size() && possibleOutcome == result);
    }
}

int main() {
    integer_log_test();
    binpow_test();
    kronecker_product_test();
    rrandomChoice_test();
    rrandomChoice_test1();
    return 0;
}