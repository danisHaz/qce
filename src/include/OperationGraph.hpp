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
            for (std::size_t i = 0; i < qubits.size(); i++) {
                this->qubits.push_back(qubits[i]);
            }
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

    class HadamardGate : public Operation<void, unsigned, qce::operations::OperationResultHolder<qce::QubitMat_t>> {
        unsigned qubitIndex;
        
        public:
        HadamardGate(void *data, unsigned qubitIndex): Operation(data), qubitIndex{qubitIndex} {}

        virtual OperationResultHolder<QubitMat_t>* constructOperation();
    };

} // namespace operations
} // namespace qce
