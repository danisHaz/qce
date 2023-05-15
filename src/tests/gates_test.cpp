#include <cassert>
#include "OperationGraph.hpp"

void hadamard_gate_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits;
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;
    qce::operations::HadamardGate gate(1, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,ampl,0,ampl,0).finished();

    auto result = gate.applyOperation({state});
    // std::cout << result << "\n\n";
    // std::cout << (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished() << "\n";
    assert(result.result == (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished());
}

void cnot_gate_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits = {0};
    qce::operations::CnotGate gate(controlQubits, 1, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished();

    auto operation = gate.constructOperation();
    assert(operation * state == (qce::DynamicQubitState(8) << 0,0,0,0,0,0,1,0).finished());

    controlQubits = {0};
    qubitPositions = {2, 0, 1};
    qce::operations::CnotGate gate1(controlQubits, 2, qubitPositions);
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;
    qce::DynamicQubitState state1 = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,ampl,ampl).finished();

    auto operation1 = gate1.constructOperation();
    assert(operation1 * state1 == (qce::DynamicQubitState(8) << 0,0,ampl,ampl,0,0,0,0).finished());
}

void swap_gate_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits = {0};
    qce::operations::SwapGate gate(controlQubits, 1, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished();

    auto operation = gate.constructOperation();
    assert(operation * state == (qce::DynamicQubitState(8) << 0,0,1,0,0,0,0,0).finished());

    controlQubits = {0};
    qubitPositions = {0, 1, 3};
    qce::operations::SwapGate gate1(controlQubits, 3, qubitPositions);
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;
    qce::DynamicQubitState state1 = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,ampl,ampl).finished();

    auto operation1 = gate1.constructOperation();
    // std::cout << *operation1.result << "\n";
    assert(operation1 * state1 == (qce::DynamicQubitState(8) << 0,0,0,ampl,0,0,0,ampl).finished());
}

void cz_gate_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits = {0};
    qce::operations::CZGate gate(controlQubits, 2, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0.5,0.5,0,0,0.5,0.5,0,0).finished();

    auto result = gate.applyOperation({state});
    assert(result.result == (qce::DynamicQubitState(8) << 0.5,0.5,0,0,0.5,-0.5,0,-0).finished());

    qubitPositions = {0, 1, 2, 3};
    controlQubits = {3};
    qce::operations::CZGate gate1(controlQubits, 0, qubitPositions);
    state = (qce::DynamicQubitState(16) << 0.5,0.5,0,0,0,0,0,0,0.5,0.5,0,0,0,0,0,0).finished();

    auto result1 = gate1.applyOperation({state});
    assert(result1.result == (qce::DynamicQubitState(16) << 0.5,0.5,0,0,0,0,0,0,0.5,-0.5,0,0,0,0,0,0).finished());
}

void cphase_gate_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits = {0};
    const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;
    qce::operations::CPhaseGate gate(controlQubits, 2, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2).finished();

    auto result = gate.applyOperation({state});
    assert(result.result == (qce::DynamicQubitState(8) << ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,Eigen::dcomplex(0, ampl/2),ampl/2,Eigen::dcomplex(0, ampl/2)).finished());
}

void chain_multiple_hadamard_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits;
    qce::operations::HadamardGate gate0(0, qubitPositions);
    qce::operations::HadamardGate gate1(1, qubitPositions);
    qce::operations::HadamardGate gate2(2, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(2) << 1,0).finished();

    // auto result = gate0.applyOperation({state, state, state});
    // result = gate1.applyOperation({result});
    // result = gate2.applyOperation({result});
    // const float ampl = qce::qubitconsts::_RSQRROOT_OF_2;

    // // std::cout << result << "\n\n";
    // // std::cout << (qce::DynamicQubitState(8) << ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2).finished() << "\n";
    // assert(result == (qce::DynamicQubitState(8) << ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2,ampl/2).finished());
}

int main() {
    // hadamard_gate_test();
    cnot_gate_test();
    swap_gate_test();
    cz_gate_test();
    cphase_gate_test();
    chain_multiple_hadamard_test();
}