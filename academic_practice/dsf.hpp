/* This file provide implementation of Disjoint-set Union (Disjoint-set tree)
with Union by rand and Path compression specialized for graph structure
which implemented in file "graph.hpp" */


#ifndef DSF_HPP
#define DSF_HPP


#include <vector>
#include "graph.hpp"


using vert = Graph_base_::vert;


struct dsf { // disjoint-set union
             // disjoint-set forest + union by rank + path compression

    // [WARNING]: Count shoud equals m_Verts.size() else
    //            make_set can cause segmentation error
    dsf (int Count) : parents(Count) {}

    ~dsf() {
        parents.clear();
    }

    void make_set (vert* V) {
        parents[*V] = V;
        sizes[*V] = 1;
    }

    // [NOTE]: path compression
    vert* find_set (vert* V) noexcept {
        if (V != parents[*V]) { // V is not root
            return parents[*V] = find_set(parents[*V]);
        }
        return V;
    }

    void unite_sets (vert* V, vert* U) {
        vert* Vroot = find_set(V);
        vert* Uroot = find_set(U);

        if (Vroot != Uroot) {
            if (sizes[*Vroot] < sizes[*Uroot]) {
                std::swap(Vroot, Uroot);
            }
            parents[*Uroot] = Vroot;
            sizes[*Vroot] += sizes[*Uroot];
        }
    }


    // [NOTE]: This vector present one of parents for each vertice, i.e.
    //         parent of vertice v is parents[*v]. If vertice u is
    //         root of tree then parents[*u] is u
    std::vector<vert*> parents; // parents set of each disjoint-set tree
    std::vector<int>   sizes; // sizes of each disjoint-set tree in forest
};


#endif // DSF_HPP
