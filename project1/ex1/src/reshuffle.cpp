#include <fstream>
#include <iostream>
#include <random>

using namespace std;

constexpr auto max_val = 0xffff;
constexpr auto max_size = 0x40000;

int main() {
    string filename = "../input/input.txt";
    fstream s(filename, fstream::binary | fstream::trunc | fstream::out);
    if (!s.is_open()) {
        cerr << "fatal error: failed to open " << filename << '\n';
        return 2;
    }
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dis(0, max_val);
    for (size_t i = 0; i < max_size; ++i)
        s << dis(gen) << '\n';
    cout << "Reshuffle success!\n";
    return 0;
}
