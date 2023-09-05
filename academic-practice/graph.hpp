#ifndef GRAPH_HPP
#define GRAPH_HPP


#include <iostream>
#include <cassert>
#include <limits>
#include "vector_utils.hpp"

// [TODO]: different ways to store graph
// [TODO]: tree is derived class from Grap_base_ with static_assert(is_tree);


struct edge_base_ {
    // +---------------------------------------------------------+
    /* | */ using vert    = int;                              // |
    /* | */ using vertptr = vert*;                            // |
    // +---------------------------------------------------------+


    edge_base_ (vertptr Vs, vertptr Vt) : sou(Vs), tar(Vt) {}

    ~edge_base_() {
        sou = nullptr;
        tar = nullptr;
    }

    // edge_base_ (const edge_base_&)            = delete;
    // edge_base_& operator= (const edge_base_&) = delete;


    vertptr sou; // source vertice
    vertptr tar; // target vertice
};


template <
    class Weight_t
>
struct edge : edge_base_ {
    using Mybase  = edge_base_;
    using vert    = Mybase::vert;
    using vertprt = Mybase::vertptr;


    edge (vert* Vs, vert* Vt, Weight_t Weight) : edge_base_(Vs, Vt), wei(Weight) {}

    ~edge() {
        wei = Weight_t {0};
    }

    vert* source() noexcept { return Mybase::sou; }
    vert* target() noexcept { return Mybase::tar; }

    bool operator== (const edge& Rhs) const noexcept {
        return (Mybase::tar == Rhs.tar) * (Mybase::sou == Rhs.sou) * (wei == Rhs.wei);
    }

    // if weigths is equal, then random
    bool operator< (const edge& Rhs) const noexcept {
        if (wei != Rhs.wei) {
            return wei < Rhs.wei;
        } else {
            return (*sou != *Rhs.sou ? *sou < *Rhs.sou : *tar < * Rhs.tar);
        }
    }

    bool operator<= (const edge& Rhs) const noexcept {
        return (*this < Rhs) + (*this == Rhs);
    }

    bool operator> (const edge& Rhs) const noexcept {
        return !(*this < Rhs);
    }

    bool operator>= (const edge& Rhs) const noexcept {
        return !(*this < Rhs) + (*this == Rhs);
    }


    Weight_t wei; // weight of edge
};


struct Graph_base_ {
    //                         Traits
    // +---------------------------------------------------------+
    /* | */   using vert      = int;                          // |
    /* | */   using vertptr   = vert*;                        // |
    /* | */   using size_type = std::set<vert*>::size_type;   // |
    // +---------------------------------------------------------+
    // [WARNING]: Despite the fact that size_type is usigned long
    //            for std::set<vert*>, we allow to create just
    //            INT_MAX vertice!

    Graph_base_ (int Count = 0) : m_Verts() {
        assert(Count >= 0 ||
               Count <= std::numeric_limits<int>::max() ||
               "Invalid Value: Count");
        Construct_verts(Count);
    }

    Graph_base_ (std::vector<vert*> Rhs) : m_Verts(Rhs) {}

    void add_n_verts (int Count) noexcept {
        assert(Count >= 0 ||
               Count <= std::numeric_limits<int>::max() - this->sizeV() ||
               "Invalid Value: Count");
        Construct_verts(Count);
    }

    // Graph_base_& operator= (const Graph_base_&) = delete;
    // Graph_base_ (const Graph_base_&)            = delete;

    ~Graph_base_() {
        // [NOTE]: does it really work? (= yes, valgrind approves)
        for (vert vi = 0; vi < (int)m_Verts.size(); ++vi) {
            delete m_Verts[vi];
            m_Verts[vi] = nullptr;
        }
        m_Verts.clear();
    }

    int sizeV() const noexcept { return (int)m_Verts.size(); }

private:
    void Construct_verts (int Count) noexcept {
        int startNo = m_Verts.size();
        int newsize = startNo + Count;

        m_Verts.resize(newsize);

        for (int i {startNo}; i < newsize; ++i) {
            m_Verts[i] = new vert(i);
       }
    }

public:
    // [WARNING]: m_Verts MUST be sorted manually!
    std::vector<vertptr> m_Verts;
};


// graph with a function of weights on edges
template <
    class Weight_t
>
struct weighted_graph : Graph_base_ {
    using Mybase      = Graph_base_;
    using weight_type = Weight_t;
    using size_type   = Mybase::size_type;
    using vert        = Mybase::vert;
    using wedge       = edge<weight_type>;

    // generate graph with Count verts and zero weight function
    // Graph = { verts:Count, wedges:0,  weight_funct==0 }
    weighted_graph (int Count) : Graph_base_(Count) {
        m_Weightfunc = std::vector<std::vector<int>>(Count, std::vector<int>(Count, 0));
    }

    // generate graph with weight function from file
    weighted_graph (std::string Filename) : Graph_base_() {
        m_Weightfunc = generate_2dvector_from_file(Filename);
        Mybase::add_n_verts( (int)m_Weightfunc.size() );
        Construct_edges();
    }

    weighted_graph (std::vector<std::vector<int>> Weightfunc) :
        m_Weightfunc(Weightfunc),
        m_Edges()
    {
        Mybase::add_n_verts( (int)m_Weightfunc.size() );
        Construct_edges();
    }

    // [WARNING]: shallow copy only provided!
    weighted_graph (const weighted_graph& Rhs) :
        Graph_base_(Rhs.m_Verts),
        m_Weightfunc(Rhs.m_Weightfunc),
        m_Edges(Rhs.m_Edges)
    {}

    ~weighted_graph() {
        m_Edges.clear();
        m_Weightfunc.clear();
    }

    int sizeE() const noexcept {
        return (int)m_Edges.size();
    }

private:
    // construct edges by weight function
    void Construct_edges() noexcept {
        for (int i {0}; i < Mybase::sizeV(); ++i) {
            for (int j {i + 1}; j < Mybase::sizeV(); ++j) {
                weight_type weight = m_Weightfunc[i][j];
                // std::cout << "i = " << i << ", j = " << j << "  [i][j] = " << m_Weightfunc[i][j]; // (c)
                if (weight) { // verts i and j are connected
                    // std::cout << " +"; // (c)
                    m_Edges.emplace(m_Verts[i], m_Verts[j], weight);
                }
                // std::cout << "\n"; // (c)
            }
        }
    }

public:
    // weighted_graph must have a weight function on it's edges;
    // weight of edge e is equals m_Weightfunc[e.sou][e.tar]
    mutable std::vector<std::vector<Weight_t>> m_Weightfunc;
            std::set<wedge>                    m_Edges;
    // [NOTE]: strict order by wedge::operator<
};


// no weights on edges, just 1 if i, j verts connected, else 0
struct general_graph : weighted_graph<bool> {};


// all edges have source and target
template <
    class Weight_t
>
struct oriented_graph : weighted_graph<Weight_t> {
    using Mybase      = weighted_graph<Weight_t>;
    using weight_type = bool;
    using size_type   = typename Mybase::size_type;
    using vert        = typename Mybase::vert;
    using vertptr     = typename Mybase::vertptr;
    using Mybase::Mybase;
};


#endif // GRAPH_HPP
