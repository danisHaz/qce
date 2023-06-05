#pragma once

#include <memory>
#include <Eigen/Dense>
#include <random>
#include <vector>

#include "Qubit.h"

namespace qce { 
namespace utils {

    #ifndef _rrand_
    #define _rrand_

    #define _rrand32u_func rand
    #define _probability_precision ((double)1e-5)
    #endif

    template<typename base_t, typename power_t>
    base_t binpow(base_t x, power_t y) {
        if (y == 0) { return (base_t)1; }

        if (y == 1) { return x; }

        if (y % 2 == 1) {
            return binpow(x, y-1) * x;
        } else {
            base_t q = binpow(x, y/2);
            return q * q;
        }
    }

    template<typename T>
    const T integerLog(T n) {
        int count = 0;
        while (n > 0) {
            n >>= 1;
            ++count;
        }

        return (T)count;
    }

    Eigen::MatrixXcd kroneckerProduct(
        const Eigen::MatrixXcd &first,
        const Eigen::MatrixXcd &second
    );

    /**
     * Function for finding index number of value in iterator range. 
    */
    template<typename Iter>
    std::size_t findIndex(Iter begin, Iter end, typename std::iterator_traits<Iter>::value_type &value) {
        std::size_t ind = 0;
        for (;begin != end && *begin != value; begin++, ind++);
        return ind;
    }

    int32_t rrandom32();
    int64_t rrandom64();
    uint32_t rrandom32u();
    uint64_t rrandom64u();

    template<typename T>
    std::vector<T> mergeVectors(const std::vector<T> &a, const std::vector<T> &b) {
        std::vector<T> result;
        result.reserver(a.size() + b.size());
        result.insert(result.begin(), a.begin(), a.end());
        result.insert(result.begin()+(ptrdiff_t)a.size(), b.begin(), b.end());

        return std::move(result);
    }

    qce::DynamicQubitState combineStates(
        const qce::DynamicQubitState &first,
        const qce::DynamicQubitState &second
    );

    qce::DynamicQubitState combineStates(
        const std::vector<qce::DynamicQubitState> &states
    );
    /**
     * Function for picking random number in specified range  with given probabilities
     * of every possible outcome.
    */
    unsigned probabilityRandomChoice(Eigen::Matrix<double, -1, 1> probabilities);
    Eigen::Matrix<double, -1, 1> convertAmplitudes2Probs(qce::DynamicQubitState qState);

} // utils
} // qce