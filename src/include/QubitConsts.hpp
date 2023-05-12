#pragma once

#include <string>
#include <complex>
#include "Utils.hpp"
#include <Eigen/Dense>
#include "Qubit.h"

#ifndef __Qubit_consts__
#define __Qubit_consts__

namespace qce {
namespace qubitconsts {
using namespace std::complex_literals;

const float _RSQRROOT_OF_2 = 1 / sqrt(2);

const QubitMat_t identity = QubitMat_t::Identity(2, 2);

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
} // qubitconsts
} // qce
#endif