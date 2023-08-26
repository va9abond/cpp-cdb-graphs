#include <iostream>
#include <vector>
// #include "graph.hpp"
// #include "graph_algo.hpp"


#include <limits>
#include <set>


template<
    template <class... > class _Container_t,
    class _Value_t
>
void print (const _Container_t<_Value_t>& Cont) {
    auto It = Cont.begin(); std::cout << "\n" << "{ ";
    while (It != Cont.end()) {
        std::cout << *(It++) << " ";
    }
    std::cout << "};\n";
}

enum color { black, green, yellow };

int main() {
    // weighted_graph<int> graph { "graph.txt" };
    // for (const std::vector<int> &vec : graph.m_Matrix) {
        // print(vec);
    // }

    // std::set Visited = {std::make_pair<int,int>(12, 41), std::make_pair<int,int>(42,21)};
    // std::cout << (Visited.end() ? "eys" : "bool not");

    // std::vector<std::vector<int>> vvi (4, std::vector<int>(4,0));
    // for (const std::vector<int>& vi : vvi) {
        // print(vi);
    // }
    // std::cout << (vvi[0]).size();
    // std::map<int, std::pair<int,int>> mymap;
    // std::cout << (mymap.insert({23, {42,24}})).second;
    // std::cout << (45 & 35);
    // color c = black;
    // std::cout << sizeof(color) << " " << sizeof(c);

    // std::set<int> myset {2314, 245,0, 1,89 };
    // print(myset);
    // std::cout << *(--myset.end());

    // int a = 42;
    // int* ptr = new int(42);
    // int** pptr = &ptr;
    // delete *pptr;
    // std::cout << **pptr << " " << *ptr;

    // std::cout << (int)(std::numeric_limits<unsigned>::max() - 100);
    // std::set<int*>::size_type b = {23};
    // std::cout << a > b;

    std::set<int*> myset;
    myset.insert(new int{42});
    myset.insert(new int{112});
    myset.insert(new int{93});
    myset.insert(new int{9248});
    myset.insert(new int{11});

    std::cout << **myset.begin() << "\n";

    for (auto* val : myset) {
        std::cout << *val << " ";
    }

    // for (auto Vit = myset.begin(); Vit != myset.end(); ++Vit) {
    //     delete *Vit;
    // }

    // myset.clear();
    // std::cout << "\n" << myset.size();

    for (auto Vit = myset.begin(); Vit != myset.end(); ) {
        int** pptr = const_cast<int**>(&(*(Vit++)));
        delete *pptr;
    }
    std::cout << "\n";
    for (auto* val : myset) {
        std::cout << *val << " ";
    }


    // for (int* val : myset) {
    //     delete val;
    // }

    // auto itt = myset.begin();
    // while (itt != myset.end()) {
        // delete *itt;
    // }

    return 0;
}
