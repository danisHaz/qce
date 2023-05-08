#include <cassert>
#include "OperationGraph.hpp"

void hadamard_gate_test() {

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
    std::cout << *operation1.result << "\n";
    assert((*operation1.result) * state1 == (qce::DynamicQubitState(8) << 0,0,ampl,ampl,0,0,0,0).finished());
}

int main() {
    cnot_gate_test();
}