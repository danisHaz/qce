#pragma once

#include <memory>
#include "OperationGraph.hpp"

namespace qce {
namespace simulator {

    class Solution {

    };

    class Simulator {
        

        public:
        virtual Solution construcSolution(std::weak_ptr<qce::operations::OperationGraph> graph) = 0;
    };

    class SimpleSimulator : public Simulator {
        
    };
} // simulator
} // qce