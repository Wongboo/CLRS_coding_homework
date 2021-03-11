//
// Created by 王波 on 2020/11/4.
//

#ifndef EX1_QUICK_SORT_HPP
#define EX1_QUICK_SORT_HPP

#include "insert_sort.hpp"

namespace sort{
    template<typename T>
    inline auto partition(vector<T>&A, size_t p, size_t r){
        T x = A[r];
        auto i = p - 1;
        for (size_t j = p; j <= r - 1; ++j)
            if (A[j] <= x)
                std::swap(A[++i], A[j]);
        std::swap(A[i + 1], A[r]);
        return i + 1;
    }

    //is_enhance uses insert_sort_3 for smaller data
    //looks not very effective
    template<typename T, bool is_enhanced = false>
    inline void quick_sort_impl(vector<T>&A, size_t p, size_t r){
        if (p + 1 < r + 1) {
            if (p + 1 == r){
                if (A[p] > A[r])
                    std::swap(A[p], A[r]);
                return;
            }
            if constexpr (is_enhanced)
                if (p + 16 > r)
                    return insert_sort_impl<T, true>(A, p, r);
            auto q = partition(A, p, r);
            quick_sort_impl<T, is_enhanced>(A, p, q - 1);
            quick_sort_impl<T, is_enhanced>(A, q + 1, r);
        }
    }

    //wrapper
    template<typename T>
    void quick_sort(vector<T>&A){
        quick_sort_impl(A, 0, A.size() - 1);
    }

    template<typename T>
    void quick_sort_enhance(vector<T>&A){
        quick_sort_impl<T, true>(A, 0, A.size() - 1);
    }
}

#endif //EX1_QUICK_SORT_HPP
