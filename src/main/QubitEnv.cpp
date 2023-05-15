#include <algorithm>
#include <memory>

#include "QubitEnv.hpp"
#include "Exceptions.h"
#include "QubitConsts.hpp"
#include "OperationGraph.hpp"
#include "OperationArgs.hpp"

qce::QubitEnv::QubitEnv() {}

qce::QubitEnv::QubitEnv(const std::vector<qce::Qubit>& qubits) {
    graph = QubitOperationGraph(qubits.size(), qubitconsts::zero_basis_state);

    for (std::size_t i = 0; i < qubits.size(); i++) {
        graph.changeState(i, std::move(qubits[i].ketState()));
    }
}

qce::QubitEnv::QubitEnv(const Qubit& qubit) {
    graph = QubitOperationGraph(1, qubit.ketState());
}

qce::QubitEnv::QubitEnv(std::size_t qubitNumber, const QubitState &initialState) {
    graph = QubitOperationGraph(qubitNumber, QubitState(initialState));
}

void qce::QubitEnv::hadamard(unsigned qubitIndex) {
    std::vector<unsigned> currentIndices = graph.findQubitUnion(qubitIndex);
    BaseOperationPtr_t hGate = std::make_shared<operations::HadamardGate>(qubitIndex, currentIndices);

    graph.add(hGate);
}