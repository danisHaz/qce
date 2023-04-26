#pragma once

#include <Eigen/Dense>
#include "Qbit.h"
#include <vector>
#include <memory>
#include <cmath>
#include "Utils.hpp"
#include <exception>

namespace qce {

    /**
     * MultipleQubitState: acts like a holder for different qubits in order to store their combined state after
     * applying different gates to them.
    */
    class MultipleQubitState {
        typedef Eigen::Matrix<std::complex<double>, Eigen::Dynamic, 1> QubitMatrix_t;
        std::shared_ptr<QubitMatrix_t> state;
        std::vector<unsigned> indices;

        public:
        MultipleQubitState(const std::vector<unsigned> &indices) {
            this->indices = std::vector<unsigned>(indices);
            std::uint64_t stateSize = utils::binpow<std::uint64_t, std::size_t>(2, indices.size());
            state = std::make_shared<QubitMatrix_t>(stateSize);
        }

        std::shared_ptr<QubitMatrix_t> getState() {
            return state;
        }
    };

    class QubitVector {
        protected:
        std::vector<MultipleQubitState&> qubits; // acts like a vector of merged qubits
 
        virtual void constructState(std::shared_ptr<MultipleQubitState> vState, std::complex<double> probability,
            unsigned qubitStatePosition, unsigned index) = 0;

        public:
        virtual std::shared_ptr<MultipleQubitState> getState() = 0;
        virtual std::shared_ptr<MultipleQubitState> getState(const std::vector<unsigned>& qubitIndices) = 0;
        virtual void add(const Qubit& q) = 0;
        virtual void clear() = 0;
    };

    class SimpleQubitVector : public QubitVector {

        void constructState(std::shared_ptr<MultipleQubitState> vState, std::complex<double> p, unsigned qubitStatePosition, unsigned index) {
            if (index + 1 == this->qubits.size()) {
                unsigned zeroPosition = (qubitStatePosition << 1);
                unsigned onePosition = (qubitStatePosition << 1) + 1;
                (*vState)[zeroPosition] = p * this->qubits[index].getState()[0];
                (*vState)[onePosition] = p * this->qubits[index].getState()[1];
                return;
            }

            constructState(vState, p * this->qubits[index].getState()[0], qubitStatePosition << 1, index+1);
            constructState(vState, p * this->qubits[index].getState()[1], (qubitStatePosition << 1) + 1, index+1);
        }

        public:

        SimpleQubitVector() {}

        SimpleQubitVector(unsigned qubitsCount, const qce::Qubit& inititalize_with) {
            qubits.reserve(qubitsCount);

            for (std::size_t i = 0; i < qubitsCount; i++) {
                qubits.emplace_back(Qubit(inititalize_with));
            }
        }

        SimpleQubitVector(const std::vector<Qubit>& qubits) {
            this->qubits = std::vector<Qubit>(qubits);
        }

        std::shared_ptr<MultipleQubitState> getState() override {
            if (qubits.size() == 0) {
                return std::make_shared<MultipleQubitState>(0);
            }

            std::shared_ptr<MultipleQubitState> qubitsState = std::make_shared<MultipleQubitState>(
                utils::binpow<std::uint64_t, unsigned>(2, qubits.size())
            );
            constructState(qubitsState, std::complex<double>(1, 0), 0, 0);

            return qubitsState;
        }

        std::shared_ptr<MultipleQubitState> getState(const std::vector<unsigned>& qubitIndices) override {
            if (qubitIndices.size() == 0) {
                return std::make_shared<MultipleQubitState>(0);
            }
            
            std::vector<Qubit> qset;

            for (std::size_t i = 0; i < qubitIndices.size(); i++) {
                if (qubitIndices[i] >= this->qubits.size()) {
                    throw std::invalid_argument("Provided index of chosen qubit is invalid");
                }

                qset.push_back(this->qubits[qubitIndices[i]]);
            }

            std::shared_ptr<MultipleQubitState> qubitsState = std::make_shared<MultipleQubitState>(
                utils::binpow<std::uint64_t, unsigned>(2, qubits.size())
            );

            throw NOT_IMPLEMENTED_ERROR_CODE;
        }

        void add(const Qubit& q) override {
            qubits.push_back(Qubit(q));
        }
        void clear() override {
            qubits.clear();
        }

    };
} // qce