#include <algorithm>
#include <memory>

#include "QubitEnv.hpp"
#include "Exceptions.h"
#include "QubitConsts.hpp"
#include "OperationGraph.hpp"

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

qce::OperGraphState qce::QubitEnv::provideExecutionArgs() const {
    return graph.compileState();
}

std::vector<unsigned> GENERATE_INDICES_BASE(const qce::QubitOperationGraph &g) {
    std::size_t cap = g.getStatesCount();
    std::vector<unsigned> currentIndices(cap);
    for (std::size_t i = 0; i < cap; i++) {
        currentIndices[i] = i;
    }

    return currentIndices;
}

#ifndef _INDICES_GENERATOR_
    #define GENERATE_INDICES_FUNC GENERATE_INDICES_BASE
#endif

void qce::QubitEnv::hadamard(unsigned qubitIndex) {
    std::vector<unsigned> indices = GENERATE_INDICES_FUNC(graph);
    BaseOperationPtr_t hGate = std::make_shared<operations::HadamardGate>(qubitIndex, indices);
    graph.add(hGate);
}

void qce::QubitEnv::x(unsigned qubitIndex) {
    std::vector<unsigned> indices = GENERATE_INDICES_FUNC(graph);
    BaseOperationPtr_t xGate = std::make_shared<operations::XGate>(qubitIndex, indices);
    graph.add(xGate);
}

void qce::QubitEnv::y(unsigned qubitIndex) {
    std::vector<unsigned> indices = GENERATE_INDICES_FUNC(graph);
    BaseOperationPtr_t yGate = std::make_shared<operations::YGate>(qubitIndex, indices);
    graph.add(yGate);
}

void qce::QubitEnv::z(unsigned qubitIndex) {
    std::vector<unsigned> indices = GENERATE_INDICES_FUNC(graph);
    BaseOperationPtr_t zGate = std::make_shared<operations::ZGate>(qubitIndex, indices);
    graph.add(zGate);
}

void qce::QubitEnv::s(unsigned qubitIndex) {
    std::vector<unsigned> indices = GENERATE_INDICES_FUNC(graph);
    BaseOperationPtr_t sGate = std::make_shared<operations::PhaseGate>(qubitIndex, indices);
    graph.add(sGate);
}

void qce::QubitEnv::cnot(unsigned inverseQubitIndex, unsigned controlQubitIndex) {
    std::vector<unsigned> indices = GENERATE_INDICES_FUNC(graph);
    std::vector<unsigned> controls = {controlQubitIndex};
    BaseOperationPtr_t cnotGate =
        std::make_shared<operations::CnotGate>(controls, inverseQubitIndex, indices);
    graph.add(cnotGate);
}

void qce::QubitEnv::swap(unsigned firstQubitIndex, unsigned secondQubitIndex) {
    std::vector<unsigned> indices = GENERATE_INDICES_FUNC(graph);
    std::vector<unsigned> controls = {secondQubitIndex};
    BaseOperationPtr_t swapGate =
        std::make_shared<operations::SwapGate>(controls, firstQubitIndex, indices);
    graph.add(swapGate);
}

void qce::QubitEnv::cz(unsigned zQubitIndex, unsigned controlQubitIndex) {
    std::vector<unsigned> indices = GENERATE_INDICES_FUNC(graph);
    std::vector<unsigned> controls = {controlQubitIndex};
    BaseOperationPtr_t czGate = std::make_shared<operations::CZGate>(controls, zQubitIndex, indices);
    graph.add(czGate);
}

void qce::QubitEnv::cs(unsigned qubitIndex, unsigned controlQubitIndex) {
    std::vector<unsigned> indices = GENERATE_INDICES_FUNC(graph);
    std::vector<unsigned> controls = {controlQubitIndex};
    BaseOperationPtr_t csGate = std::make_shared<operations::CPhaseGate>(controls, qubitIndex, indices);
    graph.add(csGate);
}