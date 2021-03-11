//
// Created by 王波 on 2020/12/29.
//

#include <string>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>
#ifdef USE_OMP
#include <omp.h>
#endif
#include "adjacent_list.hpp"

using namespace graph;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    //准备
    default_random_engine gen(random_device{}());
    ios::sync_with_stdio(false);
    constexpr auto mode = ios::binary | ios::out;
    fstream time("../output/time.txt", mode);

    for (char i{'1'}; i < '5'; ++i) {
        static size_t N = 9;
        N *= 3;
        auto run = [&](char a, size_t n) {
            fstream input(string("../input/input") + i + a + ".txt", mode);
            fstream output(string("../output/result") + i + a + ".txt", mode);

            //声明图
            adjacent_list<int> graph(N);
            vector<size_t> vec(N - 1);
            iota(vec.begin(), vec.end(), 0);
            uniform_int_distribution dis(0, 50);

            //生成图
            for (size_t j{}; j < graph.size(); ++j) {
                shuffle(vec.begin(), vec.end(), gen);
                for (size_t k{}; k < n; ++k) {
                    auto l = vec[k] + (vec[k] < j ? 0 : 1);
                    graph[j].emplace_back(l, dis(gen));
                    //sort后input没有本质区别
                    input << "结点：" << j << " " << l << " 权值："
                          << graph[j].back().second << '\n';
                }
                graph[j].sort(graph.comp);
            }

            //记时，try块没用了，只因所有边均非负
            decltype(chrono::high_resolution_clock::now()) start;
            decltype(graph.johnson()) source_list;
            //其实有边负也不要用try块，主要之前要求重新计时重构太麻烦了，就用了
            //当时写了还没来得及改，结果又不用重新计时了，直接注释了
            //while (true)
            //try {
            start = chrono::high_resolution_clock::now();
            source_list = graph.johnson();
            //break;
            //} catch (const pair<size_t, size_t> &edge) {
            //auto &val = find_if(graph[edge.first].begin(), graph[edge.first].end(),
            //                    [&](auto a) { return a.first == edge.second; })->second;
            //output << "发现一个负环:("
            //       << edge.first << " " << edge.second << " 值 " << val
            //       << ") 现在纠正之\n";
            //assert(val < 0);    //NOLINT
            //val = -val;
            //}

            //输出time
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> period = end - start;
            time << "顶点数：" << setw(3) << N
                 << " 度数：" /*<< setw(2)*/ << n
                 << " 时间：" << period.count() << "s\n";

            //输出output
            start = chrono::high_resolution_clock::now();
            //MSVC 似乎对openmp ordered支持极其糟糕，添加了也没啥提升
#pragma omp parallel for ordered schedule(static, 1)//NOLINT
            for (int k = 0; k < N; ++k) {
                stringstream the_stream;
                auto printer = [&](size_t j, const auto &lambda) -> void {
                    if (source_list[k][j].second != -1)
                        lambda(source_list[k][j].second, lambda);
                    if (j != k)
                        the_stream << ',' << j;
                };
                for (size_t j{}; j < N; ++j) {
                    the_stream << '(' << k;
                    printer(j, printer);
                    the_stream << ' '
                               << (source_list[k][j].first == graph.max ? "∞" : to_string(source_list[k][j].first))
                               << ") ";
                }
#pragma omp ordered
                output << move(*the_stream.rdbuf()).str() << "\n";
            }
            end = chrono::high_resolution_clock::now();
            period = end - start;
            cout << "顶点数：" << setw(3) << N
                  << " 度数：" << n
                  << " 打印时间：" << period.count() << "s\n";
        };
        //运行
        run('1', log(N) / log(5));
        run('2', log(N) / log(7));
    }
}