#include "Utils.hpp"

float utils::quicksqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5;

    x2 = number * 0.5;
    y  = number;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );

    return y;
}