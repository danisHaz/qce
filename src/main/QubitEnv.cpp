#include "QubitEnv.hpp"
#include <algorithm>
#include "Exceptions.h"

qce::QubitEnv::QubitEnv() {
    this->arr = std::vector<qce::Qubit>();
}

qce::QubitEnv::QubitEnv(Qubit* qubitArr, std::size_t arrayLength) {
    this->arr = std::vector<qce::Qubit>(arrayLength);
    for (std::size_t i = 0; i < arrayLength; i++) {
        this->arr[i] = qubitArr[i];
    }
}

qce::QubitEnv::QubitEnv(const std::vector<qce::Qubit>& vec) {
    this->arr = std::vector<qce::Qubit>(vec.size());
    for (std::size_t i = 0; i < vec.size(); i++) {
        this->arr[i] = qce::Qubit(vec[i]);
    }
}

qce::QubitEnv::QubitEnv(const Qubit& qubit) {
    this->arr = std::vector<qce::Qubit>(1);
    this->arr[0] = qce::Qubit(qubit);
}

qce::QubitEnv::QubitEnv(std::size_t qubitNumber, Qubit initialState) {
    this->arr = std::vector<qce::Qubit>(qubitNumber);
    for (std::size_t i = 0; i < this->arr.size(); i++) {
        this->arr[i] = qce::Qubit(initialState);
    }
}

bool qce::QubitEnv::checkQubitIndOutOfRange(unsigned qubitIndex) {
    if (qubitIndex >= this->arr.size()) {
        return false;
    }

    return true;
}

void qce::QubitEnv::applyOperationOnQubit(unsigned qubitIndex, QubitMat_t mat) {
    if (!checkQubitIndOutOfRange(qubitIndex)) {
        throw INVALID_ARGUMENT_ERROR_CODE;
    }

    const QubitState qubit = this->arr[qubitIndex].getState();
    this->arr[qubitIndex].setState(mat * qubit);
}

void qce::QubitEnv::applyOperationOn2Qubit(unsigned fQubitIndex, unsigned sQubitIndex, TwoQubitMat_t mat) {
    if (!checkQubitIndOutOfRange(fQubitIndex) || !checkQubitIndOutOfRange(sQubitIndex)) {
        throw INVALID_ARGUMENT_ERROR_CODE;
    }

    const QubitState fqState = this->arr[fQubitIndex].getState();
    const QubitState sqState = this->arr[sQubitIndex].getState();
    Eigen::Matrix<std::complex<double>, 4, 1> combinedQState =
        (Eigen::Matrix<std::complex<double>, 4, 1>() << fqState[0] * sqState[0], fqState[0] * sqState[1],\
            fqState[1] * sqState[0], fqState[1] * sqState[1]).finished();

    Eigen::Matrix<std::complex<double>, 4, 1> resCombinedQState =
        mat * combinedQState;
    
    const QubitState resFqState = (QubitState() << resCombinedQState[0], resCombinedQState[1])
        .finished();
    const QubitState resSqState = (QubitState() << resCombinedQState[2], resCombinedQState[3])
        .finished();

    this->arr[fQubitIndex].setState(resFqState);
    this->arr[sQubitIndex].setState(resSqState);
}

void qce::QubitEnv::hadamard(unsigned qubitIndex) {
    applyOperationOnQubit(qubitIndex, hadamard_gate);
}

void qce::QubitEnv::x(unsigned qubitIndex) {
    applyOperationOnQubit(qubitIndex, pauli_x_gate);
}

void qce::QubitEnv::y(unsigned qubitIndex) {
    applyOperationOnQubit(qubitIndex, pauli_y_gate);
}

void qce::QubitEnv::z(unsigned qubitIndex) {
    applyOperationOnQubit(qubitIndex, pauli_z_gate);
}

void qce::QubitEnv::s(unsigned qubitIndex) {
    applyOperationOnQubit(qubitIndex, phase_s_gate);
}

void qce::QubitEnv::cnot(unsigned inverseQubitIndex, unsigned controlQubitIndex) {
    applyOperationOn2Qubit(controlQubitIndex, inverseQubitIndex, cnot_gate);
}

qce::Qubit qce::QubitEnv::getQubit(unsigned qubitIndex) const {
    Qubit q = this->arr[qubitIndex];
    return q.getState();
}