#include <cassert>
#include "OperationGraph.hpp"

void hadamard_gate_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits;
    const float ampl = qce::_RSQRROOT_OF_2;
    qce::operations::HadamardGate gate(controlQubits, 1, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,ampl,0,ampl,0).finished();

    auto result = gate.applyOperation({state});
    // std::cout << result << "\n\n";
    // std::cout << (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished() << "\n";
    assert(result == (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished());
}

void cnot_gate_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits = {0};
    qce::operations::CnotGate gate(controlQubits, 1, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished();

    auto operation = gate.constructOperation();
    assert((*operation.result) * state == (qce::DynamicQubitState(8) << 0,0,0,0,0,0,1,0).finished());

    controlQubits = {0};
    qubitPositions = {2, 0, 1};
    qce::operations::CnotGate gate1(controlQubits, 2, qubitPositions);
    const float ampl = qce::_RSQRROOT_OF_2;
    qce::DynamicQubitState state1 = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,ampl,ampl).finished();

    auto operation1 = gate1.constructOperation();
    assert((*operation1.result) * state1 == (qce::DynamicQubitState(8) << 0,0,ampl,ampl,0,0,0,0).finished());
}

void swap_gate_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits = {0};
    qce::operations::SwapGate gate(controlQubits, 1, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished();

    auto operation = gate.constructOperation();
    assert((*operation.result) * state == (qce::DynamicQubitState(8) << 0,0,1,0,0,0,0,0).finished());

    controlQubits = {0};
    qubitPositions = {0, 1, 3};
    qce::operations::SwapGate gate1(controlQubits, 3, qubitPositions);
    const float ampl = qce::_RSQRROOT_OF_2;
    qce::DynamicQubitState state1 = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,ampl,ampl).finished();

    auto operation1 = gate1.constructOperation();
    // std::cout << *operation1.result << "\n";
    assert((*operation1.result) * state1 == (qce::DynamicQubitState(8) << 0,0,0,ampl,0,0,0,ampl).finished());
}

void cz_gate_test() {
    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits = {0};
    qce::operations::CZGate gate(controlQubits, 2, qubitPositions);
    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0.5,0.5,0,0,0.5,0.5,0,0).finished();

    auto result = gate.applyOperation({state});
    assert(result == (qce::DynamicQubitState(8) << 0.5,0.5,0,0,0.5,-0.5,0,-0).finished());

    qubitPositions = {0, 1, 2, 3};
    controlQubits = {3};
    qce::operations::CZGate gate1(controlQubits, 0, qubitPositions);
    state = (qce::DynamicQubitState(16) << 0.5,0.5,0,0,0,0,0,0,0.5,0.5,0,0,0,0,0,0).finished();

    result = gate1.applyOperation({state});
    assert(result == (qce::DynamicQubitState(16) << 0.5,0.5,0,0,0,0,0,0,0.5,-0.5,0,0,0,0,0,0).finished());
}

int main() {
    // hadamard_gate_test();
    cnot_gate_test();
    swap_gate_test();
    cz_gate_test();
}