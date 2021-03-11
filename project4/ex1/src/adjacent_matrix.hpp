//
// Created by 王波 on 2021/1/2.
//

#ifndef PROJECT4_ADJACENT_MATRIX_HPP
#define PROJECT4_ADJACENT_MATRIX_HPP

#include <limits>
#include <vector>
#include <algorithm>

namespace graph {
    using namespace std;

    template<typename data>
    struct adjacent_matrix :vector<vector<data>>{
        //造些别名，即缩写
        using matrix = vector<vector<data>>;
        using edge = pair<int, int>;
        static constexpr data max = numeric_limits<data>::max();

        explicit adjacent_matrix(size_t n) : matrix(n, vector<data>(n, max)) {}

        //加边
        inline void add_edge(edge edge1, data cost) {
            (*this)[edge1.first][edge1.second] = cost;
            (*this)[edge1.second][edge1.first] = cost;
        }

        //返回度数
        inline size_t degree(size_t i) {
            return count_if((*this)[i].cbegin(), (*this)[i].cend(), [](data a) { return a != max; });
        }

        struct disjoint_set {
            //first表示p, second表示rank
            vector<pair<size_t, size_t>> set;

            explicit disjoint_set(size_t n) : set(n) {
                for (size_t i{}; i < n; ++i)
                    set[i].first = i;
            }

            //link同书
            auto link(size_t x, size_t y) {
                if (set[x].second > set[y].second)
                    set[y].first = x;
                else {
                    set[x].first = y;
                    if (set[x].second == set[y].second)
                        ++set[y].second;
                }
            }

            //find set同书
            size_t find_set(size_t i) {
                if (i != set[i].first)
                    set[i].first = find_set(set[i].first);
                return set[i].first;
            }

            //union set同书
            inline auto union_set(size_t i, size_t j) { return link(find_set(i), find_set(j)); }
        };

        //Kruscal方法
        auto kruscal() {
            vector<edge> A{};
            A.reserve((*this).size() - 1);
            disjoint_set set{(*this).size()};
            data cost{};

            vector<tuple<size_t, size_t, data>> E;
            E.reserve((*this).size() * ((*this).size() - 1) / 2);
            for (size_t i{}; i < (*this).size(); ++i)
                for (size_t j{}; j < i; ++j)
                    if ((*this)[i][j] != max)
                        E.emplace_back(i, j, (*this)[i][j]);
            sort(E.begin(), E.end(), [](auto l, auto m) { return get<2>(l) < get<2>(m); });

            //最后一次循环
            for (const auto &[u, v, l] : E)
                if (set.find_set(u) != set.find_set(v)) {
                    A.emplace_back(u, v);
                    set.union_set(u, v);
                    cost += l;
                }
            return pair(A, cost);
        }

    };
}


#endif //PROJECT4_ADJACENT_MATRIX_HPP
