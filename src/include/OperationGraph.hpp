#pragma once
#include <functional>
#include <QubitEnv.hpp>
#include <vector>
#include <memory>
#include <Exceptions.h>
#include "QubitVector.hpp"

namespace qce {
namespace operations {
    
    // utils
    template<typename result_t>
    struct OperationResultHolder {
        std::vector<unsigned> qubits;
        std::shared_ptr<const result_t> result;

        OperationResultHolder(const std::vector<unsigned> &qubits, std::shared_ptr<result_t> result) {
            this->qubits = std::vector<unsigned>(qubits);
            this->result = result;
        }

        ~OperationResultHolder() {
            this->qubits.clear();
        }
    };

    template<typename QubitState_t>
    class BaseOperation {
        virtual std::shared_ptr<QubitState_t> applyOperation() = 0;
    };

    /**
     * Class for operation node in graph of operations.
     * Instance of this class contains information how to construct typical operation, e.g. gate.
    */
    template<typename data_t, typename oper_result_t>
    class Operation : public BaseOperation<DynamicQubitState> {

        // typedef std::function<oper_result_t*(oper_signature_t)> oper_t;
        
        protected:
        std::shared_ptr<data_t> data;
        // oper_t *oper;
        
        public:
        Operation(std::shared_ptr<data_t> data): data{data} {}
        Operation(const Operation &operation) {
            this->data = operation->data;
        }

        virtual std::shared_ptr<oper_result_t> constructOperation() = 0;
        virtual std::shared_ptr<DynamicQubitState> applyOperation() = 0;
    };

    template<typename data_t, typename oper_result_t>
    class QubitOperation : public Operation<data_t, oper_result_t> {
        protected:
        std::vector<unsigned> controlQubits;
        unsigned qubitNumber;
        std::shared_ptr<std::vector<DynamicQubitState>> qubitStates;

        unsigned countQubitNumber(const std::vector<DynamicQubitState> &qubitStates) {
            unsigned result = 0;
            for (const DynamicQubitState &qs: qubitStates) {
                std::size_t qsSize = qs.size();

                // count bits in count number of different states in particular qubit state
                result += utils::integerLog<unsigned>(qsSize);
            }

            return result;
        }

        std::shared_ptr<DynamicQubitState> combineStates(
            const DynamicQubitState &first,
            const DynamicQubitState &second
        ) const {
            std::size_t resultSize = first.size() * second.size();
            std::shared_ptr<DynamicQubitState> result =
                std::make_shared<DynamicQubitState>(resultSize);
            
            for (std::size_t i = 0; i < first.size(); i++) {
                for (std::size_t j = 0; j < second.size(); j++) {
                    (*result)[(i<<1)+j] = first[i] * second[j];
                }
            }

            return result;
        }

        std::shared_ptr<DynamicQubitState> combineStates(
            const std::vector<DynamicQubitState> &states
        ) {
            std::shared_ptr<DynamicQubitState> result =
                std::make_shared<DynamicQubitState>(states[0]);

            for (std::size_t i = 1; i < states.size(); i++) {
                result = combineStates(*result, states[i]);
            }

            return result;
        }

        public:
        QubitOperation(
            const std::vector<DynamicQubitState> &qubitStates,
            const std::vector<unsigned> &controlQubits,
            std::shared_ptr<data_t> data = nullptr
        ): Operation<data_t, oper_result_t>(data) {
            
            this-qubitStates = std::make_shared(qubitStates);
            this-controlQubits = std::vector<unsigned>(controlQubits);
            this->qubitNumber = countQubitNumber(qubitStates);
        }

        virtual std::shared_ptr<oper_result_t> constructOperation() = 0;
        virtual std::shared_ptr<DynamicQubitState> applyOperation() = 0;
    };

    template<typename T>
    class SingleQubitOperation : public QubitOperation<T, OperationResultHolder<QubitMat_t>> {

        public:
        SingleQubitOperation(
            const std::vector<DynamicQubitState> &qubitStates,
            const std::vector<unsigned> &controlQubits,
            std::shared_ptr<T> data = nullptr
        ): QubitOperation<T, OperationResultHolder<QubitMat_t>>(qubitStates, controlQubits, data) {}

        virtual std::shared_ptr<OperationResultHolder<QubitMat_t>> constructOperation() = 0;

