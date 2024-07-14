#ifndef GUARD_head_h
#define GUARD_head_h
#include<iostream>
#include<algorithm>

template<class It, class T>
T search(T container, It begin, It end) {
    T tmp;
    typename T::iterator it = container.begin();
    while (it != container.end()) {
        if (std::find(begin, end, *it) != end)
            tmp.push_back(*std::find(begin, end, *it));
        ++it;
    }
    return tmp;
}
template<class T>
void output(T tmp) {
    typename T::iterator it = tmp.begin();
    while (it != tmp.end()) {
        std::cout << *it << "  ";
        ++it;
    }
    std::cout << std::endl;
}
#endif // !GUARD_head_h
