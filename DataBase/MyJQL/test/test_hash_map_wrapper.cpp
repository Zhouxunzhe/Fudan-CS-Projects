#include <iostream>
#include <vector>
#include <set>
#include <map>
using namespace std;
#ifdef _WIN32
typedef long long my_off_t;
#else
typedef long my_off_t;
#endif
vector<set<my_off_t>> v;
map<my_off_t, short> m;
extern "C" void init(size_t size)
{
    v.clear();
    m.clear();
    v.resize(size);
}
extern "C" void add(short size, my_off_t id)
{
    if (v[size].count(id)) {
        terminate();
    }
    if (m.count(id)) {
        terminate();
    }
    v[size].insert(id);
    m[id] = size;
}
extern "C" void erase(short size, my_off_t id)
{
    if (!v[size].count(id)) {
        terminate();
    }
    if (!m.count(id)) {
        terminate();
    }
    v[size].erase(id);
    m.erase(id);
}
extern "C" int contain(short size, my_off_t id)
{
    return v[size].count(id) != 0;
}
extern "C" int empty(short size)
{
    return v[size].empty();
}
extern "C" short get_size(my_off_t id) {
    if (!m.count(id)) {
        terminate();
    }
    return m[id];
}
extern "C" size_t get_total() {
    return m.size();
}
extern "C" my_off_t get_item(int idx) {
    auto it = m.begin();
    for (int i = 0; i < idx; ++i) {
        ++it;
    }
    return it->first;
}