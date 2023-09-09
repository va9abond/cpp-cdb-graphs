#ifndef GRAPHALGO_HPP
#define GRAPHALGO_HPP


#include <queue>
#include <stack>
#include <set>
#include <utility>
#include <algorithm>
#include "graph.hpp"
#include "dsf.hpp"


// template <
//     class Graph_t
// >
// inline void BFSvoid (const Graph_t& Graph, int Vstart = 1) {
//     std::queue<int> Queue;
//     std::set<int> Visited;
//
//     Queue.push(Vstart);
//     Visited.insert(Vstart);
//
//     while (!Queue.empty())
//     {
//         int Vnow = Queue.front();
//         const std::vector<int>& Neighbours = Graph[Vnow];
//
//         for (int Vnext {1}; Vnext <= Neighbours.size(); ++Vnext)
//         {
//             if (Neighbours[Vnext - 1]) { // Vnow connected with Vnext
//                 if (Visited.find(Vnext) != Visited.end()) {
//                     Visited.insert(Vnext);
//                     Queue.push(Vnext);
//                 }
//             }
//         }
//         Queue.pop();
//     }
// }


// template <
//     class Graph_t
// >
// inline void DFSvoid (
//     const Graph_t& Graph,
//     int Vnow, // [TODO]: Vnow is Visited.front() always??
//     std::set<int>& Visited
// ) {
//     // std::set<int> Visited;
//     const std::vector<int>& Neighbours = Graph[Vnow];
//
//     for (int Vnext {1}; Vnext <= Neighbours.size(); ++Vnext) {
//        if (Neighbours[Vnext - 1]) { // Vnow connected with Vnex
//             if (Visited.find(Vnext) != Visited.end()) {
//                 // Vnext doesn't be visited before
//                 Visited.insert(Vnext);
//                 DFS(Graph, Vnext, Visited);
//             }
//         }
//     }
// }


// ==== minimal-spanning-tree problem ====
// [pseudocode]: generate Minimal Spannig Tree
// inline weighted_graph<int> generic_MST (const weighted_graph<int>& Graph) {
    // 0. init MST {Graph.m_Verts, empty_set};
    // 1. Choose random vert and insert it in MST.m_Verts
    // 2. while (MST is not Spanning Tree) {
    //        cutGraph = cutting_graph(respect wirh MST.m_Verts)
    //        find_all_safe_edges() {
    //            for (vi : Graph.m_Verts) {
    //                for (vj : Graph.m_Verts) {
    //                    if (vi in cutGraph.m_Verts && vj in Graph.m_Verts && vj not in cutGraph.m_Verts) {
    //                              edge e {vi, vj} is safe;
    //                              add e in safe_set
    //                    }
    //                }
    //            }
    //        };
    //        choose light edge from all sefe
    //        add safe edge to MST
    // }
// }

enum class MST_Algo_t {
    Prim,
    Kruskal
};

template <
    MST_Algo_t algo_t
>
inline weighted_graph<int> generateMST (const weighted_graph<int>& Graph);

template <> // template specialization when MST generating by Kruskal's algorithm
inline weighted_graph<int> generateMST<MST_Algo_t::Kruskal> (const weighted_graph<int>& Graph) {
    int countV = Graph.sizeV();

    int MSTcountE = 0;
    std::vector<std::vector<int>> res_weightfunc (countV, std::vector<int>(countV, 0));

    dsf dsf_verts(countV);
    for (int i = 0; i < countV; ++i) {
        dsf_verts.make_set(Graph.m_Verts[i]);
    }

    for (
        auto Eit = Graph.m_Edges.begin();
        Eit != Graph.m_Edges.end() && MSTcountE < countV - 1;
        ++Eit
    ) {
        vert* U = (*Eit).sou;
        vert* V = (*Eit).tar;
        if (dsf_verts.find_set(U) != dsf_verts.find_set(V)) {
            dsf_verts.unite_sets(U,V);
            // connect tree U with tree V
            res_weightfunc[*U][*V] = Graph.m_Weightfunc[*U][*V];
            res_weightfunc[*V][*U] = Graph.m_Weightfunc[*V][*U];
            ++MSTcountE;
        }
    }

    // construct MST graph by res_weightfunc
    weighted_graph<int> Result(res_weightfunc);
    return Result;
}


// ==== Graph printing ====
template <
    class Weight_t
> void print (const weighted_graph<Weight_t>& Graph) {
    std::cout << "\nGraph: |V| = " << Graph.sizeV() << ", |E| = " << Graph.sizeE() << "\n";

    for (const auto& V : Graph.m_Verts) {
        std::cout << V << " <--- " << *V << "\n";
    }

    for (auto Eit = Graph.m_Edges.begin(); Eit != Graph.m_Edges.end(); ++Eit) {
        // std::cout << "hello";
        std::cout << "source: " << *(*Eit).sou << ";  target: " << *(*Eit).tar << " [" << (*Eit).wei << "]\n";
    }
}


