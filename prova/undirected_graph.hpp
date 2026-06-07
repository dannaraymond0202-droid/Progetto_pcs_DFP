#pragma once
#include <concepts>
#include "undirected_edges.hpp"
#include <set>
#include <map>

using namespace std;

template<typename I> requires std::totally_ordered<I>   /* garantisce che il tipo dei nodi abbia una relazione d'ordine definita,
come necessario per l'ordinamento degli archi */
class undirected_graph
{
    set<undirected_edge<I>> edges;
    map<I, set<I>> lista_adi;
    map<undirected_edge<I>, int> edge_index;

public:
    /* Costruttore di default */
    undirected_graph() : edges(set<undirected_edge<I>>{}), lista_adi(map<I, set<I>>{}), edge_index(map<undirected_edge<I>, int>{})
    {}

    /* Costruttore di copia */
    undirected_graph(const undirected_graph<I>& other) : edges(other.edges), lista_adi(other.lista_adi), edge_index(other.edge_index)
    {}

    set<I> neighbours(const I& node) const { return lista_adi.at(node); }

    void add_edge(undirected_edge<I>& nuovo){
        edges.insert(nuovo);
        lista_adi[nuovo.from()].insert(nuovo.to());
        lista_adi[nuovo.to()].insert(nuovo.from());
        edge_index[nuovo]=edge_index.size();
    }

    set<undirected_edge<I>> all_edges() const { return edges; }

    set<I> all_nodes() const{
        set<I> nodi;
        for(auto& [nodo, adiacenti] : lista_adi){
            nodi.insert(nodo);
        }
        return nodi;
    }

    int edge_number(const undirected_edge<I>& edge) const {
        if(edge_index.contains(edge)) return edge_index.at(edge);
        else return -1;     //sentinella per segnalare arco in input non valido
    }

    undirected_edge<I> edge_at(const int& n) const {
        for(auto& [edge, num] : edge_index){
            if(n==num) return edge;
        }
        return undirected_edge<I>{}; //arco vuoto come sentinella
    }

    undirected_graph<I> operator-(const undirected_graph<I>& other) const {
        undirected_graph ret=*this;
        for(auto& edge : other.edges){
            if(ret.edges.contains(edge)){
                ret.edges.erase(edge);
                ret.lista_adi.at(edge.from()).erase(edge.to());
                ret.lista_adi.at(edge.to()).erase(edge.from());
                ret.edge_index.erase(edge);
            }
        }
        return ret;
    }

};
