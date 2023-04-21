#pragma once

#include <Eigen/Dense>
#include "Qbit.h"
#include <vector>
#include <memory>
#include <cmath>
#include "Utils.hpp"

namespace qce {

    class QubitVector {
        typedef Eigen::Matrix<std::complex<double>, 1, Eigen::Dynamic> MultipleQubitState;

        std::vector<Qubit> qubits;

        void constructState(std::shared_ptr<MultipleQubitState> vState, std::complex<double> p, unsigned qubitStatePosition, unsigned index) {
            if (index + 1 == qubits.size()) {
                unsigned zeroPosition = (qubitStatePosition << 1);
                unsigned onePosition = (qubitStatePosition << 1) + 1;
                (*vState)[zeroPosition] = p * qubits[index].getState()[0];
                (*vState)[onePosition] = p * qubits[index].getState()[1];
                return;
            }

            constructState(vState, p * qubits[index].getState()[0], qubitStatePosition << 1, index+1);
            constructState(vState, p * qubits[index].getState()[1], (qubitStatePosition << 1) + 1, index+1);
        }

        public:

        QubitVector() {}

        QubitVector(unsigned qubitsCount, const qce::Qubit& inititalize_with) {
            qubits.reserve(qubitsCount);

            for (std::size_t i = 0; i < qubitsCount; i++) {
                qubits.emplace_back(Qubit(inititalize_with));
            }
        }

        QubitVector(const std::vector<Qubit>& qubits) {
            this->qubits = std::vector<Qubit>(qubits);
        }

        std::shared_ptr<MultipleQubitState> getState() {
            if (qubits.size() == 0) {
                return std::make_shared<MultipleQubitState>(0);
            }

            std::shared_ptr<MultipleQubitState> qubitsState = std::make_shared<MultipleQubitState>(
                utils::binpow<std::uint64_t, unsigned>(2, qubits.size())
            );
            constructState(qubitsState, std::complex<double>(1, 0), 0, 0);

            return qubitsState;
        }
        void add(const Qubit& q) {
            qubits.push_back(Qubit(q));
        }
        void clear() {
            qubits.clear();
        }

    };
} // qce