#pragma once

#include "Qbit.h"
#include <Eigen/Dense>
#include <vector>

namespace qce {
    
    class QubitEnv {

        private:

        std::vector<Qubit> arr;

        bool checkQubitIndOutOfRange(unsigned qubitIndex);
        void applyOperationOnQubit(unsigned qubitIndex, QubitMat_t mat);
        void applyOperationOn2Qubit(unsigned fQubitIndex, unsigned sQubitIndex, TwoQubitMat_t mat);

        public:

        QubitEnv();
        QubitEnv(Qubit* qubitArr, std::size_t arrayLength);
        QubitEnv(const std::vector<Qubit>& vec);
        QubitEnv(const Qubit& qubit);
        QubitEnv(std::size_t qubitNumber, Qubit initialState = zero_basis_state);

        void hadamard(unsigned qubitIndex);
        void x(unsigned qubitIndex);
        void y(unsigned qubitIndex);
        void z(unsigned qubitIndex);
        void s(unsigned qubitIndex);
        // void t();
        void cnot(unsigned inverseQubitIndex, unsigned controlQubitIndex);
        void swap();
        void cz();
        void cswap();
        void cs();
        void toffoli();
        void cfredkin();

        void compute();

        qce::Qubit getQubit(unsigned qubitIndex) const;
    };
} // namespace qce
