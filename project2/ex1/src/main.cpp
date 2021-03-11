//
// Created by 王波 on 2020/11/25.
//

#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <type_traits>
#include <vector>

using namespace std;

template<typename T> requires is_integral_v<T>
//Apple clang does not have concept head file!
auto matrix_chain_order(vector<T> vec){
    //just like the textbook
    const auto n = vec.size() - 1;
    vector<vector<T>> m(n, vector<T>(n));
    vector<vector<size_t>> s(n - 1, vector<size_t>(n- 1));
    for (size_t l = 1; l < n; ++l)
        for (size_t i = 0; i < n - l; ++i) {
            auto j = i + l;
            m[i][j] = numeric_limits<T>::max();
            for (size_t k = i; k < j; ++k) {
                auto q = m[i][k] + m[k + 1][j] + vec[i] * vec[k + 1] * vec[j + 1];
                if (q < m[i][j]){
                    m[i][j] = q;
                    s[i][j - 1] = k;
                }
            }
        }
    return make_pair(m, s);
}

void print_optimal_parens(const vector<vector<size_t>>& s, fstream &r, size_t i, size_t j){
    if (i == j)
        r << 'A' << i + 1;
    else {
        r << "(";
        print_optimal_parens(s, r, i, s[i][j - 1]);
        print_optimal_parens(s, r, s[i][j - 1] + 1, j);
        r << ")";
    }
}

int main(){
    using T = long long;
    //open all files
    fstream s("../input/2_1_input.txt", fstream::binary | fstream::in);
    fstream r("../output/result.txt", fstream::binary | fstream::out);
    fstream t("../output/time.txt", fstream::binary | fstream::out);
    //exception handling by return value
    if (!s.is_open() || !r.is_open() || !t.is_open()) {
        cerr << "fatal error: failed to open file" << endl;
        return 255;
    }

    for (size_t i = 0; i < 5; ++i) {
        //in
        size_t size;
        s >> size;
        vector<T> vec(size + 1);
        for (auto &item : vec)
            s >> item;

        //timing
        auto start = chrono::high_resolution_clock::now();
        auto [m, index] = matrix_chain_order(vec);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> period = end - start;

        //print to file
        r << m[0][size - 1] << '\n';
        print_optimal_parens(index, r, 0, size - 1);
        r << '\n';
        t << "size:" << setw(3) << size << " time:" << period.count() << "s\n";

        if (i == 0) {
            //easy text alignment for printing P214 graph 15-5
            auto print=[size]<typename T>(vector<vector<T>>& m, size_t s, char name) {
                cout << name << ":\n";
                for (size_t l = size - 1; l > 0; --l) {
                    string out;
                    out.reserve(65);
                    for (size_t k = 0; k < size - l; ++k)
                        out += to_string(m[k][k + l - s] + s) + ' ';
                    cout << string((35 - out.size() / 2), ' ') << out << '\n';
                }
            };
            print(m, 0, 'm');
            print(index, 1, 's');
        }
    }
}