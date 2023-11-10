#include <iostream>
#include <vector>
#include <limits>
#include <set>
#include "graph.hpp"
#include "graph_algo.hpp"


namespace msl
{
    template<
        template <class... > class _Container_t,
        class _Value_t
    >
    void print (const _Container_t<_Value_t>& Cont)
    {
        auto It = Cont.begin(); std::cout << "\n" << "{ ";
        while (It != Cont.end()) {
            std::cout << *(It++) << " ";
        }
        std::cout << "};\n";
    }
}


template <
    MST_Algo_t algo_t
>
void minimal_spanning_tree_presentation (const weighted_graph<int>& Graph)
{
    weighted_graph<int> MST = generateMST<MST_Algo_t::Kruskal>(Graph);
    print(MST); std::cout << "\n";
}


template <
    SP_Algo_t algo_t
>
void shortest_path_presentation (const weighted_graph<int>& Graph)
{
    vertptr startV = Graph.m_Verts[7];
    auto res = generateSP<algo_t>(Graph, startV);
    generateSP_print(res, startV);
}


void DFS_presentation (const weighted_graph<int>& Graph)
{
    DFSprint(Graph);
    weighted_graph<int> MST = generateMST<MST_Algo_t::Kruskal>(Graph);
    DFSprint(MST);
}

template <
    MF_Algo_t algo_t
>
void generationMF_presentation (const weighted_graph<int>& Graph, vert Source, vert Target) {
    std::cout << "Source: " << Source << "\n";
    std::cout << "Targer: " << Target << "\n";

    auto flow = generateMF<MF_Algo_t::Edmonds_Karp>(Graph, Source, Target);
    std::cout << "==============================================\n";
    std::cout << "flow: " << flow.first << "\n";
    std::cout << "==============================================\n";
}

void generationMM_presentation (const std::string& file_name) {
    auto maximum_matching = generateMM(file_name);
    std::cout << "==============================================\n";
    std::cout << "maximum matching: " << maximum_matching.first << "\n";
    std::cout << "==============================================\n";
}

void greedy_coloring_presentation (const weighted_graph<int>& Graph) {
    std::vector<int> colors = greedy_coloring(Graph);
    std::cout << "=================== colors ===================";
    msl::print(colors);
    std::cout << "==============================================\n";
}

int main()
{
    weighted_graph<int> Graph ("weight_func");
    // print(Graph);

    // weighted_graph<int> BipartGraph ("matching_converted");

    // print(generateMM("matching"));

#if 0
    // task 3.1
    std::cout << "\n==== task 3.1 ====\n";
    minimal_spanning_tree_presentation<MST_Algo_t::Kruskal>(Graph);

    // task 3.2
    std::cout << "\n==== task 3.2 ====\n";
    DFS_presentation(Graph);

    // task 3.3
    std::cout << "\n==== task 3.3 ====\n";
    shortest_path_presentation<SP_Algo_t::Bellman_Ford>(Graph);

    // task 4.1
    std::cout << "\n==== task 4.1 ====\n";
    generationMF_presentation<MF_Algo_t::Edmonds_Karp>(Graph, 0, 11);

    // task 4.2
    std::cout << "\n==== task 4.2 ====\n";
    generationMM_presentation("matching");

    // task 4.3
    std::cout << "\n==== task 4.3 ====\n";
    greedy_coloring_presentation(Graph);
#endif

    // vertptr vptr = BipartGraph.extension({0, 1,4,6}, -1);
    // std::cout << "new vert: " << vptr;
    // print(BipartGraph);
    // std::cout << "\n==== task 4.2 ====\n";
    // generationMF_presentation<MF_Algo_t::Edmonds_Karp>(BipartGraph, 0, 6);

    // residual_network<int> net(Graph);
    // print(net);

    // auto res = generate_2dvector_from_file_4_2("matching");
    // for (const auto& item : res) {
    //     msl::print(item);
    // }


    // vertptr from = net.m_Verts[0]; std::cout << "from: " << from << " " << *from << "\n";
    // vertptr to = net.m_Verts[11];   std::cout << "to: " << to << " " << *to << "\n";
    // auto pair = net.find_path(from, to);
    //
    // std::cout << pair.first << "\n";
    // msl::print(pair.second);


    // shortest_path_presentation<SP_Algo_t::Bellman_Ford>(net::Mybase);

    // auto path = net.find_path(net.m_Verts[7], net.m_Verts[2]);
#if 0
    auto flow = generateMF<MF_Algo_t::basic_Ford_Fulkerson> (Graph, Graph.m_Verts[0], Graph.m_Verts[11]);

    std::cout << "\n{";
    for (const auto& verts : flow) {
        std::cout << "\n{ ";
        for (const auto& flow_ : verts) {
            std::cout << flow_ << " ";
        }
        std::cout << "}\n";
    }
    std::cout << "}\n";
#endif
    // auto& p = path.second;
    // std::cout << "\n{ ";
    // for (const auto& Vptr : p) {
    //     std::cout << *Vptr << " ";
    // }
    // std::cout << "}\n";

    // net.
    // print(net.m_Flow);

    // weighted_graph<int> graph { "weight_func.txt" };
    // std::vector<std::vector<int>> vvi = generate_2dvector_from_file("weight_func.txt");
    // for (const std::vector<int> &vi : vvi) {
        // print(vi);
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

    // std::set<int*> myset;
    // myset.insert(new int{42});
    // myset.insert(new int{112});
    // myset.insert(new int{93});
    // myset.insert(new int{9248});
    // myset.insert(new int{11});
    //
    // std::cout << **myset.begin() << "\n";
    //
    // for (auto* val : myset) {
    //     std::cout << *val << " ";
    // }

    // for (auto Vit = myset.begin(); Vit != myset.end(); ++Vit) {
    //     delete *Vit;
    // }

    // myset.clear();
    // std::cout << "\n" << myset.size();

    // for (auto Vit = myset.begin(); Vit != myset.end(); ) {
    //     int** pptr = const_cast<int**>(&(*(Vit++)));
    //     delete *pptr;
    // }
    // std::cout << "\n";
    // for (auto* val : myset) {
    //     std::cout << *val << " ";
    // }


    // for (int* val : myset) {
    //     delete val;
    // }

    // auto itt = myset.begin();
    // while (itt != myset.end()) {
        // delete *itt;
    // }




    // DFSprint(Graph);
    // DFSprint(MST);



    // for (auto Vit = DFSresult.begin(); Vit != DFSresult.end(); ++Vit) {
        // std::cout << "\n" << (*Vit) << " ";
    // }

    // for (auto it = MST.m_Edges.begin(); it != MST.m_Edges.end(); ++it) {
    //     std::cout << (*it).tar << " -> " << *(*it).tar << (*it).sou << " -> " << *(*it).sou << "\n";
    // }

    // print(DFSvoid(Graph));
    // print(DFSvoid(MST));


    // int a = 42;   int* pa = &a;
    // int b = 34;   int* pb = &b;
    // int c = 2234; int* pc = &c;
    //               int* pm = new int(100);
    //
    // std::vector<int*> pv = { pm, pa, pb, pc };
    // std::cout << *pv[0] << "\n";
    // print(pv);
    // std::vector<int*> other(pv);
    // // delete pm; pm = nullptr;
    // // *other[0] = 8;
    // std::cout << *pv[0] << "\n";
    // print(other);
    // delete pm; pm = nullptr;
    // print(pv); print(other); std::cout << *other[0];

    return 0;
}
