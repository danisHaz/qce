#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <cassert>
#include <queue>
#include <cassert>
#include <list>

#include "Exceptions.h"
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
        const result_t result;

        OperationResultHolder(const std::vector<unsigned> &qubits, const result_t result) {
            this->qubits = std::vector<unsigned>(qubits);
            this->result = result;
        }

        OperationResultHolder(std::vector<unsigned> &&qubits, const result_t result): result{result_t(result)} {
            this->qubits = std::vector<unsigned>(qubits);
        }

        OperationResultHolder(OperationResultHolder &&other): result{result(other.result)} {
            qubits = std::vector<unsigned>(other.qubits);
        }

        ~OperationResultHolder() {
            this->qubits.clear();
        }
    };

    template<typename Data_t, typename ResultState_t>
    class BaseOperation {
        public:
        virtual ResultState_t applyOperation(const OperationArgs &args) = 0;
        virtual std::shared_ptr<Data_t> getOperationData() const = 0; 
    };

    /**
     * Class for operation node in graph of operations.
     * Instance of this class contains information how to construct typical operation, e.g. gate.
    */
    template<typename Data_t, typename OperResult_t, typename ResultState_t>
    class Operation : public BaseOperation<Data_t, ResultState_t> {

        // typedef std::function<OperResult_t*(oper_signature_t)> oper_t;
        
        protected:
        std::shared_ptr<Data_t> data;
        
        public:
        Operation(const Data_t &data): data{std::make_shared<Data_t>(data)} {}
        Operation(Data_t &&data): data{std::make_shared<Data_t>(data)} {}

        Operation(const Operation &operation): data{std::make_shared<Data_t>(*operation.data)} {}

        Operation(Operation &&operation) {
            this->data = operation.data;
            operation.data = nullptr;
        }

        virtual OperResult_t constructOperation() = 0;
        virtual ResultState_t applyOperation(const OperationArgs &args) = 0;
        std::shared_ptr<Data_t> getOperationData() const {
            return data;
        }
    };

    template<typename OperResult_t>
    class QubitOperation : public Operation<std::vector<unsigned>, OperResult_t, OperationResultHolder<DynamicQubitState>> {
        
        #ifndef QUBIT_NUMBER_UNDEFINED
            #define QUBIT_NUMBER_UNDEFINED 0
        #endif

        protected:
        std::vector<unsigned> controlQubits;
        unsigned targetQubit;
        unsigned qubitNumber;

        public:
        std::string operationName = "QubitOperation";

        QubitOperation(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder,
            const std::string &operationName = ""
        ): Operation<std::vector<unsigned>, OperResult_t, OperationResultHolder<DynamicQubitState>>(qubitOrder), targetQubit(targetQubit) {
            this->controlQubits = std::vector<unsigned>(controlQubits);
            this->qubitNumber = QUBIT_NUMBER_UNDEFINED;
            if (operationName != "") {
                this->operationName = operationName;
            }
        }

        QubitOperation(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            std::vector<unsigned> &&qubitOrder,
            const std::string &operationName = ""
        ): Operation<std::vector<unsigned>, OperResult_t, DynamicQubitState>(qubitOrder), targetQubit(targetQubit) {
            this->controlQubits = std::vector<unsigned>(controlQubits);
            this->qubitNumber = QUBIT_NUMBER_UNDEFINED;
            if (operationName != "") {
                this->operationName = operationName;
            }
        }

        virtual OperResult_t constructOperation() = 0;

        OperationResultHolder<DynamicQubitState> applyOperation(const OperationArgs &args) override {
            // qubitNumber = this->data->size();
            OperResult_t operation = constructOperation();

            return OperationResultHolder<DynamicQubitState>(
                std::move(std::vector<unsigned>(*this->data)),
                std::move(operation * args.qstate)
            );
        }
    };

    class SingleQubitOperation : public QubitOperation<DynamicQubitMat_t> {    
        const qce::QubitMat_t &singleOperationMatrix;
        public:
        SingleQubitOperation(
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder,
            const qce::QubitMat_t &singleOperationMatrix,
            const std::string &operationName
        ): QubitOperation({}, targetQubit, qubitOrder, operationName), singleOperationMatrix{singleOperationMatrix} {
            if (!controlQubits.empty()) {
                throw new std::invalid_argument("Provided control qubits to " + operationName + " gate");
            }
        }

        SingleQubitOperation(
            unsigned targetQubit,
            std::vector<unsigned> &&qubitOrder,
            const qce::QubitMat_t &singleOperationMatrix,
            const std::string &operationName
        ): QubitOperation({}, targetQubit, qubitOrder, operationName), singleOperationMatrix{singleOperationMatrix} {
            if (!controlQubits.empty()) {
                throw new std::invalid_argument("Provided control qubits to " + operationName + " gate");
            }
        }

        DynamicQubitMat_t constructOperation() override {            
            DynamicQubitMat_t result = Eigen::MatrixXcd::Identity(1, 1);
            
            for (std::size_t i = 0; i < data->size(); i++) {
                DynamicQubitMat_t matrix = (i == targetQubit ? singleOperationMatrix : qubitconsts::identity);
                result = qce::utils::kroneckerProduct(result, matrix);
            }

            return result;
        }
    };
    // single qubit operations
    class HadamardGate : public SingleQubitOperation {    
        public:
        HadamardGate(
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::hadamard_gate, "Hadamard gate") {}

        HadamardGate(
            unsigned targetQubit,
            std::vector<unsigned> &&qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::hadamard_gate, "Hadamard gate") {}
    };
    class XGate : public SingleQubitOperation {
        public:
        XGate(
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::pauli_x_gate, "X gate") {}

        XGate(
            unsigned targetQubit,
            std::vector<unsigned> &&qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::pauli_x_gate, "X gate") {}
    };
    class YGate : public SingleQubitOperation {
        public:
        YGate(
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::pauli_y_gate, "Y gate") {}

        YGate(
            unsigned targetQubit,
            std::vector<unsigned> &&qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::pauli_y_gate, "Y gate") {}
    };
    class ZGate : public SingleQubitOperation {
        public:
        ZGate(
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::pauli_z_gate, "Z gate") {}

        ZGate(
            unsigned targetQubit,
            std::vector<unsigned> &&qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::pauli_z_gate, "Z gate") {}
    };
    class PhaseGate : public SingleQubitOperation {
        public:
        PhaseGate(
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::phase_s_gate, "S gate") {}

        PhaseGate(
            unsigned targetQubit,
            std::vector<unsigned> &&qubitOrder
        ): SingleQubitOperation(targetQubit, qubitOrder, qubitconsts::phase_s_gate, "S gate") {}  
    };

    // two qubit operations

    class CnotGate : public QubitOperation<DynamicQubitMat_t> {
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
        
        DynamicQubitMat_t constructOperation() override {
            std::size_t n = data->size();
            
            // count target qubit's position in relation with qubit order
            std::size_t targetQubitRelativePosition = utils::findIndex(data->begin(), data->end(), targetQubit);
            std::size_t controlQubitRelativePosition = utils::findIndex(data->begin(), data->end(), controlQubits[0]);

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

            return result;
        }
    };
    class SwapGate : public QubitOperation<DynamicQubitMat_t> {
        public:
        SwapGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): QubitOperation(controlQubits, targetQubit, qubitOrder) {}
        
        DynamicQubitMat_t constructOperation() override {
            std::size_t n = data->size();
            
            // count target qubit's position in relation with qubit order
            std::size_t targetQubitRelativePosition = utils::findIndex(data->begin(), data->end(), targetQubit);
            std::size_t controlQubitRelativePosition = utils::findIndex(data->begin(), data->end(), controlQubits[0]);

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

            return result;
        }
    };
    class CZGate : public QubitOperation<DynamicQubitMat_t> {
        public:
        CZGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): QubitOperation(controlQubits, targetQubit, qubitOrder) {}
        
        DynamicQubitMat_t constructOperation() override {
            std::size_t n = data->size();
            
            // count target qubit's position in relation with qubit order
            std::size_t targetQubitRelativePosition = utils::findIndex(data->begin(), data->end(), targetQubit);
            std::size_t controlQubitRelativePosition = utils::findIndex(data->begin(), data->end(), controlQubits[0]);

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

            return result;
        }
    };
    class CPhaseGate : public QubitOperation<DynamicQubitMat_t> {
        public:
        CPhaseGate(
            const std::vector<unsigned> &controlQubits,
            unsigned targetQubit,
            const std::vector<unsigned> &qubitOrder
        ): QubitOperation(controlQubits, targetQubit, qubitOrder) {}
        
        DynamicQubitMat_t constructOperation() override {
            using namespace std::complex_literals;
            std::size_t n = data->size();
            
            // count target qubit's position in relation with qubit order
            std::size_t targetQubitRelativePosition = utils::findIndex(data->begin(), data->end(), targetQubit);
            std::size_t controlQubitRelativePosition = utils::findIndex(data->begin(), data->end(), controlQubits[0]);

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

            return result;
        }
    };
    
    // three qubit operations

    template<typename NodeData_t>
    class Node {

        std::shared_ptr<NodeData_t> data;

        public:
        Node(const Node &node): data{std::make_shared<NodeData_t>(*node.data)} {}

        Node(const NodeData_t& data): data{std::make_shared<NodeData_t>(data)} {}

        Node(NodeData_t &&data): data{std::make_shared<NodeData_t>(data)} {}

        Node(Node &&node) {
            data = node.data;
            node.data = nullptr;
        }

        Node& operator=(Node &&node) {
            data = node.data;
            node.data = nullptr;
            return *this;
        }

        std::shared_ptr<NodeData_t> getData() const {
            return this->data;
        }
    };

    template<typename OperationType_t, typename State_t>
    class OperationGraphHolder {
        std::vector<Node<OperationType_t>> nodes;
        std::vector<State_t> initialStates;

        public:
        OperationGraphHolder(
            const std::vector<Node<OperationType_t>> &nodes,
            const std::vector<State_t> &initialStates
        ):  nodes{std::vector<Node<OperationType_t>>(nodes)},
            initialStates{std::vector<State_t>(initialStates)} {}

        const std::vector<Node<OperationType_t>>& getNodes() const {
            return nodes;
        }

        const std::vector<State_t>& getInitialStates() const {
            return initialStates;
        }
    };

    template<typename OperationType_t, typename State_t>
    class OperationGraph {
        std::vector<Node<OperationType_t>> nodes;
        std::vector<State_t> initialStates;

        // merge-find set for qubit indices in particular states 
        std::vector<unsigned> parentIndices;
        std::vector<std::list<unsigned>> qubitGraph;

        // dsu part
        void appendQubitIndex() {
            std::size_t newIndex = parentIndices.size();
            parentIndices.push_back(newIndex);
            qubitGraph.push_back({newIndex});
        }

        unsigned getParentQubitIndex(unsigned ind) {
            return (ind == parentIndices[ind]) ? ind : getParentQubitIndex(parentIndices[ind]);
        }

        void unionQubitSets(unsigned first, unsigned second) {
            unsigned firstP = getParentQubitIndex(first);
            unsigned secondP = getParentQubitIndex(second);

            if (firstP != secondP) {
                if (utils::rrandom32() & 1) {
                    parentIndices[first] = second;
                    qubitGraph[first].insert(qubitGraph[first].end(), qubitGraph[second].begin(), qubitGraph[second].end());
                    qubitGraph[second].clear();
                } else {
                    parentIndices[second] = first;
                    qubitGraph[second].insert(qubitGraph[second].end(), qubitGraph[first].begin(), qubitGraph[first].end());
                    qubitGraph[first].clear();
                }
            }
        }
        // end dsu part

        public:        
        std::vector<unsigned> findQubitUnion(unsigned ind) {
            std::vector<unsigned> result(qubitGraph[ind].size());

            for (std::list<unsigned>::iterator it = qubitGraph[ind].begin(); it != qubitGraph[ind].end(); ++it) {
                result.emplace_back(*it);
            }

            return result;
        }

        OperationGraph() {}

        OperationGraph(const std::vector<State_t> &initialStates):
            initialStates{std::vector<State_t>(initialStates)} {}

        OperationGraph(std::vector<State_t> &&initialStates):
            initialStates{std::vector<State_t>(initialStates)} {}

        OperationGraph(const std::vector<Node<OperationType_t>> &nodes):
            nodes{std::vector<Node<OperationType_t>>(nodes)} {}

        OperationGraph(std::vector<Node<OperationType_t>> &&nodes):
            nodes{std::vector<Node<OperationType_t>>(nodes)} {}

        OperationGraph(const std::size_t qubitsCount, const State_t &state) {
            initialStates.reserve(qubitsCount);
            for (std::size_t i = 0; i < qubitsCount; i++) {
                initialStates.emplace_back(State_t(state));
            }
        }

        void add(const OperationType_t &data) {
            Node<OperationType_t> node_c(data);
            if (nodes.capacity() >= 1) {
                nodes.emplace_back(std::move(node_c));
            } else {
                nodes.push_back(std::move(node_c));
            }
        }

        void add(OperationType_t &&data) {
            Node<OperationType_t> node_m(data);
            if (nodes.capacity() >= 1) {
                nodes.emplace_back(std::move(node_m));
            } else {
                nodes.push_back(std::move(node_m));
            }
        }

        void remove(const std::size_t operationIndex) {
            if (operationIndex > nodes.size()) {
                throw std::invalid_argument("Provided invalid argument operationIndex OperationGraph");
            }

            nodes.erase(nodes.begin() + (std::ptrdiff_t)operationIndex);
        }

        void changeState(const std::size_t stateIndex, const State_t &newState) {
            assert(stateIndex < initialStates.size());
            initialStates[stateIndex] = State_t(newState);
        }

        void changeState(const std::size_t stateIndex, State_t &&newState) {
            assert(stateIndex < initialStates.size());
            initialStates[stateIndex] = State_t(newState);
        }

        const std::size_t qubitsCount() const {
            return initialStates.size();
        }

        void uniteIndices(std::vector<unsigned> indices) {
            if (indices.size() == 0) {
                return;
            }

            std::vector<unsigned> result;

            for (std::size_t i = 1; i < indices.size(); i++) {
                unionQubitSets(indices[i-1], indices[i]);
            }
        }

        OperationGraphHolder<OperationType_t, State_t> compileState() const {
            return OperationGraphHolder<OperationType_t, State_t>(nodes, initialStates);
        }

        std::size_t getStatesCount() const {
            return initialStates.size();
        }
    };
} // namespace operations
} // namespace qce