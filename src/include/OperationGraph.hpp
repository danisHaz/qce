#pragma once
#include <functional>
#include <QubitEnv.hpp>
#include <vector>
#include <memory>
#include <cassert>
#include <Exceptions.h>
#include <iostream>
#include "QubitVector.hpp"
#include "Utils.hpp"

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

    template<typename QubitState_t>
    class BaseOperation {
        virtual QubitState_t applyOperation(
            const std::vector<QubitState_t> &states
        ) = 0;
    };

    /**
     * Class for operation node in graph of operations.
     * Instance of this class contains information how to construct typical operation, e.g. gate.
    */
    template<typename data_t, typename oper_result_t, typename QubitState_t>
    class Operation : public BaseOperation<DynamicQubitState> {

        // typedef std::function<oper_result_t*(oper_signature_t)> oper_t;
        
        protected:
        data_t &data;
        
        public:
        Operation(data_t &data): data{data} {}
        Operation(const Operation &operation) {
            this->data = operation->data;
        }

        virtual oper_result_t constructOperation() = 0;
        virtual QubitState_t applyOperation(
            const std::vector<QubitState_t> &states
        ) = 0;
    };

    template<typename data_t, typename oper_result_t>
    class QubitOperation : public Operation<data_t, oper_result_t, DynamicQubitState> {
        
        #ifndef QUBIT_NUMBER_UNDEFINED
            #define QUBIT_NUMBER_UNDEFINED 0
        #endif

        protected:
        std::vector<unsigned> controlQubits;
        unsigned targetQubit;
        unsigned qubitNumber;

        unsigned countQubitNumber(const std::vector<DynamicQubitState> &qubitStates) {
            unsigned result = 0;
            for (const DynamicQubitState &qs: qubitStates) {
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
            const std::vector<DynamicQubitState> &states
        ) {
            DynamicQubitState result = states[0];

            for (std::size_t i = 1; i < states.size(); i++) {
                result = combineStates(result, states[i]);
            }

            return result;
        }

        public:
        std::string operationName = "QubitOperation";
        QubitOperation(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            data_t &data = nullptr,
            const std::string &operationName = ""
        ): Operation<data_t, oper_result_t, DynamicQubitState>(data), targetQubit(targetQubit) {
            this->controlQubits = std::vector<unsigned>(controlQubits);
            this->qubitNumber = QUBIT_NUMBER_UNDEFINED;
            if (operationName != "") {
                this->operationName = operationName;
            }
        }

        virtual oper_result_t constructOperation() = 0;

        DynamicQubitState applyOperation(
            const std::vector<DynamicQubitState> &states
        ) override {
            qubitNumber = countQubitNumber(states);
            oper_result_t operation = constructOperation();
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
                DynamicQubitMat_t matrix = (i == targetQubit ? singleOperationMatrix : identity);
                result = utils::kroneckerProduct(result, matrix);
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
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, hadamard_gate, "Hadamard gate") {}
    };
    class XGate : public SingleQubitOperation {
        public:
        XGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, pauli_x_gate, "X gate") {}
    };
    class YGate : public SingleQubitOperation {
        public:
        YGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, pauli_y_gate, "Y gate") {}
    };
    class ZGate : public SingleQubitOperation {
        public:
        ZGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, pauli_z_gate, "Z gate") {}
    };
    class PhaseGate : public SingleQubitOperation {
        public:
        PhaseGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(controlQubits, targetQubit, qubitOrder, phase_s_gate, "S gate") {}
    };

    // two qubit operations

    class CnotGate : public QubitOperation<const std::vector<unsigned>, OperationResultHolder<DynamicQubitMat_t>> {
        public:
        CnotGate(
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
            throw NOT_IMPLEMENTED_ERROR_CODE;
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
            throw NOT_IMPLEMENTED_ERROR_CODE;
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
            throw NOT_IMPLEMENTED_ERROR_CODE;
        }
    };
    
    // three qubit operations

    template<typename NodeData_t>
    class Node {

        std::shared_ptr<NodeData_t> data;
        std::vector<std::shared_ptr<Node<NodeData_t>>> children;

        public:
        Node(const Node &node) {
            this->children = std::vector<std::shared_ptr<Node<NodeData_t>>>(node->children);
        }

        Node(const std::vector<std::shared_ptr<Node<NodeData_t>>> &nodes) {
            this->children = std::vector<std::shared_ptr<Node<NodeData_t>>>(nodes);
        }

        Node(std::size_t childrenNodeCount, const Node<NodeData_t> &initNode) {
            this->children.reserve(childrenNodeCount);

            this->children.emplace_back(
                std::make_shared<Node<NodeData_t>>(initNode)
            );
        }

        Node(const NodeData_t& data) {
            this->data = std::make_shared<NodeData_t>(data);
        }

        std::shared_ptr<NodeData_t> getData() {
            return this->data;
        }

        bool hasNext(unsigned nextElementIndex=0) {
            if (this->children.size() <= nextElementIndex) {
                return false;
            }

            return true;
        }

        std::shared_ptr<Node<NodeData_t>> getNext(unsigned nextElementIndex=0) {
            if (!hasNext(nextElementIndex)) {
                
            }
        }
    };

    template<typename NodeData_t>
    class Graph {

        std::vector<std::shared_ptr<Node<NodeData_t>>> basicNodes, currentNodes;

        public:
        Graph(const std::vector<std::shared_ptr<Node<NodeData_t>>> &basicNodes) {
            this->basicNodes = std::vector<std::shared_ptr<Node<NodeData_t>>>(basicNodes);

        }

        Graph(std::size_t nodesCount, const Node<NodeData_t> &initNode) {
            this->basicNodes.reserve(nodesCount);

            for (std::size_t i = 0; i < nodesCount; i++) {
                this->basicNodes.emplace_back(
                    std::make_shared<Node<NodeData_t>>(initNode)
                );
            }
        }

        virtual void add() = 0;
    };

    class OperationGraph {
        std::shared_ptr<std::vector<Qubit>> initialQubits;
        std::vector<std::vector<BaseOperation<OperationResultHolder<DynamicQubitState>>>> graph;

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

        void addNode(std::shared_ptr<BaseOperation<OperationResultHolder<DynamicQubitState>>> operation, unsigned qubitIndex) {
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
