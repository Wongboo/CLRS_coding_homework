//
// Created by 王波 on 2020/11/4.
//

#ifndef EX1_INSERT_SORT_HPP
#define EX1_INSERT_SORT_HPP

#include "sort.hpp"

namespace sort{
    //insert_sort_3 is a terrible idea
    //it is the way that libc++ uses to accelarate std::sort
    //anyway, do not look at it
    template<typename T>
    inline void insert_sort_3(vector<T>& A, size_t start){
        if (A[start] < A[start + 1]){
            if (A[start + 1] < A[start + 2])
                return;
            std::swap(A[start + 1], A[start + 2]);
            if (A[start] > A[start + 1])
                std::swap(A[start], A[start + 1]);
            return;
        }
        if (A[start + 1] > A[start + 2]){
            std::swap(A[start + 1], A[start + 2]);
            return;
        }
        std::swap(A[start], A[start + 1]);
        if (A[start + 1] > A[start + 2])
            std::swap(A[start + 1], A[start + 2]);
    }

    //if is_enhanced, using insert_sort_3, for first 3 value
    //I still don't know why libc++ implement this way
    template<typename T, bool is_enhanced = false>
    inline void insert_sort_impl(vector<T>& A, size_t start, size_t end){
        if constexpr (is_enhanced)
            insert_sort_3(A, start);
        for (size_t i = is_enhanced ? 3 : 1; i <= end - start; ++i) {
            T key = A[start + i];
            size_t j;
            for (j = i - 1; j < i && key < A[start + j]; --j)
                A[start + j + 1] = A[start + j];
            A[start + j + 1] = key;
        }
    }

    //wrapper
    template<typename T>
    void insert_sort(vector<T>& A){
        insert_sort_impl(A, 0, A.size() - 1);
    }

    template<typename T>
    [[maybe_unused]] void insert_sort_enhance(vector<T>& A){
        insert_sort_impl<T, true>(A, 0, A.size() - 1);
    }}
#endif //EX1_INSERT_SORT_HPP
