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


template <
    class Graph_t
>
inline void BFSvoid (const Graph_t& Graph, int Vstart = 1) {
    std::queue<int> Queue;
    std::set<int> Visited;

    Queue.push(Vstart);
    Visited.insert(Vstart);

    while (!Queue.empty())
    {
        int Vnow = Queue.front();
        const std::vector<int>& Neighbours = Graph[Vnow];

        for (int Vnext {1}; Vnext <= Neighbours.size(); ++Vnext)
        {
            if (Neighbours[Vnext - 1]) { // Vnow connected with Vnext
                if (Visited.find(Vnext) != Visited.end()) {
                    Visited.insert(Vnext);
                    Queue.push(Vnext);
                }
            }
        }
        Queue.pop();
    }
}


template <
    class Graph_t
>
inline void DFSvoid (
    const Graph_t& Graph,
    int Vnow, // [TODO]: Vnow is Visited.front() always??
    std::set<int>& Visited
) {
    // std::set<int> Visited;
    const std::vector<int>& Neighbours = Graph[Vnow];

    for (int Vnext {1}; Vnext <= Neighbours.size(); ++Vnext) {
       if (Neighbours[Vnext - 1]) { // Vnow connected with Vnex
            if (Visited.find(Vnext) != Visited.end()) {
                // Vnext doesn't be visited before
                Visited.insert(Vnext);
                DFS(Graph, Vnext, Visited);
            }
        }
    }
}


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

enum MST_base_algorithm {
    Prim,
    Kruskal
};



#endif // GRAPHALGO_HPP
