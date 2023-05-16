#include <random>

#include "Utils.hpp"
#include "Qubit.h"

Eigen::MatrixXcd qce::utils::kroneckerProduct(
    const Eigen::MatrixXcd &first,
    const Eigen::MatrixXcd &second
) {
    const std::size_t resultRows = first.rows() * second.rows();
    const std::size_t resultCols = first.cols() * second.cols();

    Eigen::MatrixXcd result(resultRows, resultCols);
    for (unsigned i = 0; i < first.rows(); i++) {
        for (unsigned j = 0; j < first.cols(); j++) {
            result.block(i*second.rows(), j*second.cols(), second.rows(), second.cols()) = first(i,j) * second;
        }
    }

    return result;
}

int qce::utils::rrandom32() {
    return (_rrand32u_func() - (RAND_MAX >> 1) - 1) * 2;
}

long qce::utils::rrandom64() {
    long _rand32 = rrandom32();
    return ((_rand32 << 16) + _rand32);
}

unsigned qce::utils::rrandom32u() {
    return _rrand32u_func();
}

unsigned long qce::utils::rrandom64u() {
    return ((_rrand32u_func() << 16) + _rrand32u_func());
}

qce::DynamicQubitState qce::utils::combineStates(
    const qce::DynamicQubitState &first,
    const qce::DynamicQubitState &second
) {
    std::size_t resultSize = first.size() * second.size();
    qce::DynamicQubitState result(resultSize);
    
    for (std::size_t i = 0; i < first.size(); i++) {
        for (std::size_t j = 0; j < second.size(); j++) {
            result[(i<<1)+j] = first[i] * second[j];
        }
    }

    return result;
}

qce::DynamicQubitState qce::utils::combineStates(
    const std::vector<qce::DynamicQubitState> &states
) {
    qce::DynamicQubitState result = states[0];

    for (std::size_t i = 1; i < states.size(); i++) {
        result = qce::utils::combineStates(result, states[i]);
    }

    return result;
}