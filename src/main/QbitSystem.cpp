#include <iostream>
#include "Qbit.h"
#include "QubitEnv.hpp"
#include <Eigen/Dense>
#include <complex>
#include <memory>
#include "OperationGraph.hpp"
#include "QubitVector.hpp"
#include "Utils.hpp"

int main() {
    using namespace std::complex_literals;
    std::vector<unsigned> qvIndices = {0, 1};

    std::vector<unsigned> qubitPositions = {0, 1, 2};
    std::vector<unsigned> controlQubits;
    qce::operations::XGate gate(controlQubits, 0, qubitPositions);

    qce::DynamicQubitState state = (qce::DynamicQubitState(8) << 0,0,0,0,0,0,0,0).finished();

    std::cout << (*gate.constructOperation()->result) * state << "\n";

    return 0;
}