#pragma once

#include <Eigen/Dense>
#include <vector>

#include "Qubit.h"
#include "QubitConsts.hpp"
#include "OperationArgs.hpp"
#include "OperationGraph.hpp"

namespace qce {

    template<typename T>
    class AbstractEnvironment {
        virtual T provideExecutionArgs() const = 0;
    };

    typedef std::shared_ptr<
        operations::BaseOperation<
        std::vector<unsigned>,
        operations::OperationResultHolder<DynamicQubitState>
        >> BaseOperationPtr_t;

    typedef operations::OperationGraph<
        BaseOperationPtr_t,
        QubitState> QubitOperationGraph;

    typedef qce::operations::OperationGraphHolder<qce::BaseOperationPtr_t, qce::QubitState> OperGraphState;

    class QubitEnv : public AbstractEnvironment<OperGraphState> {

        private:
        QubitOperationGraph graph;

        public:
        QubitEnv();
        QubitEnv(const std::vector<Qubit>& qubits);
        QubitEnv(const Qubit& qubit);
        QubitEnv(std::size_t qubitNumber, const QubitState &initialState);

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
        void swap(unsigned firstQubitIndex, unsigned secondQubitIndex);
        /**
         * Controlled z gate
        */
        void cz(unsigned zQubitIndex, unsigned controlQubitIndex);
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

        OperGraphState provideExecutionArgs() const override;
    };
} // namespace qce
