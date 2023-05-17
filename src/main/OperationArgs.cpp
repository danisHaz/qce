#include "OperationArgs.hpp"

qce::operations::OperationArgs::OperationArgs(): qstate{(DynamicQubitState(1) << 1).finished()} {}

qce::operations::OperationArgs::OperationArgs(const DynamicQubitState &state): qstate{DynamicQubitState(state)} {}

qce::operations::OperationArgs::OperationArgs(DynamicQubitState &&state): qstate{DynamicQubitState(state)} {}

qce::operations::OperationArgs::OperationArgs(const std::vector<unsigned> &qubitIndices):
    qubitIndices{std::vector<unsigned>(qubitIndices)} {}

qce::operations::OperationArgs::OperationArgs(const DynamicQubitState &state, const std::vector<unsigned> &qubitIndices):
    qstate{DynamicQubitState(state)},
    qubitIndices{std::vector<unsigned>(qubitIndices)} {}


qce::operations::OperationArgs::OperationArgs(DynamicQubitState &&state, std::vector<unsigned> &&qubitIndices):
    qstate{DynamicQubitState(state)},
    qubitIndices{std::vector<unsigned>(qubitIndices)} {}