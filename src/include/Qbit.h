#pragma once

#include <string>
#include <complex>
#include <Eigen/Dense>
#include "Utils.hpp"

namespace qce {

    typedef Eigen::Matrix<std::complex<double>, 2, 1> QubitState;
    typedef Eigen::Matrix<std::complex<double>, 1, 2> QubitStateT;
    typedef Eigen::Matrix<std::complex<double>, 2, 2> DMatrState;
    typedef Eigen::Matrix<std::complex<double>, 2, 2> QubitMat_t;
    typedef Eigen::Matrix<std::complex<double>, 4, 4> TwoQubitMat_t;
    typedef Eigen::Matrix<std::complex<double>, 8, 8> ThreeQubitMat_t;

    class Qubit {
        protected:

        QubitState state;

        public:

        Qubit();
        Qubit(const QubitState& otherState);
        Qubit(const Qubit& other);
        Qubit(std::complex<double> first, std::complex<double> second);

        QubitState getState() const;
        void setState(const QubitState& newState);
        DMatrState getDensityMatrix() const;
        QubitStateT braState() const;
        QubitState ketState() const;
    };

    class PureQubit : Qubit {
        public:

        PureQubit(std::complex<double> first, std::complex<double> second);
        PureQubit();
        PureQubit(const PureQubit& qubit);
        PureQubit(const QubitState& state); 
    };

    using namespace std::complex_literals;

    const float _RSQRROOT_OF_2 = utils::quicksqrt(2);

    const QubitState zero_basis_state = qce::QubitState(1, 0);
    const QubitState one_basis_state = qce::QubitState(0, 1);
    const QubitState plus_basis_state = (qce::QubitState(1, 1) * _RSQRROOT_OF_2);
    const QubitState minus_basis_state = (qce::QubitState(1, -1) * _RSQRROOT_OF_2);
    const QubitState plusi_basis_state = (qce::QubitState(1, 1i) * _RSQRROOT_OF_2);
    const QubitState minusi_basis_state = (qce::QubitState(1, -1i) * _RSQRROOT_OF_2);

    const QubitMat_t hadamard_gate = (_RSQRROOT_OF_2 * (qce::QubitMat_t() << 1,1,1,-1).finished());
    const QubitMat_t pauli_z_gate = (qce::QubitMat_t() << 1,0,0,-1).finished();
    const QubitMat_t pauli_y_gate = (qce::QubitMat_t() << 0,-1i,1i,0).finished();
    const QubitMat_t pauli_x_gate = (qce::QubitMat_t() << 0,1,1,0).finished();
    const QubitMat_t phase_s_gate = (qce::QubitMat_t() << 1,0,0,1i).finished();
    const TwoQubitMat_t cnot_gate = (qce::TwoQubitMat_t() <<
                                            1,0,0,0,\
                                            0,1,0,0,\
                                            0,0,0,1,\
                                            0,0,1,0).finished();
    const TwoQubitMat_t swap_gate = (qce::TwoQubitMat_t() <<
                                            1,0,0,0,\
                                            0,0,1,0,\
                                            0,1,0,0,\
                                            0,0,0,1).finished();
    const TwoQubitMat_t cz_gate = (qce::TwoQubitMat_t() <<
                                            1,0,0,0,\
                                            0,1,0,0,\
                                            0,0,1,0,\
                                            0,0,0,-1).finished();
    const TwoQubitMat_t cphase_gate = (qce::TwoQubitMat_t() <<
                                                1,0,0,0,\
                                                0,1,0,0,\
                                                0,0,1,0,\
                                                0,0,0,1i).finished();
    const ThreeQubitMat_t toffoli_gate = (qce::ThreeQubitMat_t() <<
                                                    1,0,0,0,0,0,0,0,\
                                                    0,1,0,0,0,0,0,0,\
                                                    0,0,1,0,0,0,0,0,\
                                                    0,0,0,1,0,0,0,0,\
                                                    0,0,0,0,1,0,0,0,\
                                                    0,0,0,0,0,1,0,0,\
                                                    0,0,0,0,0,0,0,1,\
                                                    0,0,0,0,0,0,1,0).finished();
    const ThreeQubitMat_t fredkin_gate = (qce::ThreeQubitMat_t() << 
                                                    1,0,0,0,0,0,0,0,\
                                                    0,1,0,0,0,0,0,0,\
                                                    0,0,1,0,0,0,0,0,\
                                                    0,0,0,1,0,0,0,0,\
                                                    0,0,0,0,1,0,0,0,\
                                                    0,0,0,0,0,0,1,0,\
                                                    0,0,0,0,0,1,0,0,\
                                                    0,0,0,0,0,0,0,1).finished();
} // namespace qce