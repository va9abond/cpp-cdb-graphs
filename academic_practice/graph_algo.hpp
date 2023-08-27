// [TODO]: is_tree (связность + ацикличность)
// [TODO]: Back removing
// [TODO]: Kruskal
// [TODO]: Prima
// [TODO]: Boruvka
//
// [TODO]: make ostov
// [TODO]: make min ostov


#ifndef GRAPHALGO_HPP
#define GRAPHALGO_HPP


#include <queue>
#include <stack>
#include <set>
#include <utility>
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
//
//
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

enum MST_Algo_t {
    Prim,
    Kruskal
};

template <
    MST_Algo_t algo_t
>
inline weighted_graph<int> generateMST (const weighted_graph<int>& Graph);

template <>
inline weighted_graph<int> generateMST<Kruskal> (const weighted_graph<int>& Graph) {
    weighted_graph<int> Result(Graph.sizeV());
    // dsf dsf_verts (Graph.m_Verts);
    dsf dsf_verts(Graph.sizeV());
    for (int i = 0; i < Graph.sizeV(); ++i) {
        dsf_verts.make_set(Graph.m_Verts[i]);
    }

    for (auto Eit = Graph.m_Edges.begin(); Eit != Graph.m_Edges.end(); ++Eit) {
        vert* V = (*Eit).sou;
        vert* U = (*Eit).tar;
        if (dsf_verts.find_set(V) != dsf_verts.find_set(U)) {
            dsf_verts.unite_sets(V,U);
            Result.m_Verts.push_back(U);
            Result.m_Verts.push_back(V);
            Result.m_Edges.insert(*Eit);
        }
    }
    return Result;
}


#endif // GRAPHALGO_HPP
