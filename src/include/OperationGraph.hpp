#pragma once
#include <functional>
#include <QubitEnv.hpp>
#include <vector>

namespace qce {
namespace operations {
    
    // utils
    template<typename result_t>
    struct OperationResultHolder {
        std::vector<unsigned> qubits;
        const result_t *result;

        OperationResultHolder(const std::vector<unsigned> &qubits, const result_t *result): result{result} {
            this->qubits = std::vector<unsigned>(qubits);
        }

        ~OperationResultHolder() {
            this->qubits.clear();
            delete this->result;
        }
    };

    template<typename data_t, typename oper_signature_t, typename oper_result_t>
    class Operation {

        // typedef std::function<oper_result_t*(oper_signature_t)> oper_t;
        
        protected:
        data_t *data = nullptr;
        // oper_t *oper;
        
        public:
        Operation(data_t *data): data{data} {}
        ~Operation() {
            delete data;
        }

        virtual oper_result_t* constructOperation() = 0;
    };

    template<typename T>
    class SingleQubitOperation : public Operation<T, unsigned, OperationResultHolder<qce::QubitMat_t>> {
        protected:
        unsigned qubitIndex;
        QubitMat_t *opMatr;

        public:
        SingleQubitOperation(T *data, unsigned qubitIndex, const QubitMat_t &opMatr): Operation<T, unsigned, OperationResultHolder<qce::QubitMat_t>>(data), qubitIndex{qubitIndex} {
            this->opMatr = new QubitMat_t(opMatr);
        }

        virtual OperationResultHolder<QubitMat_t>* constructOperation() {
            std::vector<unsigned> index = {this->qubitIndex};
            return new OperationResultHolder<QubitMat_t>(index, opMatr);
        }
    };

    template<typename data_t, typename matrix_t>
    class MultipleQubitOperation : public Operation<data_t, std::vector<unsigned>, OperationResultHolder<matrix_t>> {
        protected:
        std::vector<unsigned> qubitIndices;
        matrix_t* opMatr;

        public:
        MultipleQubitOperation(data_t *data, const std::vector<unsigned> &qubitIndices, const matrix_t &opMatr): Operation<data_t, std::vector<unsigned>, OperationResultHolder<matrix_t>>(data) {
            this->opMatr = new matrix_t(opMatr);
            this->qubitIndices = std::vector<unsigned>(qubitIndices);
        }

        virtual OperationResultHolder<matrix_t>* constructOperation() override {
            return new OperationResultHolder<matrix_t>(qubitIndices, opMatr);
        } 
    };

    // single qubit operations
    class HadamardGate : public SingleQubitOperation<void> {
        public:
        HadamardGate(void *data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, hadamard_gate) {}
    };
    class XGate : public SingleQubitOperation<void> {
        public:
        XGate(void *data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, pauli_x_gate) {}
    };
    class YGate : public SingleQubitOperation<void> {
        public:
        YGate(void *data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, pauli_y_gate) {}
    };
    class ZGate : public SingleQubitOperation<void> {
        public:
        ZGate(void *data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, pauli_z_gate) {}
    };
    class PhaseGate : public SingleQubitOperation<void> {
        public:
        PhaseGate(void *data, unsigned qubitIndex): SingleQubitOperation(data, qubitIndex, phase_s_gate) {}
    };

    // two qubit operations
    class CnotGate : public MultipleQubitOperation<void, TwoQubitMat_t> {
        public:
        CnotGate(void *data, const std::vector<unsigned> &qubits): MultipleQubitOperation<void, TwoQubitMat_t>(data, qubits, cnot_gate) {}
    };
    class SwapGate : public MultipleQubitOperation<void, TwoQubitMat_t> {
        public:
        SwapGate(void *data, const std::vector<unsigned> &qubits): MultipleQubitOperation<void, TwoQubitMat_t>(data, qubits, swap_gate) {}
    };
    class CZGate : public MultipleQubitOperation<void, TwoQubitMat_t> {
        public:
        CZGate(void *data, const std::vector<unsigned> &qubits): MultipleQubitOperation<void, TwoQubitMat_t>(data, qubits, cz_gate) {}
    };
    class CPhaseGate : public MultipleQubitOperation<void, TwoQubitMat_t> {
        public:
        CPhaseGate(void *data, const std::vector<unsigned> &qubits): MultipleQubitOperation<void, TwoQubitMat_t>(data, qubits, cphase_gate) {}
    };
    
    // three qubit operations

} // namespace operations
} // namespace qce
