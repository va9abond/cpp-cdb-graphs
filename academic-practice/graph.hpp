#ifndef GRAPH_HPP
#define GRAPH_HPP


#include <iostream>
#include <cassert>
#include <limits>
#include <algorithm>
#include <queue>
#include "vector_utils.hpp"

// [TODO]: different ways to store graph
// [TODO]: tree is derived class from Grap_base_ with static_assert(is_tree);
// [TODO]: add proxy to verify is vertice in given graph

struct edge_base_ {
    // +---------------------------------------------------------+
    /* | */ using vert    = int;                              // |
    /* | */ using vertptr = vert*;                            // |
    // +---------------------------------------------------------+


    edge_base_(vertptr Vs, vertptr Vt) : sou(Vs), tar(Vt) {}

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
    using vertptr     = Mybase::vertptr;
    using wedge       = edge<weight_type>;
    // using Mybase::Mybase;

    // generate graph with Count verts and zero weight function
    // Graph = { verts:Count, wedges:0,  weight_funct==0 }
    weighted_graph (int Count) : Graph_base_(Count), m_Weightfunc(Count, std::vector<int>(Count, 0)) {}

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
            for (int j {0}; j < Mybase::sizeV(); ++j) {
                weight_type weight = m_Weightfunc[i][j];
                if (weight) { // verts i and j are connected
                    m_Edges.emplace(m_Verts[i], m_Verts[j], weight);
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
};

template <
    class Weight_t
>
struct residual_network : weighted_graph<Weight_t> // flow_network
{
    using Mybase      = weighted_graph<Weight_t>;
    using weight_type = Weight_t;
    using size_type   = typename Mybase::size_type;
    using vert        = typename Mybase::vert;
    using vertptr     = typename Mybase::vertptr;
    using wedge       = edge<weight_type>;
    using index_t     = typename std::vector<std::vector<weight_type>>::size_type;


    // generate flow
    // [WARNING]: parallelization of edges:
    //            if there are (sou --> tar and tar --> sou) then
    //            tar --> sou converts to tar --> new_vert --> sou
    residual_network (const weighted_graph<weight_type>& Rhs) :
        Mybase(Edges_parallelization(Rhs.m_Weightfunc)), // construct weighted_graph with new weight_funct
        m_Flow(Mybase::m_Weightfunc.size(), std::vector<int>(Mybase::m_Weightfunc.size(), 0)), // init flow as 0
        m_Capacity(Mybase::m_Weightfunc.size(), std::vector<int>(Mybase::m_Weightfunc.size(), 0)) // init capacity
    {
        update_capacity();
    }

    void update_capacity() noexcept
    {
        index_t countV = Mybase::m_Weightfunc.size();
        const auto& M = Mybase::m_Weightfunc;

        for (index_t i = {0}; i < countV - 1; ++i) {
            for (index_t j = {0}; j < countV; ++j) {
                weight_type weight = M[i][j];
                weight_type weight_reverse = M[j][i];
                m_Capacity[i][j] = (weight != 0) * (weight - m_Flow[i][j]) + // i -> j
                                   (weight_reverse != 0) * (m_Flow[j][i])  + // not i -> j, but j -> i
                                    0;                                       // not i -> j, not j -> i
                // ^^^^ equivalent to vvvv
                // if (weight) {
                //     m_Capacity[i][j] = weight - m_Flow[i][j];
                // } else if (weight_reverse) {
                //     m_Capacity[i][j] = m_Flow[j][i];
                // } else {
                //     m_Capacity[i][j] = 0;
                // }
            }
        }
    }

    decltype(auto) find_path (vertptr Source, vertptr Target) noexcept
    {
        return bfs(Source, Target);
    }


private:
    std::vector<std::vector<int>> Edges_parallelization (const std::vector<std::vector<int>>& dvec) noexcept
    {
        index_t countV = dvec.size();
        index_t last_index = countV - 1;
        std::vector<std::vector<int>> Result(dvec);

        for (index_t i = {0}; i < countV - 1; ++i) {
            for (index_t j = {i+1}; j < countV; ++j) {
                weight_type weight = dvec[i][j];
                weight_type weight_reverse = dvec[j][i];

                if (weight && weight_reverse) {                                  // sou -> tar AND tar -> sou
                    Result[j][i] = 0;                                            // destroy: tar -> sou
                    Result.push_back(std::vector<weight_type>(++last_index, 0)); // create:  new vert with no connects
                    Result.back()[i] = weight_reverse;                           // create: new_vert -> sou

                    for (index_t k = {0}; k < Result.size(); ++k) {
                        Result[k].push_back(0);                                  // disconnect others witch new_vert
                    }

                    Result[j][last_index] = weight_reverse;                      // create:: tar->new_vert
                }
            }
        }
        return Result;
    }

    // [TODO]: 3 nested if!!!
#if 1
    decltype(auto) bfs (vertptr Source, vertptr Target) noexcept
    {
        auto countV = Graph_base_::m_Verts.size();
        std::vector<vertptr> Parents(countV, nullptr);
        std::vector<bool>    Visited(countV, 0);
        bool                 is_find_path = false;

        std::queue<vertptr> Queue;
        Queue.push(Source);
        Visited[*Source] = true;
        Parents[*Source] = nullptr;

        while (!is_find_path && !Queue.empty()) {
            vertptr Vnow = Queue.front();
            Visited[*Vnow] = true;

            std::vector<vert> Nbrs = m_Capacity[*Vnow];
            for (index_t nbrNo = {0}; nbrNo < Nbrs.size(); ++nbrNo) {
                weight_type ci = Nbrs[nbrNo]; // capacity
                if (ci) { // capacity != 0
                    Parents[nbrNo] = Vnow;
                    if (!Visited[nbrNo]) {
                        if ((int)nbrNo == *Target) {
                            is_find_path = true; break;
                        } else {
                            Queue.push(Mybase::m_Verts[nbrNo]);
                        }
                    }
                }
            }
            Queue.pop();
        }

        std::vector<vertptr> Path(0);
        weight_type min_capacity = (is_find_path ? std::numeric_limits<weight_type>::max() : 0);
        if (is_find_path) {
            vertptr Vnow = Target;

            while (Vnow != nullptr) {
                Path.push_back(Vnow);
                vertptr Vparent = Parents[*Vnow];
                // std::cout << "\n=======" << *Vparent;
                if (Vparent) {
                    min_capacity = (min_capacity > m_Capacity[*Vparent][*Vnow] ? m_Capacity[*Vparent][*Vnow] : min_capacity);

                }

                Vnow = Vparent;
            }
            std::reverse(Path.begin(), Path.end());
        }
        return std::make_pair(min_capacity, Path);
    }
#endif

public:
    std::vector<std::vector<weight_type>> m_Flow;     // resulting flow
    std::vector<std::vector<weight_type>> m_Capacity; // residual capacity
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
