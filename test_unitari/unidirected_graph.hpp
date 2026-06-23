#pragma once //serve per non avere errori di ridefinizione della classe quando la chiamiamo più volte
#include<map>
#include<set>
#include<list>
#include<algorithm>    //aggiunto per la funzione find
#include"unidirected_edges.hpp"
#include <iostream>

template <typename T>
class unidirected_graph {
private:
    std::map<T, std::set<T>> adj;  //crea una mappa (un dizionario) la cui chiave è un nodo e il valore un insieme (quello dei suoi vicini)
    std::list<unidirected_edge<T>> edges; //lista di tutti gli archi in ordine di inserimento

public:

    unidirected_graph() = default;  //costruttore di default, crea un grafo vuoto

    unidirected_graph(const unidirected_graph& other) //costruttore di copia, va a creare una copia indipendente (copia other)
        : adj(other.adj), edges(other.edges) {} //lista di inizializzazione che copia la lista di adiacenza e i lati


    std::set<T> neighbours(T n) const { //restituisce un insieme di nodi (i vicini), const perché non modifica il grafo
        auto it = adj.find(n); //cerca nella map adj la chiave n, se non la trova l'iteratore diventa uguale a adj.end()
        if (it==adj.end()){
            return {}; //se non lo trova restituisce un insieme vuoto
        }
        return (*it).second; //se lo trova accediamo al secondo elemento dell'iteratore ossia il valore del dizionario (quindi l'insieme dei vicini)
    }


    void add_edge(T a, T b) { //aggiunge un arco al grafo, i nodi vengono aggiunti implicitamente
        unidirected_edge<T> e(a,b);
        
        auto i = find(edges.begin(), edges.end(), e);
        if(i == edges.end()) edges.push_back(e); //se non l'ho trovato lo aggiungo alla fine della lista
        //aggiorno la lista di adiacenza
        adj[e.from()].insert(e.to()); //aggiungo "b" alla lista di vicini di "a"
        adj[e.to()].insert(e.from()); //aggiungo "a" alla lista di vicini di "b
    }


    std::list<unidirected_edge<T>> all_edges() const { //restituisce la lista di tutti gli archi
        return edges; //fa una copia della lista di archi
    }

    std::set<T> all_nodes() const { //restituisce l'insieme di tutti i nodi
        std::set<T> nodes; //insieme vuoto che conterrà i nodi (unici e ordinati)

        //itero sulla mappa e prendo solo le chiavi (i nodi)
        for(auto it = adj.begin(); it!=adj.end(); ++it) {
            nodes.insert((*it).first);
        }
        return nodes;
    }
    
    int edge_number(const unidirected_edge<T>& e) const { //dato un arco restituisce la sua numerazione all'interno del grafo (per list è l'ordine di inserimento)
        int i = 0;
        for(auto it=edges.begin(); it!=edges.end(); ++it) {
            if(*it==e) return i;
            ++i;
        }
        return -1; //se restituisce -1 significa che l'arco non è stato trovato
    }

    unidirected_edge<T> edge_at(int i) const { //dato un indice restituisce il corrispondente arco
        //verifichiamo che l'indice non sia negativo
        if(i<0) {
            return edges.front(); //mi restituisce il primo arco anche se non corrisponderebbe a niente perché deve restituire un arco
        }

        int pos = 0;
        for(auto it= edges.begin(); it!=edges.end(); ++it) {//pos aumenta e tiene traccia dell'indice dell'elemento corrente durante lo scorrimento dell'iteratore
            if(pos == i) return *it;
            ++pos;
        }
        return edges.back(); //mi restituisce l'ultimo arco anche se non corrisponderebbe a niente perché deve restituire un arco
    }

    unidirected_graph operator-(const unidirected_graph& other) const { //restituisce gli archi presenti nel primo grafo ma non nel secondo
        unidirected_graph result; //dichiaro una variabile con costruttore di default
        for(auto it=edges.begin(); it!=edges.end(); ++it){ //itera sugli archi del grafo a sinistra del -, this
            bool found = false;
            for(auto oit=other.edges.begin(); oit!= other.edges.end(); ++oit) { //itera sugli archi del grafo a destra del -, other
                if(*it == *oit) {
                    found = true; //l'arco è presente anche nel secondo grafo
                    break; //esce dal ciclo interno
                }
            }
            
            //se l'arco del primo grafo non è stato trovato in other lo aggiungiamo a result
            if(found == false){
                result.add_edge((*it).from(), (*it).to());
            }
        }

        return result;

    }
};
