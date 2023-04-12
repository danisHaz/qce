#pragma once

#include <cstddef>

namespace qce {
    namespace linalg {

        template <typename dtype>
        class Matr {
            dtype** matrix;
            public:

            const std::size_t n, m;
            Matr(const std::size_t n, const std::size_t m);
            Matr(const Matr& other);
            Matr(const std::size_t n, const std::size_t m, dtype** matrix);
            ~Matr();
            const Matr& add(const Matr& other);

        };
    }
}