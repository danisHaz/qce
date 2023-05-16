#include <cassert>
#include <iostream>
#include <complex>

#include "QubitEnv.hpp"
#include "OperationGraph.hpp"
#include "Simulator.hpp"
#include "QubitConsts.hpp"
#include "Qubit.h"

const double GATE_EQ_PRECISION = 1e-5;

void qubit_env_hadamard_test1() {
    qce::QubitEnv env(3, qce::qubitconsts::zero_basis_state);
    env.hadamard(0);
    env.hadamard(1);
    env.hadamard(2);

    qce::simulator::SimpleSimulator sim;
    qce::simulator::Solution sol = sim.constructSolution(env);
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2 / 2;
    auto idealResult = (qce::DynamicQubitState(8) << ampl,ampl,ampl,ampl,ampl,ampl,ampl,ampl).finished();

    assert(sol.getResult().isApprox(idealResult, GATE_EQ_PRECISION));
}

void qubit_env_hadamard_test2() {
    qce::QubitEnv env(3, qce::qubitconsts::zero_basis_state);
    env.hadamard(0);
    env.hadamard(2);

    qce::simulator::SimpleSimulator sim;
    qce::simulator::Solution sol = sim.constructSolution(env);
    const float ampl = (float)1 / 2;
    auto idealResult = (qce::DynamicQubitState(8) << ampl,ampl,0,0,ampl,ampl,0,0).finished();

    assert(sol.getResult().isApprox(idealResult, GATE_EQ_PRECISION));
}

void qubit_env_hadamard_test3() {
    qce::QubitEnv env(4, qce::qubitconsts::zero_basis_state);
    env.hadamard(1);
    env.hadamard(2);
    env.hadamard(3);

    qce::simulator::SimpleSimulator sim;
    qce::simulator::Solution sol = sim.constructSolution(env);
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2 / 2;
    auto idealResult = (qce::DynamicQubitState(16)   << ampl,ampl,ampl,ampl,ampl,ampl,ampl,ampl,\
                                                        0,0,0,0,0,0,0,0).finished();

    assert(sol.getResult().isApprox(idealResult, GATE_EQ_PRECISION));
}

void qubit_env_no_gates_test() {
    qce::QubitEnv env(3, qce::qubitconsts::one_basis_state);

    qce::simulator::SimpleSimulator sim;
    qce::simulator::Solution sol = sim.constructSolution(env);
    auto idealResult = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,0,1).finished();

    assert(sol.getResult().isApprox(idealResult, GATE_EQ_PRECISION));
}

void qubit_env_x_gate_test() {
    qce::QubitEnv env(3, qce::qubitconsts::zero_basis_state);
    env.x(1);
    env.x(2);
    env.x(0);

    qce::simulator::SimpleSimulator sim;
    qce::simulator::Solution sol = sim.constructSolution(env);
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2 / 2;
    auto idealResult = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,0,1).finished();

    assert(sol.getResult().isApprox(idealResult, GATE_EQ_PRECISION));
}

void qubit_env_hadamard_x_test() {
    qce::QubitEnv env(4, qce::qubitconsts::zero_basis_state);
    env.hadamard(0);
    env.x(0);
    env.x(2);
    env.hadamard(1);
    env.x(1);

    qce::simulator::SimpleSimulator sim;
    qce::simulator::Solution sol = sim.constructSolution(env);
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2 / 2;
    auto idealResult = (qce::DynamicQubitState(16)   << 0, 0, 0.5, 0, 0, 0, 0.5, 0,\
                                                        0, 0, 0.5, 0, 0, 0, 0.5, 0).finished();

    assert(sol.getResult().isApprox(idealResult, GATE_EQ_PRECISION));
}

void qubit_env_hadamard_y_test() {
    qce::QubitEnv env(4, qce::qubitconsts::zero_basis_state);
    env.y(1); env.hadamard(1); env.hadamard(2); env.y(3); env.y(1); env.hadamard(3);

    qce::simulator::SimpleSimulator sim;
    qce::simulator::Solution sol = sim.constructSolution(env);
    const auto ampl = std::complex<double>(0, qce::qubitconsts::_RSQRROOT_OF_2 / 2);
    auto idealResult = (qce::DynamicQubitState(16)   << -ampl, ampl, -ampl, ampl, -ampl, ampl, -ampl, ampl,\
                                                            0,    0,   0,    0,     0,    0,   0,    0).finished();

    assert(sol.getResult().isApprox(idealResult, GATE_EQ_PRECISION));
}

void qubit_env_hadamard_xyz_test() {
    qce::QubitEnv env(4, qce::qubitconsts::zero_basis_state);
    env.y(1); env.hadamard(1); env.hadamard(2); env.y(3); env.y(1); env.hadamard(3);
    env.hadamard(0); env.x(0); env.x(2); env.hadamard(1); env.x(1);
    env.hadamard(3); env.z(2); env.y(1); env.z(0); env.hadamard(2); env.z(2);

    qce::simulator::SimpleSimulator sim;
    qce::simulator::Solution sol = sim.constructSolution(env);
    const auto ampl = qce::qubitconsts::_RSQRROOT_OF_2;
    auto idealResult = (qce::DynamicQubitState(16)   <<    0,    0,    0, ampl,    0,    0,    0,    0,\
                                                           0,    0,    0, -ampl,    0,    0,    0,    0).finished();

    assert(sol.getResult().isApprox(idealResult, GATE_EQ_PRECISION));
}

int main() {
    qubit_env_no_gates_test();
    qubit_env_hadamard_test1();
    qubit_env_hadamard_test2();
    qubit_env_hadamard_test3();
    qubit_env_x_gate_test();
    qubit_env_hadamard_x_test();
    qubit_env_hadamard_y_test();
    qubit_env_hadamard_xyz_test();
}