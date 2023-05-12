#include "OperationArgs.hpp"

qce::OperationArgs::OperationArgs() {
    qstate = DynamicQubitState(1);
}

qce::OperationArgs::OperationArgs(const DynamicQubitState &state) {
    this->qstate = DynamicQubitState(state);
}

qce::OperationArgs::OperationArgs(DynamicQubitState &&state) {
    this->qstate = DynamicQubitState(state);
}

qce::OperationArgs::OperationArgs(const std::vector<unsigned> &numericArgs) {
    this->numericArgs = std::vector<unsigned>(numericArgs);
}

qce::OperationArgs::OperationArgs(const DynamicQubitState &state, const std::vector<unsigned> &numericArgs) {
    this->qstate = DynamicQubitState(state);
    this->numericArgs = std::vector<unsigned>(numericArgs);
}

qce::OperationArgs::OperationArgs(DynamicQubitState &&state, std::vector<unsigned> &&numericArgs) {
    this->qstate = DynamicQubitState(state);
    this->numericArgs = std::vector<unsigned>(numericArgs);
}