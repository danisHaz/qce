#include <iostream>
#include <string>
#include <complex>
#include "Qubit.h"
#include <Eigen/Dense>
#include <cmath>
#include <memory>
#include "Exceptions.h"

qce::Qubit::Qubit(std::complex<double> first, std::complex<double> second) {
    if (abs((first * first + second * second) - std::complex<double>(1, 0)) != std::complex<double>(0, 0)) {
        throw QUBIT_ENV_ERROR_CODE;
    }

    this->state = Eigen::Array2cd(first, second);
}

qce::Qubit::Qubit() {
    using namespace std::complex_literals;
    this->state = Eigen::Array2cd(0. + 0i, 0. + 0i);
}

qce::Qubit::Qubit(const Qubit& qubit) {
    this->state = Eigen::Array2cd(qubit.state[0], qubit.state[1]);
}

qce::Qubit::Qubit(const qce::QubitState& otherState) {
    this->state = qce::QubitState(otherState[0], otherState[1]);
}

qce::Qubit::Qubit(std::shared_ptr<Qubit> qubitPtr) {
    this->state = Eigen::Array2cd(qubitPtr->state[0], qubitPtr->state[1]);
}

qce::QubitStateT qce::Qubit::braState() const {
    qce::QubitState conjugatedState = this->state.conjugate().nestedExpression();
    return Eigen::Transpose<qce::QubitState>(conjugatedState).nestedExpression();
}

/*
    For now it's actually original state
*/
qce::QubitState qce::Qubit::ketState() const {
    return this->state;
}

qce::QubitState qce::Qubit::getState() const {
    return this->state;
}

void qce::Qubit::setState(const QubitState& newState) {
    this->state = qce::QubitState(newState[0], newState[1]);
}

qce::DMatrState qce::Qubit::getDensityMatrix() const {
    return this->ketState() * this->braState();
}