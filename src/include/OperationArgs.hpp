#pragma once

#include "QubitConsts.hpp"
#include "Qubit.h"

namespace qce {
namespace operations {
    struct OperationArgs {
        DynamicQubitState qstate;
        std::vector<uint32_t> qubitIndices;

        OperationArgs();
        OperationArgs(const DynamicQubitState &);
        OperationArgs(DynamicQubitState &&);
        OperationArgs(const std::vector<uint32_t> &);
        OperationArgs(const DynamicQubitState &, const std::vector<uint32_t> &);
        OperationArgs(DynamicQubitState &&, std::vector<uint32_t> &&);
    };
}
} // qce