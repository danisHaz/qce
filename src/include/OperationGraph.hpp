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

    template<typename vector_t>
    class BaseOperation {
        virtual std::shared_ptr<vector_t> applyOperation(const vector_t &state) = 0;
    };

    /**
     * Class for operation node in graph of operations.
     * Instance of this class contains information how to construct typical operation, e.g. gate.
    */
    template<typename data_t, typename oper_signature_t, typename oper_result_t>
    class Operation : public BaseOperation<DynamicQubitState> {

        // typedef std::function<oper_result_t*(oper_signature_t)> oper_t;
        
        protected:
        std::shared_ptr<data_t> data;
        // oper_t *oper;
        
        public:
        Operation(std::shared_ptr<data_t> data): data{data} {}
        Operation(const Operation &operation) {
            *this->data = *operation->data;
        }

        virtual std::shared_ptr<oper_result_t> constructOperation() = 0;
    };

    template<typename data_t, typename oper_signature_t, typename oper_result_t, typename matrix_t>
    class QubitOperation : public Operation<data_t, oper_signature_t, oper_result_t> {
        protected:
        oper_signature_t qubitInds;
        std::shared_ptr<matrix_t> opMatr;

        public:
        QubitOperation(std::shared_ptr<data_t> data, const oper_signature_t &qubitInds, const matrix_t &opMatr):
            Operation<data_t, oper_signature_t, oper_result_t>(data) {
            
            this->qubitInds = oper_signature_t(qubitInds);
            this->opMatr = std::make_shared<matrix_t>(opMatr);
        }

    };

    template<typename T>
    class SingleQubitOperation : public QubitOperation<T, unsigned, OperationResultHolder<qce::QubitMat_t>, qce::QubitMat_t> {

        public:
        SingleQubitOperation(std::shared_ptr<T> data, unsigned qubitIndex, const QubitMat_t &opMatr):
            QubitOperation<T, unsigned, OperationResultHolder<qce::QubitMat_t>, qce::QubitMat_t>(data, qubitIndex, opMatr) {}

        std::shared_ptr<OperationResultHolder<QubitMat_t>> constructOperation() override {
            std::vector<unsigned> index = {this->qubitInds};
            return std::make_shared<OperationResultHolder<QubitMat_t>>(index, this->opMatr);
        }

        std::shared_ptr<QubitState> applyOperation(const QubitState &state) {
            QubitState tempState = QubitState(state);
            OperationResultHolder<QubitMat_t> operation = *constructOperation();
            QubitState result = (*operation.result) * tempState;
            return std::make_shared<QubitState>(result);
        }
    };

    template<typename data_t, typename matrix_t>
    class MultipleQubitOperation : public QubitOperation<data_t, std::vector<unsigned>, OperationResultHolder<matrix_t>, matrix_t> {

        public:
        MultipleQubitOperation(std::shared_ptr<data_t> data, const std::vector<unsigned> &qubitIndices, const matrix_t &opMatr):
            QubitOperation<data_t, std::vector<unsigned>, OperationResultHolder<matrix_t>, matrix_t>(data, qubitIndices, opMatr) {}

        std::shared_ptr<OperationResultHolder<matrix_t>> constructOperation() override {
            return std::make_shared<OperationResultHolder<matrix_t>>(this->qubitInds, this->opMatr);
        }

        std::shared_ptr<DynamicQubitState> applyOperation(const DynamicQubitState &state) {
            DynamicQubitState tempState = DynamicQubitState(state);
            OperationResultHolder<matrix_t> operation = *constructOperation();
            DynamicQubitState result = (*operation.result) * tempState;
            return std::make_shared<DynamicQubitState>(result);
        }
    };

    // single qubit operations
    class HadamardGate : public SingleQubitOperation<void> {
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
