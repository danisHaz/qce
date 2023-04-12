#include "OperationGraph.hpp"

using namespace qce::operations;

OperationResultHolder<qce::QubitMat_t>* HadamardGate::constructOperation() {
    std::vector<unsigned> index;
    index.push_back(this->qubitIndex);
    QubitMat_t *h_gate_cpy = new QubitMat_t(hadamard_gate);
    OperationResultHolder<QubitMat_t> *holder = new OperationResultHolder<QubitMat_t>(index, h_gate_cpy);
    return holder;
}