#pragma once

#include <memory>
#include <Eigen/Eigen>

namespace qce { 
namespace utils {

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
} // utils
} // qce