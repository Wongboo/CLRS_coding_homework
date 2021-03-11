//
// Created by 王波 on 2020/11/25.
//

#include "fft.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace fft;

int main() {
    using T = double;
    //open all files
    fstream s("../input/2_2_input.txt", fstream::binary | fstream::in);
    fstream r("../output/result.txt", fstream::binary | fstream::out);
#ifdef USE_OMP
    fstream t("../output/time_omp.txt", fstream::binary | fstream::out);
#else
    fstream t("../output/time.txt", fstream::binary | fstream::out);
#endif
    //exception handling by return value
    if (!s.is_open() || !r.is_open() || !t.is_open()) {
        cerr << "fatal error: failed to open file" << endl;
        return 255;
    }

    for (size_t i = 0; i < 6; ++i) {
        //in
        size_t size;
        s >> size;
        vector<T> vec(size);
        for (auto &item : vec)
            s >> item;

        //timing
        auto start = chrono::high_resolution_clock::now();
        auto out = fft_transform(vec);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> period = end - start;

        //print helper
        auto print = [&](auto& r) {
            r << size << '\n';
            for (const auto &item : out)
                r << real(item) << " ";
            r << '\n';
        };

        //print to file
        print(r);
        if (i == 0)
            print(cout);
        t << "size:" << setw(3) << size << " time:" << period.count() << "s\n";
    }

    return 0;
}
