#include "Utils.hpp"

Eigen::MatrixXcd qce::utils::kroneckerProduct(
    const Eigen::MatrixXcd &first,
    const Eigen::MatrixXcd &second
) {
    const std::size_t resultRows = first.rows() * second.rows();
    const std::size_t resultCols = first.cols() * second.cols();

    Eigen::MatrixXcd result(resultRows, resultCols);
    for (unsigned i = 0; i < first.rows(); i++) {
        for (unsigned j = 0; j < first.cols(); j++) {
            result.block(i*second.rows(), j*second.cols(), second.rows(), second.cols()) = first(i,j) * second;
        }
    }

    return result;
}