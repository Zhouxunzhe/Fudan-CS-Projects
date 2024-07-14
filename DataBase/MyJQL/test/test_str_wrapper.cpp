#include <iostream>
#include <string>
#include <map>
using namespace std;

#ifdef _WIN32
typedef long long my_off_t;
#else
typedef long my_off_t;
#endif

typedef struct {
    my_off_t addr;
    short idx;
} my_RID;

map<pair<my_off_t, short>, string> m;

extern "C" void init() {
    m.clear();
}

extern "C" void insert(my_RID rid, char *s) {
    pair<my_off_t, short> p{rid.addr, rid.idx};
    if (m.count(p)) {
        terminate();
    }
    m[p] = s;
}

extern "C" void erase(my_RID rid) {
    pair<my_off_t, short> p{rid.addr, rid.idx};
    if (!m.count(p)) {
        terminate();
    }
    m.erase(p);
}

extern "C" size_t get_total() {
    return m.size();
}

extern "C" my_RID get_rid(int idx) {
    auto it = m.begin();
    for (int i = 0; i < idx; ++i) {
        ++it;
    }
    auto p = it->first;
    return {p.first, p.second};
}

extern "C" int equal(my_RID rid, char *s) {
    pair<my_off_t, short> p{rid.addr, rid.idx};
    if (!m.count(p)) {
        terminate();
    }
    return m[p] == s;
}