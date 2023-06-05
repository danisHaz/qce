#include <cassert>
#include <cstdint>

#include "OperationGraph.hpp"
#include "OperationArgs.hpp"

const double GATE_EQ_PRECISION = 1e-5;

void hadamard_gate_test() {
    std::vector<std::size_t> qubitPositions = {0, 1, 2};
    std::vector<std::size_t> controlQubits;
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;
    qce::operations::HadamardGate gate(1, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,ampl,0,ampl,0).finished();

    auto result = gate.applyOperation(qce::operations::OperationArgs(state));
    auto answer = (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished();
    // std::cout << result << "\n\n";
    // std::cout << (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished() << "\n";
    assert(result.result.isApprox(answer, GATE_EQ_PRECISION));
}

void cnot_gate_test() {
    std::vector<std::size_t> qubitPositions = {0, 1, 2};
    std::vector<std::size_t> controlQubits = {0};
    qce::operations::CnotGate gate(controlQubits, 1, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished();

    auto operation = gate.constructOperation();
    auto answer = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,1,0).finished();
    assert((operation * state).isApprox(answer, GATE_EQ_PRECISION));

    controlQubits = {0};
    qubitPositions = {2, 0, 1};
    qce::operations::CnotGate gate1(controlQubits, 2, qubitPositions);
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;
    qce::DynamicQubitState state1 = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,ampl,ampl).finished();

    auto operation1 = gate1.constructOperation();
    auto answer1 = (qce::DynamicQubitState(8) << 0,0,ampl,ampl,0,0,0,0).finished();
    assert((operation1 * state1).isApprox(answer1, GATE_EQ_PRECISION));
}

void swap_gate_test() {
    std::vector<std::size_t> qubitPositions = {0, 1, 2};
    std::vector<std::size_t> controlQubits = {0};
    qce::operations::SwapGate gate(controlQubits, 1, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished();

    auto operation = gate.constructOperation();
    auto answer = (qce::DynamicQubitState(8) << 0,0,1,0,0,0,0,0).finished();
    assert((operation * state).isApprox(answer, GATE_EQ_PRECISION));

    controlQubits = {0};
    qubitPositions = {0, 1, 3};
    qce::operations::SwapGate gate1(controlQubits, 3, qubitPositions);
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;
    qce::DynamicQubitState state1 = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,ampl,ampl).finished();

    auto operation1 = gate1.constructOperation();
    auto answer1 = (qce::DynamicQubitState(8) << 0,0,0,ampl,0,0,0,ampl).finished();
    // std::cout << *operation1.result << "\n";
    assert((operation1 * state1).isApprox(answer1, GATE_EQ_PRECISION));
}

void cz_gate_test() {
    std::vector<std::size_t> qubitPositions = {0, 1, 2};
    std::vector<std::size_t> controlQubits = {0};
    qce::operations::CZGate gate(controlQubits, 2, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0.5,0.5,0,0,0.5,0.5,0,0).finished();

    auto result = gate.applyOperation(qce::operations::OperationArgs(state));
    auto answer = (qce::DynamicQubitState(8) << 0.5,0.5,0,0,0.5,-0.5,0,-0).finished();
    assert(result.result.isApprox(answer, GATE_EQ_PRECISION));

    qubitPositions = {0, 1, 2, 3};
    controlQubits = {3};
    qce::operations::CZGate gate1(controlQubits, 0, qubitPositions);
    state = (qce::DynamicQubitState(16) << 0.5,0.5,0,0,0,0,0,0,0.5,0.5,0,0,0,0,0,0).finished();

    auto result1 = gate1.applyOperation(qce::operations::OperationArgs(state));
    auto answer1 = (qce::DynamicQubitState(16) << 0.5,0.5,0,0,0,0,0,0,0.5,-0.5,0,0,0,0,0,0).finished();
    assert(result1.result.isApprox(answer1, GATE_EQ_PRECISION));
}

void cphase_gate_test() {
    std::vector<std::size_t> qubitPositions = {0, 1, 2};
    std::vector<std::size_t> controlQubits = {0};
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;
    qce::operations::CPhaseGate gate(controlQubits, 2, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2).finished();

    auto result = gate.applyOperation(qce::operations::OperationArgs(state));
    auto answer = (qce::DynamicQubitState(8) << ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,Eigen::dcomplex(0, ampl/2),ampl/2,Eigen::dcomplex(0, ampl/2)).finished();
    assert(result.result.isApprox(answer, GATE_EQ_PRECISION));
}

void chain_multiple_hadamard_test() {
    std::vector<std::size_t> qubitPositions = {0, 1, 2};
    qce::operations::HadamardGate gate0(0, qubitPositions);
    qce::operations::HadamardGate gate1(1, qubitPositions);
    qce::operations::HadamardGate gate2(2, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 1,0,0,0,0,0,0,0).finished();

    auto result0 = gate0.applyOperation(qce::operations::OperationArgs(state));
    auto result1 = gate1.applyOperation(qce::operations::OperationArgs(result0.result));
    auto result2 = gate2.applyOperation(qce::operations::OperationArgs(result1.result));
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;

    // auto resultState = qce::utils::kroneckerProduct(qce::utils::kroneckerProduct(result0.result, result1.result), result2.result); 
    auto answer = (qce::DynamicQubitState(8) << ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2).finished();

    // std::cout << result << "\n\n";
    // std::cout << (qce::DynamicQubitState(8) << ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2).finished() << "\n";
    assert(result2.result.isApprox(answer, GATE_EQ_PRECISION));
}

int main() {
    // hadamard_gate_test();
    cnot_gate_test();
    swap_gate_test();
    cz_gate_test();
    cphase_gate_test();
    chain_multiple_hadamard_test();
}