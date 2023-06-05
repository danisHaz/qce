#include <iostream>
#include "Qubit.h"
#include "QubitEnv.hpp"
#include <Eigen/Dense>
#include <complex>
#include <memory>
#include "OperationGraph.hpp"
#include "QubitVector.hpp"
#include "Utils.hpp"

int main() {
    using namespace std::complex_literals;
    std::vector<std::size_t> qvIndices = {0, 1};

    std::vector<std::size_t> qubitPositions = {0, 1, 2};
    std::vector<std::size_t> controlQubits = {0};
    qce::operations::CnotGate gate(controlQubits, 1, qubitPositions);
    qce::operations::HadamardGate hadamardGate(1, {0, 1, 2});

    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,1,0,0,0).finished();

    auto operation = gate.constructOperation();
    std::cout << operation << "\n";

    std::cout << operation * state << "\n";

    return 0;
}