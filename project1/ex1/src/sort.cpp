//
// Created by 王波 on 2020/11/4.
//
#ifdef _MSC_VER
#pragma comment(linker, "/STACK:1024000,1024000")
#endif

#include <iostream>
#include "merge_sort.hpp"
#include "quick_sort.hpp"
#include "count_sort.hpp"
#include "heap_sort.hpp"

using namespace std;
using namespace sort;

constexpr auto max_size = 0x40000;

int main(){
    cout << "Do you want to reshuffle(if so, Press Y/y, ensure you've built reshuffle)\n";
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y')
#ifdef _WIN32
        system(".\reshuffle.exe");
#else
        system("./reshuffle");
#endif

    //input
    string filename = "../input/input.txt";
    fstream s(filename, fstream::binary | fstream::in);
    if (!s.is_open()) {
        cerr << "fatal error: failed to open " << filename << '\n';
        return 2;
    }
    vector<int> to_sort(max_size);
    for (size_t i = 0; i < max_size; ++i)
        s >> to_sort[i];

    //export data to csv for data analysis in excel/WPS
    fstream t("../output/output.csv", fstream::binary | fstream::trunc | fstream::out);
    t << "num,8,64,512,4096,32768,262144\n";
    t.close();

#ifdef USE_INSERT
    sorting(insert_sort, to_sort);
    //sorting(insert_sort_enhance, to_sort);
#endif
//the following macro is for debug
//ignore them!
#ifdef USE_MERGE
    sorting(merge_sort, to_sort);
    sorting(merge_sort_enhance, to_sort);
    sorting(std_stable_sort, to_sort);
#endif
#ifdef USE_QUICK
    sorting(quick_sort, to_sort);
    sorting(quick_sort_enhance, to_sort);
    //sorting(q_sort, to_sort);
    sorting(std_sort, to_sort);
#endif
#ifdef USE_HEAP
    sorting(heap_sort, to_sort);
#endif
#ifdef USE_COUNT
    sorting(count_sort, to_sort);
#endif

    cout << "Sorting complete!\n"
            "Do you want to start python module(if so, Press Y/y)\n";
    cin >> ch;
    if (ch == 'y' || ch == 'Y'){
        cout << "Now start python process.\n";
#ifdef _WIN32
        system("python excel.py");}
#else
        system("python3 excel.py");}
#endif
}

