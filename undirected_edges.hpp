#pragma once
#include <concepts>
using namespace std;

template<typename I> requires std::totally_ordered<I>   /* garantisce che il tipo dei nodi abbia una relazione d'ordine definita,
come necessario per l'ordinamento degli archi */
class undirected_edge
{
    I node_from;
    I node_to;

public:
    /* Costruttore di default */
    undirected_edge() : node_from(I{}), node_to(I{})
    {}

    /* Costruttore user-defined */
    undirected_edge(const I& node1, const I& node2) : node_from(node1), node_to(node2)
    {
        if(node_from>node_to){
            I temp=node_from;
            node_from=node_to;
            node_to=temp;
        }
    }

    /* Restituiscono i valori di nodo di partenza e di arrivo */
    I from() const { return node_from; }
    I to() const { return node_to; }

    /* Implementazione < */
    bool operator<(const undirected_edge& other) const {
        if (node_from != other.from()) return node_from < other.from();
        else return node_to < other.to();
    }
    
    /* Implementazione == */
    bool operator==(const undirected_edge& other) const {
        return (node_from==other.from() && node_to==other.to());
    }

};

template <typename I>
ostream& operator<<(ostream& os, const undirected_edge<I>& e)
{
    os << "(" << e.from() << ")---(" << e.to() << ")";
    return os;
}
