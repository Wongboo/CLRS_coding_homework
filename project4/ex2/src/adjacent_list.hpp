//
// Created by 王波 on 2021/1/2.
//

#ifndef PROJECT4_ADJACENT_LIST_HPP
#define PROJECT4_ADJACENT_LIST_HPP

#include <queue>
#include <map>
#include <list>
#include <fstream>
#include <numeric>
#include <functional>
#include <memory>
#include <iostream>
#include <cassert>
#ifdef USE_OMP
#include <omp.h>
#endif

namespace graph {
    using namespace std;

    template<typename data>
    struct adjacent_list : vector<list<pair<size_t, data>>> {
        //造些别名，即缩写
        using node = pair<size_t, data>;
        using single_source = vector<pair<data, size_t>>;
        static constexpr data max = numeric_limits<data>::max();
        static constexpr auto comp = [](node a, node b){return a.first < b.first; };

        explicit adjacent_list(size_t size) : vector<list<node>>(size) {}

        //初始化单点源
        inline auto initialize_single_source(size_t s) const{
            single_source source(this->size(), {max, -1});
            source[s].first = 0;
            return source;
        }

        //松弛化
        inline bool relax(size_t u, size_t v, data w, single_source & source) const{
            if (source[u].first != max && source[v].first > source[u].first + w){
                source[v].first = source[u].first + w;
                source[v].second = u;
                return true;
            }
            return false;
        }

        //Bellman_Ford方法
        inline auto bellman_ford(size_t s) const{
            //初始化单点源
            auto source = initialize_single_source(s);

            for (size_t i{}; i < source.size() - 1; ++i) {
                for (size_t j{}; j < source.size(); ++j)
                    for (const auto &[v, w] : (*this)[j])
                        relax(j, v, w, source);
            }

            for (size_t j{}; j < source.size(); ++j)
                for (const auto &[i, w] : (*this)[j])
                    if (source[j].first != max && source[i].first > source[j].first + w){
                        throw pair{j, i};
                    }
            return source;
        }

        //Dijkstra方法
        inline auto dijkstra(size_t s) const{
            //初始化单点源
            auto source = initialize_single_source(s);
#ifdef USE_SET_IMPLEMENT
            multimap<data, size_t> the_queue;
            vector<typename decltype(the_queue)::const_iterator> vec(this->size());
            for (size_t i{}; i < this->size(); ++i)
                vec[i] = the_queue.emplace(source[i].first, i);

            while (!the_queue.empty()){
                auto u = the_queue.extract(the_queue.begin()).mapped();
                for (auto &[v, w] : (*this)[u])
                    if (relax(u, v, w, source))
                         vec[v] = the_queue.emplace_hint(the_queue.erase(vec[v]), source[v].first, v);
            }
#else
            //注意别用vector<bool>，其实此例用这个也没啥问题
            auto is_visited = make_unique<bool[]>(this->size());
            auto comparer = [](const node& i, const node& j){return i.second > j.second;};
            vector<node> vec{{s, 0}};
            vec.reserve(this->size());
            priority_queue the_queue{comparer, move(vec)};
            //priority_queue 即是一个二叉最小/大堆，最小堆可以用库函数

            while (!the_queue.empty()){
                size_t u = the_queue.top().first;
                the_queue.pop();
                if (!is_visited[u]){
                    is_visited[u] = true;
                    for (auto &[v, w] : (*this)[u])
                        if (relax(u, v, w, source)){
                            //若relaxed且无负边，一定不会visited
                            //debug下检测一下
                            assert(!is_visited[v]);
                            //用lazy deletion的技巧
                            the_queue.emplace(v, source[v].first);
                        }
                }
            }
#endif
            return source;
        }

        //Johnson方法
        auto johnson() const{
            //初始化G'
            adjacent_list list_{*this};
            list_.emplace_back();
            for (size_t i{}; i < list_.size() - 1; ++i)
                list_.back().emplace_back(i, 0);

            //改G'权值
            auto source = list_.bellman_ford(this->size());
            for (size_t j = 0; j < source.size(); ++j)
                for (auto &[u, edge] : list_[j])
                    edge += source[j].first - source[u].first;

            //Dijkstra方法
            vector<single_source> source_list(this->size());
#pragma omp parallel for schedule(dynamic, 1) //NOLINT, very important
            for (int i = 0; i < this->size(); ++i) {
                source_list[i] = list_.dijkstra(i);
                for (size_t j{}; j < this->size(); ++j)
                    if (source_list[i][j].first != max)
                        source_list[i][j].first += source[j].first - source[i].first;
            }

            return source_list;
        }
    };
}

#endif //PROJECT4_ADJACENT_LIST_HPP
