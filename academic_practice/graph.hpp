#ifndef GRAPH_HPP
#define GRAPH_HPP


#include "vector_utils.hpp"
#include <cassert>
#include <limits>

// [TODO]: different ways to store graph
// [TODO]: tree is derived class from Grap_base_ with static_assert(is_tree);


struct edge_base_ {
    // +---------------------------------------------------------+
    /* | */ using vert = int;                                 // |
    // +---------------------------------------------------------+


    edge_base_(vert* Vs, vert* Vt) : sou(Vs), tar(Vt) {}

    ~edge_base_() {
        sou = nullptr;
        tar = nullptr;
    }

    edge_base_ (const edge_base_&)            = delete;
    edge_base_& operator= (const edge_base_&) = delete;


    vert* sou; // source vertice
    vert* tar; // targer vertice
};


struct Graph_base_ {
    //                         Traits
    // +---------------------------------------------------------+
    /* | */   using vert      = int;                          // |
    /* | */   using size_type = std::set<vert*>::size_type;   // |
    // +---------------------------------------------------------+
    // [WARNING]: Despite the fact that size_type is usigned long
    //            for std::set<vert*>, we allow to create just
    //            INT_MAX vertice!

    Graph_base_ (int Count = 0) : m_Verts() {
        assert(Count > 0 &&
               Count <= std::numeric_limits<int>::max() &&
               "Invalid Value: Count");
        Construct_verts(Count);
    }

    void add_n_verts (int Count) noexcept {
        assert(Count > 0 &&
               Count <= std::numeric_limits<int>::max() - this->sizeV() &&
               "Invalid Value: Count");
        int startNo = 1 + m_Verts.size();
        Construct_verts(Count, startNo);
    }

    Graph_base_& operator= (const Graph_base_&) = delete;
    Graph_base_ (const Graph_base_&)            = delete;

    ~Graph_base_() {
        // [NOTE]: does it really work? (= yes, valgrind approves)
        // [TODO]: is there a better way?
        for (auto Vit = m_Verts.begin(); Vit != m_Verts.end(); ++Vit) {
            delete *Vit;
        }
    }

    int sizeV() const noexcept { return (int)m_Verts.size(); }

private:
    void Construct_verts (int Count, int startNo = 1) noexcept { // [TODO]: is there a better way?
        for (int i {0}; i < Count; ++i) {
            vert* Vnew = new vert(startNo + i);
            m_Verts.push_back(Vnew);
        }
    }

public:
    std::vector<vert*> m_Verts;
};


template <
    class Weight_t
>
struct edge : edge_base_ {
    using Mybase = edge_base_;
    using vert   = Mybase::vert;

    edge (vert* Vs, vert* Vt, Weight_t Weight) : edge_base_(Vs, Vt), wei(Weight) {}

    ~edge() {
        wei = Weight_t {0};
    }

    vert* source() noexcept { return Mybase::sou; }
    vert* targer() noexcept { return Mybase::tar; }

    bool operator== (const edge& Rhs) const noexcept {
        return (Mybase::tar == Rhs.tar) * (Mybase::sou == Rhs.sou) * (wei == Rhs.wei);
    }

    bool operator< (const edge& Rhs) const noexcept {
        return wei < Rhs.wei;
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

    // [WARNING]: shallow copy only provided!
    weighted_graph (const weighted_graph& Rhs) : Graph_base_(Rhs.m_Verts), m_Edges(Rhs.m_Edges) {}

    int sizeE() const noexcept {
        return (int)m_Edges.size();
    }

private:
    // construct edges by weight function
    void Construct_edges() noexcept {
        for (int i {0}; i < Mybase::sizeV(); ++i) {
            for (int j {i + 1}; j < Mybase::sizeV(); ++j) {
                weight_type weight = m_Weightfunc[i][j];
                if (weight) { // verts i and j are connected
                    m_Edges.emplace(m_Verts[i-1], m_Verts[j-1], weight);
                }
            }
        }
    }

public:
    // weighted_graph must have a weight function on it's edges;
    // weight of edge e is equals m_Weightfunc[e.sou][e.tar]
    mutable std::vector<std::vector<Weight_t>> m_Weightfunc;
            std::set<wedge>                    m_Edges;
    // [NOTE]: strict order by wedge::operator<
    // [TODO]: or multimap to find by keys (weight) its useful for MST??
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
    using Mybase::Mybase;
};


#endif // GRAPH_HPP
