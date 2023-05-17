#pragma once

#include <string>
#include <complex>
#include <Eigen/Dense>
#include <cmath>
#include <memory>

namespace qce {

    typedef Eigen::Matrix<std::complex<double>, 2, 1> QubitState;
    typedef Eigen::Matrix<std::complex<double>, 1, 2> QubitStateT;
    typedef Eigen::Matrix<std::complex<double>, 2, 2> DMatrState;
    typedef Eigen::Matrix<std::complex<double>, 2, 2> QubitMat_t;
    typedef Eigen::Matrix<std::complex<double>, 4, 4> TwoQubitMat_t;
    typedef Eigen::Matrix<std::complex<double>, 8, 8> ThreeQubitMat_t;

    typedef Eigen::Matrix<std::complex<double>, Eigen::Dynamic, 1> DynamicQubitState;
    typedef Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> DynamicQubitMat_t;
    class Qubit {
        protected:

        QubitState state;

        public:

        Qubit();
        Qubit(const QubitState& otherState);
        Qubit(const Qubit& other);
        Qubit(std::complex<double> first, std::complex<double> second);
        Qubit(std::shared_ptr<Qubit> qubitPtr);

        QubitState getState() const;
        void setState(const QubitState& newState);
        DMatrState getDensityMatrix() const;
        QubitStateT braState() const;
        QubitState ketState() const;
    };
} // namespace qce