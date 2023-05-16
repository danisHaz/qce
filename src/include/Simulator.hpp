#pragma once

#include <memory>
#include <iostream>

#include "OperationGraph.hpp"
#include "QubitEnv.hpp"
#include "OperationArgs.hpp"

namespace qce {
namespace simulator {

    class Solution {
        DynamicQubitState result;
        public:

        Solution(DynamicQubitState &&state): result{state} {}

        const DynamicQubitState& getResult() {
            return result;
        }
    };


    template<typename T>
    class Simulator {
        
        public:
        virtual Solution constructSolution(const T &env) = 0;
    };

    class SimpleSimulator : public Simulator<QubitEnv> {
        
        public:
        Solution constructSolution(const QubitEnv &env) override {
            qce::OperGraphState args = env.provideExecutionArgs();
            DynamicQubitState curState = (DynamicQubitState(1) << 1).finished();

            for (const qce::QubitState &initState: args.getInitialStates()) {
                curState = qce::utils::kroneckerProduct(curState, initState);
            }

            for (const qce::operations::Node<qce::BaseOperationPtr_t> &node: args.getNodes()) {
                curState = (*node.getData())->applyOperation(qce::operations::OperationArgs(curState)).result;
            }

            return Solution(std::move(curState));
        }
    };
} // simulator
} // qce