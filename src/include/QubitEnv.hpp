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
        std::vector<std::size_t>,
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
        void hadamard(std::size_t qubitIndex);

        /**
         * Sigma_x gate
        */
        void x(std::size_t qubitIndex);
        /**
         * Sigma_y gate
        */
        void y(std::size_t qubitIndex);
        /**
         * Sigma_z gate
        */
        void z(std::size_t qubitIndex);
        /**
         * S (phase) gate
        */
        void s(std::size_t qubitIndex);
        // void t();
        /**
         * Controlled NOT gate
        */
        void cnot(std::size_t inverseQubitIndex, std::size_t controlQubitIndex);
        /**
         * Swap gate
        */
        void swap(std::size_t firstQubitIndex, std::size_t secondQubitIndex);
        /**
         * Controlled z gate
        */
        void cz(std::size_t zQubitIndex, std::size_t controlQubitIndex);
        /**
         * Controlled phase gate
        */
        void cs(std::size_t qubitIndex, std::size_t controlQubitIndex);
        /**
         * Toffoli gate
        */
        void toffoli();
        /**
         * Controlled Fredkin gate
        */
        void cfredkin();

        // Common gates section end

        std::size_t compute();
        

        qce::Qubit getQubit(std::size_t qubitIndex) const;
        std::size_t getQubitCount() const;

        OperGraphState provideExecutionArgs() const override;
    };
} // namespace qce
