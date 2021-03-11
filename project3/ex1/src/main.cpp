//
// Created by 王波 on 2020/12/8.
//

#include "rb_tree.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <random>

using namespace std;
using namespace data_structure;

int main(){
    using T = int;
    random_device rd;
    mt19937 gen(rd());

    cout << "Do you want to reshuffle(if so, Press Y/y)\n";
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y'){
        //reshuffle
        fstream s("../input/input.txt", fstream::binary | fstream::out);
        uniform_int_distribution<T> dis(1, 10);
        vector<T> out(100);
        for (size_t i = 0; i < 100; ++i)
            out[i] = i== 0 ? dis(gen) : dis(gen) + out[i - 1];
        shuffle(out.begin(), out.end(), gen);
        for (const auto &item : out)
            s << item << '\n';
        s.close();
    }

    fstream inorder("../output/inorder.txt", fstream::binary | fstream::out);
    fstream time1("../output/time1.txt", fstream::binary | fstream::out);
    fstream delete_data("../output/delete_data.txt", fstream::binary | fstream::out);
    fstream time2("../output/time2.txt", fstream::binary | fstream::out);

    auto run = [&](size_t n){
        fstream in("../input/input.txt", fstream::binary | fstream::in);
        rb_tree<T> the_tree;

        //section1 inorder
        auto start = chrono::high_resolution_clock::now();
        vector<T> t(n);
        for (size_t i = 0; i < n; ++i){
            in >> t[i];
            the_tree.emplace(t[i]);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> period = end - start;
        time1 << "size:" << setw(3) << n << " time:" << period.count() << "s\n";
        inorder << "size:" << setw(3) << n << " sequence:" << the_tree << '\n';

        //section2 delete
        shuffle(t.begin(), t.end(), gen);
        start = chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n / 4; ++i)
            the_tree.remove(t[i]);
        end = chrono::high_resolution_clock::now();
        period = end - start;
        time2 << "size:" << setw(3) << n << " time:" << period.count() << "s\n";

        delete_data << "size:" << setw(3) << n << endl << "deleted node:";
        for (size_t i = 0; i < n / 4; ++i)
            delete_data << t[i] << " ";
        delete_data << "\nsequence:" << the_tree << '\n';
    };

    for (size_t i = 20; i <= 100; i += 20)
        run(i);
}

