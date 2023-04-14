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
        QubitEnv(const std::vector<Qubit>& vec);
        QubitEnv(const Qubit& qubit);
        QubitEnv(std::size_t qubitNumber, Qubit initialState = zero_basis_state);

        // Section with common gates

        /**
         * Default Hadamard gate
        */
        void hadamard(unsigned qubitIndex);

        /**
         * Sigma_x gate
        */
        void x(unsigned qubitIndex);
        /**
         * Sigma_y gate
        */
        void y(unsigned qubitIndex);
        /**
         * Sigma_z gate
        */
        void z(unsigned qubitIndex);
        /**
         * S (phase) gate
        */
        void s(unsigned qubitIndex);
        // void t();
        /**
         * Controlled NOT gate
        */
        void cnot(unsigned inverseQubitIndex, unsigned controlQubitIndex);
        /**
         * Swap gate
        */
        void swap();
        /**
         * Controlled z gate
        */
        void cz();
        /**
         * Controlled swap gate
        */
        void cswap();
        /**
         * Controlled phase gate
        */
        void cs();
        /**
         * Toffoli gate
        */
        void toffoli();
        /**
         * Controlled Fredkin gate
        */
        void cfredkin();

        // Common gates section end

        void compute();
        

        qce::Qubit getQubit(unsigned qubitIndex) const;
    };
} // namespace qce
