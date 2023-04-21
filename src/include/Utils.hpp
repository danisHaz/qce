#pragma once

namespace utils {

    /**
     * Fast square root algorithm (Quake III)
    */
    float quicksqrt(float number);

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
} // utils