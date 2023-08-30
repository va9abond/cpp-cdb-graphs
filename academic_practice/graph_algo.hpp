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

template <> // template specialization when MST generating by Kruskal's algorithm
inline weighted_graph<int> generateMST<Kruskal> (const weighted_graph<int>& Graph) {
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


template <
    class Weight_t
> void print (const weighted_graph<Weight_t>& Graph) {
    std::cout << "\nGraph: |V| = " << Graph.sizeV() << ", |E| = " << Graph.sizeE() << "\n";
    for (auto Eit = Graph.m_Edges.begin(); Eit != Graph.m_Edges.end(); ++Eit) {
        // std::cout << "hello";
        std::cout << "source: " << *(*Eit).sou << ";  target: " << *(*Eit).tar << " [" << (*Eit).wei << "]\n";
    }
}


inline void DFSprint_main (const weighted_graph<int>& Graph, const vert* Vnow, std::vector<vert>& Visited, std::vector<vert>& Vindxs) {
    Visited[*Vnow] = 1;
    std::vector<int> weightNbrs { Graph.m_Weightfunc[*Vnow] }; // weights of edges with nbrs
    for (int vi = 0; vi < (int)weightNbrs.size(); ++vi) { //
        if (weightNbrs[vi] && !Visited[vi]) { // filter neighbours: just unvisiter yet
            DFSprint_main(Graph, Graph.m_Verts[vi], Visited, Vindxs);
            Vindxs.push_back(*Graph.m_Verts[vi]);
        }
    }

}


inline std::vector<vert> DFSprint_init (const weighted_graph<int>& Graph) {
    int countV = Graph.sizeV();

    std::vector<vert> Vindxs; // verts indexes
    std::vector<vert> Visited (countV, 0);

    for (int vi = 0; vi < countV ; ++vi) {
        if (!Visited[vi]) {
            DFSprint_main(Graph, Graph.m_Verts[vi], Visited, Vindxs);
        }
    }

    return Vindxs;
}


#endif // GRAPHALGO_HPP
