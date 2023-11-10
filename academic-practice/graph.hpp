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
    // +---------------------------------------------------------+
    // |                       Traits                            |
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

    // 1. if new vert nv is nullptr, this will create a new vert
    // 2. insert in graph connecting it with other vertices, all weights = 1
    vertptr extension (const std::vector<vert>& nbrs, const vert new_vert = -1) {
        if ( new_vert != -1 && (unsigned)new_vert < m_Verts.size() ) { return nullptr; }

        int index = (new_vert == -1 ? m_Verts.size() : new_vert);
        m_Verts.push_back(new vert(index));

        resize_2dvector(m_Weightfunc, m_Verts.size(), 0);
        for (const vert& nbr : nbrs) {
            m_Weightfunc[index][nbr] = 1;
            m_Weightfunc[nbr][index] = 1;
            m_Edges.emplace(m_Verts[index], m_Verts[nbr], 1);
            m_Edges.emplace(m_Verts[nbr], m_Verts[index], 1);
        }
        return m_Verts.back();
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
    using weight_type = typename Mybase::weight_type;
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
        Mybase(parallelization(Rhs.m_Weightfunc)), // construct weighted_graph with new weight_funct
        m_Flow(Mybase::m_Weightfunc.size(), std::vector<int>(Mybase::m_Weightfunc.size(), 0)), // init flow as 0
        m_Capacity(Mybase::m_Weightfunc.size(), std::vector<int>(Mybase::m_Weightfunc.size(), 0)) // init capacity
    {
        update_capacity();
    }

    void update_capacity() noexcept {
        index_t countV = Mybase::m_Weightfunc.size();
        const auto& C = Mybase::m_Weightfunc; // main capacity function
                                              // capacity on edges of main graph

        for (index_t u = {0}; u < countV; ++u) {
            for (index_t v = {0}; v < countV; ++v) {
                weight_type cuv = C[u][v]; // capacity on (u,v)
                weight_type cvu = C[v][u]; // capacity on (v,u)

                // m_Capacity[u][v] = (cuv != 0) * (cuv - m_Flow[u][v]) + // u -> v
                //                    (cvu != 0) * (m_Flow[v][u]);        // not u -> v, but v -> u
                                                                       // else not u -> v, not v -> u => = 0

                // ^^^^ equivalent to vvvv
                if (cuv) {                                   // (u,v) \in E
                    m_Capacity[u][v] = cuv - m_Flow[u][v];
                } else if (cvu) {                            // (v,u) \in E
                    m_Capacity[u][v] = m_Flow[v][u];
                } else {                                     // other cases
                    m_Capacity[u][v] = 0;
                }
            }
        }
    }

    std::pair<weight_type, std::vector<vertptr>> find_path (vertptr Source, vertptr Target) noexcept {
        return bfs(Source, Target);
    }


    weight_type flow (vert Source) {
        weight_type flow = 0;
        for (auto const& item : m_Flow[Source]) {
            flow += item;
        }

        return flow;
    }

private:
    std::vector<std::vector<int>> parallelization (const std::vector<std::vector<int>>& dvec) noexcept {
        index_t countV = dvec.size();
        index_t new_size = countV;
        std::vector<std::vector<int>> Result(dvec);

        for (index_t i = {0}; i < countV - 1; ++i) {
            for (index_t j = {i+1}; j < countV; ++j) {
                weight_type weight = dvec[i][j];
                weight_type weight_reverse = dvec[j][i];

                if (weight && weight_reverse) {                                  // sou -> tar AND tar -> sou
                    Result[j][i] = 0;                                            // destroy: tar -> sou
                    Result.push_back(std::vector<weight_type>(++new_size, 0));   // create: vert(new_vert) with no connections
                    Result.back()[i] = weight_reverse;                           // create: new_vert -> sou

                    for (index_t k = {0}; k < new_size-1; ++k) {                 // resizing vector to size+1
                        Result[k].push_back(0);                                  // disconnect others with new_vert
                    }
                    Result[j][new_size-1] = weight_reverse;                      // create: tar->new_vert
                }
            }
        }
        return Result;                                                           // sou -> tar AND tar -> new_vert -> sou
    }

#if 1
    std::pair<weight_type, std::vector<vertptr>> bfs (vertptr Source, vertptr Target) noexcept {
        auto countV = Mybase::m_Verts.size();
        std::vector<vertptr> Parents(countV, nullptr);
        std::vector<bool>    Visited(countV, 0);
        bool                 is_find_path = false;

        std::queue<vertptr> Queue;
        Queue.push(Source);
        Visited[*Source] = true;
        Parents[*Source] = nullptr;

        while (!is_find_path && !Queue.empty()) {
            vertptr Vnow = Queue.front();

            std::vector<vert> Nbrs = m_Capacity[*Vnow];
            for (index_t nbrNo = {0}; nbrNo < Nbrs.size(); ++nbrNo) {
                weight_type ci = Nbrs[nbrNo]; // capacity
                if (ci) { // capacity != 0
                    if (!Visited[nbrNo]) {
                        Visited[nbrNo] = true;
                        Parents[nbrNo] = Vnow;
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

        if (is_find_path) {
            std::vector<vertptr> Path(0);
            weight_type min_capacity = (is_find_path ? std::numeric_limits<weight_type>::max() : 0);

            vertptr Vnow = Target;

            while (Vnow != nullptr) {
                Path.push_back(Vnow);
                vertptr Vparent = Parents[*Vnow];
                if (Vparent) {
                    min_capacity = (min_capacity > m_Capacity[*Vparent][*Vnow] ? m_Capacity[*Vparent][*Vnow] : min_capacity);
                }
                Vnow = Vparent;
            }
            std::reverse(Path.begin(), Path.end());
            return std::make_pair(min_capacity, Path);
        } else {
            return std::make_pair(0, std::vector<vertptr>(0, nullptr));
        }
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
