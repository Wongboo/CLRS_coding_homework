//
// Created by 王波 on 2020/12/8.
//

#include "../../ex1/src/rb_tree.hpp"
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
        vector<std::pair<T,T>> out(45);
        for (size_t i = 0; i < 45; ++i){
            uniform_int_distribution<T> ds(i >= 25 ? i + 5 : i, i >=25 ? 50 : 25);
            out[i] = {i >= 25 ? i + 5 : i, ds(gen)};
        }
        shuffle(out.begin(), out.end(), gen);
        for (const auto &item : out)
            s << item.first << " " << item.second << '\n';
        s.close();
    }

    fstream inorder("../output/inorder.txt", fstream::binary | fstream::out);
    fstream delete_data("../output/delete_data.txt", fstream::binary | fstream::out);
    fstream search("../output/search.txt", fstream::binary | fstream::out);
    fstream in("../input/input.txt", fstream::binary | fstream::in);
    interval_tree<T> the_tree;

    //section1 inorder
    vector<pair<T,T>> t(30);
    for (size_t i = 0; i < 30; ++i) {
        in >> t[i].first >> t[i].second;
        the_tree.emplace(t[i]);
    }
    inorder << the_tree << '\n';

    //section2 delete
    shuffle(t.begin(), t.end(), gen);
    delete_data << "delete:\n";
    for (size_t i = 0; i < 3; ++i) {
        auto max = the_tree.remove(t[i]);
        delete_data << t[i].first << " " << t[i].second << " " << max << '\n';
    }
    delete_data << "inorder:\n" << the_tree << '\n';

    //section3 search
    uniform_int_distribution<T> dis(0, 4);
    auto run = [&](T b){
        auto [x, y] = minmax({dis(gen) + b, dis(gen) + b});
        search << "search for:" << x << "," << y;
        if (auto result = the_tree.interval_search({x, y}))
            search << " success:" << result->first << "," << result->second << '\n';
        else
            search << " null" << '\n';
    };

    run(uniform_int_distribution<T>(0,16)(gen));
    run(26);
    run(uniform_int_distribution<T>(30, 46)(gen));
}