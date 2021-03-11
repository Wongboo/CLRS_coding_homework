//
// Created by 王波 on 2020/12/29.
//

#include "adjacent_matrix.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <iomanip>

using namespace graph;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    //准备
    default_random_engine gen(random_device{}());
    fstream time("../output/time.txt", ios::binary | ios::out);
    auto run = [&](size_t N) {
        static char i = '0';
        ++i;
        fstream input(string("../input/input") + i + ".txt", ios::binary | ios::out);
        fstream output(string("../output/result") + i + ".txt", ios::binary | ios::out);

        //声明图
        adjacent_matrix<int> graph(N);
        vector<size_t> vec(N - 1);
        iota(vec.begin(), vec.end(), 0);
        uniform_int_distribution dis(1, 20);

        //生成图
        for (size_t j{}; j < N; ++j) {
            shuffle(vec.begin(), vec.end(), gen);
            for (size_t k{}; k < N / 2 - graph.degree(j); ++k) {
                auto l = vec[k] + (vec[k] < j ? 0 : 1);
                if (graph.degree(l) <= N / 2)
                    graph.add_edge({j, l}, dis(gen));
            }
        }

        //输出input
        for (size_t k{}; k < graph.size(); ++k)
            for (size_t j{}; j < graph.size(); ++j)
                if (graph[k][j] != graph.max)
                    input << "结点：" << k << " " << j << " 权值："
                          << graph[k][j] << '\n';

        auto start = chrono::high_resolution_clock::now();
        auto[u, v] = graph.kruscal();
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> period = end - start;

        //输出output
        output << "代价为：" << v << '\n';
        for (size_t j{}; j < u.size(); ++j)
            output << "结点：" << u[j].first << " " << u[j].second << " 权值："
                   << graph[u[j].first][u[j].second] << '\n';

        time << "结点数：" << setw(3) << N << " 时间：" << period.count() << "s\n";
    };

    //运行
    run(8);
    run(64);
    run(128);
    run(512);
}