        std::shared_ptr<DynamicQubitState> applyOperation() override {
            std::shared_ptr<OperationResultHolder<QubitMat_t>> operation = this->constructOperation();
            unsigned resultStateSize = 1;
            for (const DynamicQubitState &qs: qubitStates) {
                resultState += qs.size();
            }
            std::unique_ptr<DynamicQubitState> resultState = std::make_unique<DynamicQubitState>()

            // QubitState tempState = QubitState(state);
            // OperationResultHolder<QubitMat_t> operation = *(constructOperation());
            // QubitState result = (*operation.result) * tempState;
            // return std::make_shared<QubitState>(result);
        }
    };

    template<typename data_t, typename matrix_t>
    class MultipleQubitOperation : public QubitOperation<data_t, std::vector<unsigned>, OperationResultHolder<matrix_t>, matrix_t> {

        public:
        MultipleQubitOperation(std::shared_ptr<data_t> data, const std::vector<unsigned> &qubitIndices, const matrix_t &opMatr):
            QubitOperation<data_t, std::vector<unsigned>, OperationResultHolder<matrix_t>, matrix_t>(data, qubitIndices, opMatr) {}

        virtual std::shared_ptr<OperationResultHolder<matrix_t>> constructOperation(
            const std::vector<DynamicQubitState> &qubitStates,
            const std::vector<unsigned> &controlQubits
        ) = 0;

        std::shared_ptr<DynamicQubitState> applyOperation(const DynamicQubitState &state) override {
            DynamicQubitState tempState = DynamicQubitState(state);
            OperationResultHolder<matrix_t> operation = *constructOperation();
            DynamicQubitState result = (*operation.result) * tempState;
            return std::make_shared<DynamicQubitState>(result);
        }
    };

    // single qubit operations
    class HadamardGate : public SingleQubitOperation<void> {
        protected:
        

        public:
        HadamardGate(std::shared_ptr<void> data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, hadamard_gate) {}
    };
    class XGate : public SingleQubitOperation<void> {
        public:
        XGate(std::shared_ptr<void> data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, pauli_x_gate) {}
    };
    class YGate : public SingleQubitOperation<void> {
        public:
        YGate(std::shared_ptr<void> data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, pauli_y_gate) {}
    };
    class ZGate : public SingleQubitOperation<void> {
        public:
        ZGate(std::shared_ptr<void> data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, pauli_z_gate) {}
    };
    class PhaseGate : public SingleQubitOperation<void> {
        public:
        PhaseGate(std::shared_ptr<void> data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, phase_s_gate) {}
    };

    // two qubit operations
    class CnotGate : public MultipleQubitOperation<void, TwoQubitMat_t> {
        public:
        CnotGate(std::shared_ptr<void> data, const std::vector<unsigned> &qubits): MultipleQubitOperation<void, TwoQubitMat_t>(data, qubits, cnot_gate) {}
    };
    class SwapGate : public MultipleQubitOperation<void, TwoQubitMat_t> {
        public:
        SwapGate(std::shared_ptr<void> data, const std::vector<unsigned> &qubits): MultipleQubitOperation<void, TwoQubitMat_t>(data, qubits, swap_gate) {}
    };
    class CZGate : public MultipleQubitOperation<void, TwoQubitMat_t> {
        public:
        CZGate(std::shared_ptr<void> data, const std::vector<unsigned> &qubits): MultipleQubitOperation<void, TwoQubitMat_t>(data, qubits, cz_gate) {}
    };
    class CPhaseGate : public MultipleQubitOperation<void, TwoQubitMat_t> {
        public:
        CPhaseGate(std::shared_ptr<void> data, const std::vector<unsigned> &qubits): MultipleQubitOperation<void, TwoQubitMat_t>(data, qubits, cphase_gate) {}
    };
    
    // three qubit operations

    class OperationGraph {
        std::shared_ptr<std::vector<Qubit>> initialQubits;
        std::vector<std::vector<BaseOperation<DynamicQubitState>>> graph;

        public:
        OperationGraph() {
            this->initialQubits = std::make_shared<std::vector<Qubit>>();
        }

        OperationGraph(unsigned initialQubitCount) {
            this->initialQubits = std::make_shared<std::vector<Qubit>>(initialQubitCount, zero_basis_state);
        }

        void clear() {
            initialQubits->clear();
        }

        template<typename data_t, typename oper_signature_t, typename oper_result_t>
        void addNode(Operation<data_t, oper_signature_t, oper_result_t> operation, unsigned qubitIndex) {
            throw NOT_IMPLEMENTED_ERROR_CODE;
        }

        void addQubit(const Qubit& q) {
            initialQubits->push_back(Qubit(q));
        }

        void addQubit(std::shared_ptr<Qubit> q) {
            initialQubits->push_back(Qubit(q));
        }
    };

} // namespace operations
} // namespace qce
