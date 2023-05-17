#pragma once

#include "QubitConsts.hpp"
#include "Qubit.h"

namespace qce {
namespace operations {
    struct OperationArgs {
        DynamicQubitState qstate;
        std::vector<unsigned> qubitIndices;

        OperationArgs();
        OperationArgs(const DynamicQubitState &);
        OperationArgs(DynamicQubitState &&);
        OperationArgs(const std::vector<unsigned> &);
        OperationArgs(const DynamicQubitState &, const std::vector<unsigned> &);
        OperationArgs(DynamicQubitState &&, std::vector<unsigned> &&);
    };
}
} // qce