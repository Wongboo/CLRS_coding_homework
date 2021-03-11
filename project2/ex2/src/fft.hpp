//
// Created by 王波 on 2020/11/25.
//

#ifndef PROJECT2_FFT_HPP
#define PROJECT2_FFT_HPP

#ifdef USE_OMP
#include <omp.h>
#endif
#include <cassert>
#include <complex>
#include <cmath>
#include <numeric>
#include <vector>
#include <type_traits>

namespace fft{
    using std::complex;
    using std::vector;

    template<typename T>
    inline auto bit_reverse_copy(const vector<T>& vec, size_t size_, size_t lg_size){
        //reverse copy implement
        //see reverse as a whole process
        vector<size_t> reverse(size_);
        size_t add_size = 1;
        size_t size = size_ >> 1u;
        reverse[1] = size;
        for (size_t i = 0; i < lg_size - 1; ++i) {
            size >>= 1u;
            add_size <<= 1u;
            for (size_t j = 0; j < add_size; ++j)
                reverse[add_size + j] = reverse[j] + size;
        }

        vector<complex<T>> out(size_);
        for (size_t i = 0; i < size_; ++i)
            out[i] = vec[reverse[i]];
        return out;
    }

    template<typename T> requires std::is_floating_point_v<T>
    //Apple clang does not have concept head file!
    auto fft_transform(vector<T> vec){
        //preparation
        const auto size = vec.size();
        size_t lg_size = log2(size);
        assert((size & (size - 1)) == 0);

        //reverse copy implement
        auto out = bit_reverse_copy(vec, size, lg_size);

        size_t m = 1;
        for (size_t i = 0; i < lg_size; ++i) {
            m *= 2;
            //calculate omega
            auto omega_ = std::exp(complex<T>(0, 2) * std::acos(-1) / T(m));
#ifdef USE_OMP
#pragma omp parallel for shared(out, omega_, size, m) default(none) num_threads(omp_get_num_procs() / 2)
#endif
            //just like the textbook
            for (int j = 0; j < size; j += m) {
                decltype(omega_) omega = 1.0;
                for (size_t k = 0; k < m / 2; ++k) {
                    auto t = omega * out[k + j + m / 2];
                    auto u = out[k + j];
                    out[k + j + m / 2] = u - t;
                    out[k + j] = u + t;
                    omega *= omega_;
                }
            }
        }
        return out;
    }
}

#endif //PROJECT2_FFT_HPP
