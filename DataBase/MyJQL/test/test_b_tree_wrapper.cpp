#include <map>
using namespace std;

#ifdef _WIN32
typedef long long my_off_t;
#else
typedef long my_off_t;
#endif

map<my_off_t, short> m;

extern "C" void init() {
    m.clear();
}

extern "C" void insert(my_off_t addr, short idx) {
    if (m.count(addr)) {
        terminate();
    }
    m[addr] = idx;
}

extern "C" void erase(my_off_t addr) {
    if (!m.count(addr)) {
        terminate();
    }
    m.erase(addr);
}

extern "C" short get_idx(my_off_t addr) {
    if (!m.count(addr)) {
        terminate();
    }
    return m[addr];
}

extern "C" int contain(my_off_t addr) {
    return m.count(addr) != 0;
}

extern "C" size_t get_total() {
    return m.size();
}

extern "C" my_off_t get_addr(int idx) {
    auto it = m.begin();
    for (int i = 0; i < idx; ++i) {
        ++it;
    }
    return it->first;
}