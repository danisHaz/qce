#include <iostream>
#include "Qbit.h"
#include "QubitEnv.hpp"
#include <Eigen/Dense>
#include <complex>
#include <memory>
#include "OperationGraph.hpp"

int main() {
    using namespace std::complex_literals;
    qce::Qubit fqbit(qce::one_basis_state);
    qce::Qubit sqbit(qce::zero_basis_state);
    std::vector<qce::Qubit> qv = {fqbit, sqbit};
    std::vector<unsigned> qvIndices = {0, 1};

    std::unique_ptr<qce::QubitEnv> env(new qce::QubitEnv(qv));
    std::unique_ptr<qce::operations::SwapGate> o = std::make_unique<qce::operations::SwapGate>(nullptr, qvIndices);

    // std::shared_ptr<qce::operations::OperationResultHolder<qce::TwoQubitMat_t>> result = o->constructOperation();
    // std::cout << (*result->result) * fqbit.getState() << "\n";
    return 0;
}