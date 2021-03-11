//
// Created by 王波 on 2020/11/4.
//

#ifndef EX1_COUNT_SORT_HPP
#define EX1_COUNT_SORT_HPP

#include <array>
#include "sort.hpp"

namespace sort{
    //why write so strange, because we do not only write for 2^15-1
    //no need to storage b, at this time
    //when use for MSVC, make your stack larger!
    template<typename T, size_t k>
    __forceinline void count_sort_impl(vector<T>&A, size_t n){
        std::array<size_t, k + 1> C;
        C.fill(T(0));
        vector<T> B(n + 1);
        for (const auto &item : A)
            ++C[item];
        for (size_t i = 0; i < k; ++i)
            C[i + 1] += C[i];
        for (size_t j = n; j < n + 1; --j)
            B[--C[A[j]]] = A[j];
        A = std::move(B);
    }

    template<typename T>
    void count_sort(vector<T>&A) {
        count_sort_impl<T, 0x40000>(A, A.size() - 1);
    }
}

#endif //EX1_COUNT_SORT_HPP
