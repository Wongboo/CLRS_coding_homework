//
// Created by 王波 on 2020/11/4.
//

#ifndef EX1_MERGE_SORT_HPP
#define EX1_MERGE_SORT_HPP

#include "insert_sort.hpp"
#ifdef USE_OMP
#include <omp.h>
#endif

namespace sort{
    template<typename T>
    inline void merge_merge(vector<T>& A, size_t start, size_t mid, size_t end, size_t len){
        vector<T> intermediate(len + 1);
        size_t first = start, second = mid + 1, inter = 0;
        while (first != mid + 1 && second != end + 1) {
            if (A[first] > A[second])
                intermediate[inter++] = A[second++];
            else
                intermediate[inter++] = A[first++];
        }
        while (first != mid + 1)
            intermediate[inter++] = A[first++];
        while (second != end + 1)
            intermediate[inter++] = A[second++];
        for (size_t i = 0; i < len + 1; ++i)
            A[i + start] = intermediate[i];
    }

    //is_enhance uses insert_sort for smaller data
    //looks effective
    template<typename T, bool is_enhanced = false>
    inline void merge_sort_impl(vector<T>& A, size_t start, size_t end, size_t len){
        if (len == 0)
            return;
        if (len == 1){
            if (A[start] > A[end])
                std::swap(A[start], A[end]);
            return;
        }
        if constexpr (is_enhanced)
            if (len < 32)
                return insert_sort_impl(A, start, end);
        auto len_ = len / 2;
#pragma omp taskgroup 
        {
#pragma omp task shared(A) untied if( len >= (1<<15u)) //NOLINT
            merge_sort_impl<T, is_enhanced>(A, start, start + len_, len_);
#pragma omp task shared(A) untied if( len >= (1<<15u)) //NOLINT
            merge_sort_impl<T, is_enhanced>(A, start + len_ + 1, end, len - len_ - 1);
#pragma omp taskyield
        }
        merge_merge(A, start, start + len_, end, len);
    }

    //wrapper
    template<typename T>
    void merge_sort(vector<T>& A) {
#pragma omp parallel //NOLINT
#pragma omp single
        merge_sort_impl(A, 0, A.size() - 1, A.size() - 1);
    }

    template<typename T>
    void merge_sort_enhance(vector<T>& A){
#pragma omp parallel //NOLINT
#pragma omp single
        merge_sort_impl<T, true>(A, 0, A.size() - 1, A.size() - 1);
    }
}

#endif //EX1_MERGE_SORT_HPP
