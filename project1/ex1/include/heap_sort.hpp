//
// Created by 王波 on 2020/11/4.
//

#ifndef EX1_HEAP_SORT_HPP
#define EX1_HEAP_SORT_HPP

#include "sort.hpp"

namespace sort{
    template<typename T>
    void max_heapify(vector<T>& A, size_t heap_size, size_t i){
        const auto l = 2 * i + 1;
        const auto r = 2 * i + 2;
        size_t largest;
        if (l < heap_size && A[l] > A[i])
            largest = l;
        else
            largest = i;
        if (r < heap_size && A[r] > A[largest])
            largest = r;
        if (largest != i) {
            std::swap(A[i], A[largest]);
            max_heapify(A, heap_size, largest);
        }
    }

    template<typename T>
    void heap_sort(vector<T>& A){
        for (size_t i = A.size() / 2; i > 0; --i)
            max_heapify(A, A.size(), i - 1);
        auto heap_size = A.size();
        for (size_t i = A.size(); i > 1; --i) {
            std::swap(A[0], A[i - 1]);
            max_heapify(A, --heap_size, 0);
        }
    }

}

#endif //EX1_HEAP_SORT_HPP
