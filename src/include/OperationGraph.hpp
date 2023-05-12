#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <queue>

#include "Exceptions.h"
#include "QubitEnv.hpp"
#include "QubitVector.hpp"
#include "Utils.hpp"
#include "QubitConsts.hpp"
#include "OperationArgs.hpp"

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

        OperationResultHolder(OperationResultHolder &&other) {
            qubits = std::move(other.qubits);
            result = other.result;
        }

        ~OperationResultHolder() {
            this->qubits.clear();
        }
    };

    template<typename ResultState_t>
    class BaseOperation {
        virtual ResultState_t applyOperation(
            const std::vector<OperationArgs> &states
        ) = 0;
    };

    /**
     * Class for operation node in graph of operations.
     * Instance of this class contains information how to construct typical operation, e.g. gate.
    */
    template<typename Data_t, typename OperResult_t, typename ResultState_t>
    class Operation : public BaseOperation<DynamicQubitState> {

        // typedef std::function<OperResult_t*(oper_signature_t)> oper_t;
        
        protected:
        Data_t &data;
        
        public:
        Operation(Data_t &data): data{data} {}
        Operation(const Operation &operation) {
            this->data = operation->data;
        }

        virtual OperResult_t constructOperation() = 0;
        virtual ResultState_t applyOperation(
            const std::vector<OperationArgs> &states
        ) = 0;
    };

    template<typename Data_t, typename OperResult_t>
    class QubitOperation : public Operation<Data_t, OperResult_t, DynamicQubitState> {
        
        #ifndef QUBIT_NUMBER_UNDEFINED
            #define QUBIT_NUMBER_UNDEFINED 0
        #endif

        protected:
        std::vector<unsigned> controlQubits;
        unsigned targetQubit;
        unsigned qubitNumber;

        unsigned countQubitNumber(const std::vector<OperationArgs> &qubitStates) {
            unsigned result = 0;
            for (const OperationArgs &args: qubitStates) {
                const DynamicQubitState qs = args.qstate;
                std::size_t qsSize = qs.size();

                // count bits in count number of different states in particular qubit state
                result += utils::integerLog<unsigned>(qsSize);
            }

            return result;
        }

        DynamicQubitState combineStates(
            const DynamicQubitState &first,
            const DynamicQubitState &second
        ) const {
            std::size_t resultSize = first.size() * second.size();
            DynamicQubitState result(resultSize);
            
            for (std::size_t i = 0; i < first.size(); i++) {
                for (std::size_t j = 0; j < second.size(); j++) {
                    result[(i<<1)+j] = first[i] * second[j];
                }
            }

            return result;
        }

        DynamicQubitState combineStates(
            const std::vector<OperationArgs> &states
        ) const {
            DynamicQubitState result = states[0].qstate;

            for (std::size_t i = 1; i < states.size(); i++) {
                result = combineStates(result, states[i].qstate);
            }

            return result;
        }

        public:
        std::string operationName = "QubitOperation";
        QubitOperation(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            Data_t &data = nullptr,
            const std::string &operationName = ""
        ): Operation<Data_t, OperResult_t, DynamicQubitState>(data), targetQubit(targetQubit) {
            this->controlQubits = std::vector<unsigned>(controlQubits);
            this->qubitNumber = QUBIT_NUMBER_UNDEFINED;
            if (operationName != "") {
                this->operationName = operationName;
            }
        }

        virtual OperResult_t constructOperation() = 0;

        DynamicQubitState applyOperation(
            const std::vector<OperationArgs> &states
        ) override {
            qubitNumber = countQubitNumber(states);
            OperResult_t operation = constructOperation();
            DynamicQubitState combinedState = combineStates(states);
            return (*operation.result) * combinedState;
        }
    };

    class SingleQubitOperation : public QubitOperation<const std::vector<unsigned>, OperationResultHolder<DynamicQubitMat_t>> {    
        const qce::QubitMat_t &singleOperationMatrix;
        public:
        SingleQubitOperation(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder,
            const qce::QubitMat_t &singleOperationMatrix,
            const std::string &operationName = nullptr
        ): QubitOperation(controlQubits, targetQubit, qubitOrder, operationName), singleOperationMatrix{singleOperationMatrix} {
            if (!controlQubits.empty()) {
                throw new std::invalid_argument("Provided control qubits to " + operationName + " gate");
            }
        }

        OperationResultHolder<DynamicQubitMat_t> constructOperation() override {            
            DynamicQubitMat_t result = Eigen::MatrixXcd::Identity(1, 1);
            
            for (std::size_t i = 0; i < data.size(); i++) {
                DynamicQubitMat_t matrix = (i == targetQubit ? singleOperationMatrix : qubitconsts::identity);
                result = qce::utils::kroneckerProduct(result, matrix);
            }

            return OperationResultHolder<DynamicQubitMat_t>(
                OperationResultHolder<DynamicQubitMat_t>(data, std::make_shared<DynamicQubitMat_t>(result))
            );
        }
    };
    // single qubit operations
    class HadamardGate : public SingleQubitOperation {    
        public:
        HadamardGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, qubitconsts::hadamard_gate, "Hadamard gate") {}
    };
    class XGate : public SingleQubitOperation {
        public:
        XGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, qubitconsts::pauli_x_gate, "X gate") {}
    };
    class YGate : public SingleQubitOperation {
        public:
        YGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, qubitconsts::pauli_y_gate, "Y gate") {}
    };
    class ZGate : public SingleQubitOperation {
        public:
        ZGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, qubitconsts::pauli_z_gate, "Z gate") {}
    };
    class PhaseGate : public SingleQubitOperation {
        public:
        PhaseGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, qubitconsts::phase_s_gate, "S gate") {}
    };

    // two qubit operations

    class CnotGate : public QubitOperation<const std::vector<unsigned>, OperationResultHolder<DynamicQubitMat_t>> {
        public:
        CnotGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): QubitOperation(controlQubits, targetQubit, qubitOrder) {
            if (controlQubits.size() != 1) {
                throw new std::invalid_argument("Provided invalid control qubits to cnot gate");
            }
        }
        
        OperationResultHolder<DynamicQubitMat_t> constructOperation() override {
            std::size_t n = data.size();
            
            // count target qubit's position in relation with qubit order
            std::size_t targetQubitRelativePosition = utils::findIndex(data.begin(), data.end(), targetQubit);
            std::size_t controlQubitRelativePosition = utils::findIndex(data.begin(), data.end(), controlQubits[0]);

            // find power
            std::size_t targetQubitNumber = (1 << (n-targetQubitRelativePosition-1));
            std::size_t controlQubitNumber = (1 << (n-controlQubitRelativePosition-1));

            // implement algorithm for swapping amplitudes for target and control
            std::size_t resultMatrixSize = (1 << n);
            DynamicQubitMat_t result = Eigen::MatrixXcd::Zero(resultMatrixSize, resultMatrixSize);

            for (std::size_t i = 0; i < resultMatrixSize; i++) {
                bool controlCheck = (i & controlQubitNumber) == 0;
                bool targetCheck = (i & targetQubitNumber) == 0;

                if (controlCheck) {
                    result(i, i) = 1; continue;
                }

                if (!targetCheck) {
                    // means that this is an amplitude for |..1..1..> state
                    // change to |..1..0..> (|..0..1..>)
                    result(i, i-targetQubitNumber) = 1;
                } else {
                    result(i, i+targetQubitNumber) = 1;
                }
            }

            return OperationResultHolder<DynamicQubitMat_t>(data, std::make_shared<DynamicQubitMat_t>(result));
        }
    };
    class SwapGate : public QubitOperation<const std::vector<unsigned>, OperationResultHolder<DynamicQubitMat_t>> {
        public:
        SwapGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): QubitOperation(controlQubits, targetQubit, qubitOrder) {}
        
        OperationResultHolder<DynamicQubitMat_t> constructOperation() override {
            std::size_t n = data.size();
            
            // count target qubit's position in relation with qubit order
            std::size_t targetQubitRelativePosition = utils::findIndex(data.begin(), data.end(), targetQubit);
            std::size_t controlQubitRelativePosition = utils::findIndex(data.begin(), data.end(), controlQubits[0]);

            // find power
            std::size_t targetQubitNumber = (1 << (n-targetQubitRelativePosition-1));
            std::size_t controlQubitNumber = (1 << (n-controlQubitRelativePosition-1));

            // implement algorithm for swapping amplitudes for target and control
            std::size_t resultMatrixSize = (1 << n);
            DynamicQubitMat_t result = Eigen::MatrixXcd::Zero(resultMatrixSize, resultMatrixSize);

            for (std::size_t i = 0; i < resultMatrixSize; i++) {
                bool controlCheck = (i & controlQubitNumber) == 0;
                bool targetCheck = (i & targetQubitNumber) == 0;

                if (controlCheck == targetCheck) {
                    result(i, i) = 1; continue;
                }

                if (!targetCheck) {
                    result(i, i-targetQubitNumber+controlQubitNumber) = 1;
                } else {
                    result(i, i+targetQubitNumber-controlQubitNumber) = 1;
                }
            }

            return OperationResultHolder<DynamicQubitMat_t>(data, std::make_shared<DynamicQubitMat_t>(result));
        }
    };
    class CZGate : public QubitOperation<const std::vector<unsigned>, OperationResultHolder<DynamicQubitMat_t>> {
        public:
        CZGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): QubitOperation(controlQubits, targetQubit, qubitOrder) {}
        
        OperationResultHolder<DynamicQubitMat_t> constructOperation() override {
            std::size_t n = data.size();
            
            // count target qubit's position in relation with qubit order
            std::size_t targetQubitRelativePosition = utils::findIndex(data.begin(), data.end(), targetQubit);
            std::size_t controlQubitRelativePosition = utils::findIndex(data.begin(), data.end(), controlQubits[0]);

            // find power
            std::size_t targetQubitNumber = (1 << (n-targetQubitRelativePosition-1));
            std::size_t controlQubitNumber = (1 << (n-controlQubitRelativePosition-1));

            // implement algorithm for swapping amplitudes for target and control
            std::size_t resultMatrixSize = (1 << n);
            DynamicQubitMat_t result = Eigen::MatrixXcd::Zero(resultMatrixSize, resultMatrixSize);

            for (std::size_t i = 0; i < resultMatrixSize; i++) {
                bool controlCheck = (i & controlQubitNumber) == 0;
                bool targetCheck = (i & targetQubitNumber) == 0;

                if (controlCheck == targetCheck && !targetCheck) {
                    result(i, i) = -1;
                } else {
                    result(i, i) = 1;
                }
            }

            return OperationResultHolder<DynamicQubitMat_t>(data, std::make_shared<DynamicQubitMat_t>(result));
        }
    };
    class CPhaseGate : public QubitOperation<const std::vector<unsigned>, OperationResultHolder<DynamicQubitMat_t>> {
        public:
        CPhaseGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): QubitOperation(controlQubits, targetQubit, qubitOrder) {}
        
        OperationResultHolder<DynamicQubitMat_t> constructOperation() override {
            using namespace std::complex_literals;
            std::size_t n = data.size();
            
            // count target qubit's position in relation with qubit order
            std::size_t targetQubitRelativePosition = utils::findIndex(data.begin(), data.end(), targetQubit);
            std::size_t controlQubitRelativePosition = utils::findIndex(data.begin(), data.end(), controlQubits[0]);

            // find power
            std::size_t targetQubitNumber = (1 << (n-targetQubitRelativePosition-1));
            std::size_t controlQubitNumber = (1 << (n-controlQubitRelativePosition-1));

            // implement algorithm for swapping amplitudes for target and control
            std::size_t resultMatrixSize = (1 << n);
            DynamicQubitMat_t result = Eigen::MatrixXcd::Zero(resultMatrixSize, resultMatrixSize);

            for (std::size_t i = 0; i < resultMatrixSize; i++) {
                bool controlCheck = (i & controlQubitNumber) == 0;
                bool targetCheck = (i & targetQubitNumber) == 0;

                if (controlCheck == targetCheck && !targetCheck) {
                    result(i, i) = 1i;
                } else {
                    result(i, i) = 1;
                }
            }

            return OperationResultHolder<DynamicQubitMat_t>(data, std::make_shared<DynamicQubitMat_t>(result));
        }
    };
    
    // three qubit operations

    template<typename NodeData_t>
    class Node {

        std::shared_ptr<NodeData_t> data;

        public:
        Node(const Node &node) {
            data = std::make_shared<NodeData_t>(*node.data);
        }

        Node(const NodeData_t& data) {
            this->data = std::make_shared<NodeData_t>(data);
        }

        Node(Node &&node) {
            data = node.data;
            node.data = nullptr;
        }

        Node& operator=(Node &&node) {
            data = node.data;
            node.data = nullptr;
        }

        std::shared_ptr<NodeData_t> getData() {
            return this->data;
        }
    };

    template<typename OperationType_t>
    class OperationGraph {
        std::vector<Node<OperationType_t>> nodes;

        public:
        OperationGraph() {
            nodes = std::vector<Node<OperationType_t>>();
        }

        OperationGraph(const std::vector<Node<OperationType_t>> &nodes) {
            this->nodes = std::vector<Node<OperationType_t>>(nodes);
        }

        OperationGraph(std::vector<Node<OperationType_t>> &&nodes) {
            nodes = std::vector<Node<OperationType_t>>(nodes);
        }

        OperationGraph(const std::size_t nodesCount) {
            nodes.reserve(nodesCount);
        }

        void add(const Node<OperationType_t> &node) {
            Node<OperationType_t> node_c(node);
            if (nodes.capacity() >= 1) {
                nodes.emplace_back(node_c);
            } else {
                nodes.push_back(node_c);
            }
        }

        void add(Node<OperationType_t> &&node) {
            Node<OperationType_t> node_m(node);
            if (nodes.capacity() >= 1) {
                nodes.emplace_back(node_m);
            } else {
                nodes.push_back(node_m);
            }
        }

        void remove(std::size_t operationIndex) {
            if (operationIndex > nodes.size()) {
                throw std::invalid_argument("Provided invalid argument operationIndex OperationGraph");
            }

            nodes.erase(nodes.begin() + (std::ptrdiff_t)operationIndex);
        }
    };
} // namespace operations
} // namespace qce