inline void DFSprint_main (const weighted_graph<int>& Graph, const vert* Vnow, std::vector<vert>& Visited, std::vector<vertptr>& Vindxs) {
    Visited[*Vnow] = 1;
    std::vector<int> weightNbrs { Graph.m_Weightfunc[*Vnow] }; // weights of edges with nbrs
    for (int vi = 0; vi < (int)weightNbrs.size(); ++vi) { //
        if (weightNbrs[vi] && !Visited[vi]) { // filter neighbours: just unvisiter yet
            DFSprint_main(Graph, Graph.m_Verts[vi], Visited, Vindxs);
            Vindxs.push_back(Graph.m_Verts[vi]);
        }
    }
}


inline std::vector<vertptr> DFSprint_init (const weighted_graph<int>& Graph) {
    int countV = Graph.sizeV();

    std::vector<vertptr> Vindxs; // verts indexes
    std::vector<vert> Visited (countV, 0);

    for (int vi = 0; vi < countV ; ++vi) {
        if (!Visited[vi]) {
            DFSprint_main(Graph, Graph.m_Verts[vi], Visited, Vindxs);
        }
    }

    return Vindxs;
}

inline void DFSprint (const weighted_graph<int>& Graph)
{
    std::vector<vertptr> DFSresult = DFSprint_init(Graph);
    std::cout << "\n" << "{\n";
    for (const auto& V : DFSresult) {
        std::cout << "    " << *V << "  " << V << "\n";
    }
    std::cout << "};";
}


// ==== shortest-path problem algorithm type ====
enum class SP_Algo_t
{
    Bellman_Ford,
};

#if 0
inline void init_single_source (const weighted_graph<int>& Graph, const vertptr Source)
{
    std::vector<int> estimates(Graph.sizeV(), std::numeric_limits<int>::max());
    std::vector<vertptr> predecessors(Graph.sizeV(), nullptr);
    estimates[*Source] = 0;
}
#endif

inline std::vector<vert> generate_path_ (std::vector<vertptr>& Preds, const vertptr Source, vertptr Target)
{
    // assert(Source != Target);
    std::vector<vert> Path;
    vertptr Vprev = Target;

    while (Vprev != Source) { // Preds[*Vprev], i.e. Preds[*Vstart] = nullptr;
        Path.push_back(*Vprev);
        Vprev = Preds[*Vprev];
    }
    Path.push_back(*Source);
    std::reverse(Path.begin(), Path.end());
    return Path;
}

template <
    SP_Algo_t algo_t
>
inline std::map<vertptr, std::pair<int, std::vector<vert>>> generateSP (
    const weighted_graph<int>&, const vertptr);

template<>
inline std::map<vertptr, std::pair<int, std::vector<vert>>> generateSP<SP_Algo_t::Bellman_Ford> (
    const weighted_graph<int>& Graph, const vertptr Source)
{
    using wedge = weighted_graph<int>::wedge;
    const int countV = Graph.sizeV();

    // init single source
    const int int_max = std::numeric_limits<int>::max();
    std::vector<int> estimates (countV, int_max);
    std::vector<vertptr> preds (countV, nullptr); // predecessor of each vert
    estimates[*Source] = 0;

    // main loop
    for (int i = {0}; i < countV - 1; ++i) {
        // relaxation
        for (const wedge& E : Graph.m_Edges) {
            // sou ---> tar
            int& sou_est = estimates[*E.sou];
            int& tar_est = estimates[*E.tar];
            bool is_need_relax = (sou_est < int_max) && (tar_est > sou_est + E.wei);

            if (is_need_relax) {
                tar_est = sou_est + E.wei;
                preds[*E.tar] = E.sou;
            }
#if flag_reverse_edge
            // tar ---> sou
            if (estimates[*E.sou] < estimates[*E.tar] + E.wei) { // s < t+w is always true
                estimates[*E.sou] = estimates[*E.tar] + E.wei;
                preds[*E.tar] = E.sou;
            }
#endif
        }
    }
    // check a negative loop
    for (const wedge& E : Graph.m_Edges) {
        // sou ---> tar
        assert(estimates[*E.tar] <= estimates[*E.sou] + E.wei);
#if flag_reverse_edge
        // tar ---> sou
        if (estimates[*E.sou] < estimates[*E.tar] + E.wei) {
            return false;
        }
#endif
    }

    // generate result
    std::map<vertptr, std::pair<int, std::vector<vert>>> general_res;
    for(int i = {0}; i < countV; ++i) {
        std::pair<int, std::vector<vert>> dis_path { estimates[i], generate_path_(preds, Source, Graph.m_Verts[i]) };
        general_res.emplace(Graph.m_Verts[i], dis_path);
    }
    return general_res;
}

inline void generateSP_print (const std::map<vertptr, std::pair<int, std::vector<vert>>>& map, vertptr Source)
{
    std::cout << "\n===== All Shortest Ways, source: " << *Source << " ====\n";
    for (const auto& el : map) {
        auto path = el.second.second;
        std::cout << el.first << " <--- " << *el.first << ", distance:" << el.second.first;
        std::cout << " { ";
        for (const auto& v : path) {
            std::cout << v << " ";
        }
        std::cout << "}\n";
    }
}


// ==== maximum flow problem algorithm type ====
enum class MF_Algo_t
{
    basic_Ford_Fulkerson,
};
// residual network
struct maximum_flow_problem
{
    // construct



    // output
};

#endif // GRAPHALGO_HPP
