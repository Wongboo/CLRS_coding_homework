//
// Created by 王波 on 2020/11/4.
//

#ifndef EX1_SORT_HPP
#define EX1_SORT_HPP

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#ifndef _WIN32
#define __forceinline inline __attribute__((always_inline))
#endif

namespace sort{
    //alias
    using std::vector;

    template<typename T>
    using sort_method = void(vector<T>&);

    //use to processing exporting data
    template<typename T>
    void sorting_impl(sort_method<T> method, const std::string& out, const vector<T>& in){using std::string;
        using std::fstream;
        //stupid C++ doesn't make pow constexpr, none of compiler implement string constexpr
        //so I write this instead of write a constexpr lambda, the effect is the same
        constexpr int base[][2] = {{3, 8}, {6, 64}, {9, 512}, {12, 0x1000}, {15, 0x8000}, {18, 0x40000}};
        string filename = "../output/" + out;
        fstream time_out(filename + "/time.txt", fstream::binary | fstream::trunc | fstream::out);
        //export data to csv for data analysis in excel/WPS
        fstream t("../output/output.csv", fstream::binary | fstream::app | fstream::out);
        t << out;
        for (const auto &[item, num] : base) {
            vector<T> to_sort(in.begin(), in.begin() + num);

            auto start = std::chrono::high_resolution_clock::now();
            method(to_sort);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;

            time_out << out << " for " << std::setw(6) << num << " elapsed time: "
                     << elapsed_seconds.count() << "s\n";
            t << ',' << elapsed_seconds.count();
            fstream s(filename + "/result_" + std::to_string(item) + ".txt",
                      fstream::binary | fstream::trunc | fstream::out);
            for (const auto &term : to_sort)
                s << term << '\n';
            s.close();
        }
        t << '\n';
        t.close();
    }
    //I'm too lazy to write the string myself, use a macro
#define sorting(a, b) ::sort::sorting_impl(a, std::string(#a), b)

    //wrap for std::sort... for benchmark
    template<typename T>
    inline void std_sort(vector<T>&A){
        return std::sort(A.begin(), A.end());
    }

    template<typename T>
    inline void std_stable_sort(vector<T>&A){
        return std::stable_sort(A.begin(), A.end());
    }

    template<typename T>
    [[maybe_unused]] inline void q_sort(vector<T> &A) {
        qsort(&A[0], A.size(), sizeof(T), [](const void *a, const void *b) {
            T arg1 = *static_cast<const T *>(a);
            T arg2 = *static_cast<const T *>(b);

            return arg1 > arg2 ? 1 : arg1 < arg2 ? -1 : 0;
        });
    }
}

#endif //EX1_SORT_HPP
