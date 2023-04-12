#pragma once
#include <functional>
#include <QubitEnv.hpp>
#include <vector>
#include <memory>

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

    template<typename data_t, typename oper_signature_t, typename oper_result_t>
    class Operation {

        // typedef std::function<oper_result_t*(oper_signature_t)> oper_t;
        
        protected:
        std::shared_ptr<data_t> data;
        // oper_t *oper;
        
        public:
        Operation(std::shared_ptr<data_t> data): data{data} {}

        virtual std::shared_ptr<oper_result_t> constructOperation() = 0;
    };

    template<typename T>
    class SingleQubitOperation : public Operation<T, unsigned, OperationResultHolder<qce::QubitMat_t>> {
        protected:
        unsigned qubitIndex;
        std::shared_ptr<QubitMat_t> opMatr;

        public:
        SingleQubitOperation(std::shared_ptr<T> data, unsigned qubitIndex, const QubitMat_t &opMatr):
            Operation<T, unsigned, OperationResultHolder<qce::QubitMat_t>>(data), qubitIndex{qubitIndex} {
            this->opMatr = std::make_shared<QubitMat_t>(opMatr);
        }

        virtual std::shared_ptr<OperationResultHolder<QubitMat_t>> constructOperation() {
            std::vector<unsigned> index = {this->qubitIndex};
            return std::make_shared<OperationResultHolder<QubitMat_t>>(index, opMatr);
        }
    };

    template<typename data_t, typename matrix_t>
    class MultipleQubitOperation : public Operation<data_t, std::vector<unsigned>, OperationResultHolder<matrix_t>> {
        protected:
        std::vector<unsigned> qubitIndices;
        std::shared_ptr<matrix_t> opMatr;

        public:
        MultipleQubitOperation(std::shared_ptr<data_t> data, const std::vector<unsigned> &qubitIndices, const matrix_t &opMatr): Operation<data_t, std::vector<unsigned>, OperationResultHolder<matrix_t>>(data) {
            this->opMatr = std::make_shared<matrix_t>(opMatr);
            this->qubitIndices = std::vector<unsigned>(qubitIndices);
        }

        virtual std::shared_ptr<OperationResultHolder<matrix_t>> constructOperation() override {
            return std::make_shared<OperationResultHolder<matrix_t>>(qubitIndices, opMatr);
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

} // namespace operations
} // namespace qce
