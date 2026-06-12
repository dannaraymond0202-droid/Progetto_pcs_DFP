#pragma once
//per evitare errori di ridefinizione 
#include<unordered_map>
#include<map>
#include<queue>
#include<vector>
#include"fifo.hpp"
#include"lifo.hpp"
#include"unidirected_edges.hpp"
#include"unidirected_graph.hpp"
//Dijkstra modificata per esercitazione 11
//l'ordine di visita è l'ordine con cui tolgo gli elementi dal contenitore

template<typename T, typename Contenitore>
unidirected_graph<T> graph_visit(const unidirected_graph<T>& G, const T& sorgente, Contenitore& c) {  //BFS e DFS
    unidirected_graph<T> albero;
    std::unordered_map<T, bool> reached;

    for (auto& nodo : G.all_nodes()){ //inizializza tutti i nodi a false
        reached[nodo] = false;
    }

    c.put(sorgente);  //inserisco il nodo sorgente in fondo alla coda o in cima alla pila
    reached[sorgente] = true; //assegno nella mappa dei nodi trovati al nodo sorgente il valore di true

    while(!c.empty()){
        T u = c.get(); //prendo il primo elemento della coda o della pila

        for(auto& v : G.neighbours(u)){ //prendo i vicini del nodo
            if(!reached[v]){ //se sono false
                c.put(v); //inserisco nel contenitore
                reached[v] = true; //assegno loro il valore di true
                albero.add_edge(u, v); //aggiunge arco all'albero di visita
            }
        }
    }

    return albero; //che sarà diverso se la visita è in ampiezza o in profondità

}

template<typename T>
void dfs_visita(const unidirected_graph<T>& G, const T& u, std::unordered_map<T, bool>& reached, unidirected_graph<T>& albero){  //funzione di suporto per la parte ricorsiva della DFS
    reached[u] = true;    //assegno nella mappa dei nodi trovati al nodo sorgente il valore di true
        for(auto& v : G.neighbours(u)){ 
            if(!reached[v]){    //se questo vicino di u non è ancora stato visitato...
                albero.add_edge(u, v); //aggiunge arco all'albero di visita
                dfs_visita(G, v, reached, albero);    //e richiama un'istanza di se stessa sul nuovo nodo sorgente, vicino del precedente nodo sorgente
            }
        }
}

template<typename T>
unidirected_graph<T> recursive_dfs(const unidirected_graph<T>& G, const T& sorgente){    //funzione che chiama l'utente, dato che richiede in input solo grafo e nodo sorgente
    unidirected_graph<T> albero;
    std::unordered_map<T, bool> reached;
    for (auto& nodo : G.all_nodes()) {
        reached[nodo] = false;    //tutti i nodi sono inizialmente non raggiunti
    }

    dfs_visita(G, sorgente, reached, albero);
    return albero;
}

template<typename T> //aggiungo in input la mappa delle distanze dei nodi dalla sorgente e la modifico per riferimento
std::map<T,T> Dijkstra(const unidirected_graph<T>& G, const T& sorgente, const std::map<unidirected_edge<T>, double>& pesi, std::map<T, double>& dist){
    std::map<T, T> pred;       //predecessore di ogni nodo
    std::priority_queue<std::pair<double, T>, std::vector<std::pair<double,T>>, std::greater<std::pair<double, T>>> PQ; //creo la priority queue che ordini dalla distanza minore alla maggiore

    dist[sorgente]=0.0; //la distanza della sorgente a se stessa è 0
    pred[sorgente] = sorgente; //il predecessore della sorgente è sé stessa
    PQ.push({0.0, sorgente}); //aggiungo alla PQ
    

    while(!PQ.empty()){
        double d = PQ.top().first; //accedo alla distanza del primo elemento nella PQ 
        T u = PQ.top().second; //accedo al nodo del primo elemento nella PQ 
        PQ.pop(); //lo rimuovo

        if(d == dist[u]){  //siccome nella PQ possono esserci distanze diverse associate allo stesso nodo controllo che d non sia obsoleta (può capitare che d sia maggiore di una dist già aggiornata migliore)
            for(T w : G.neighbours(u)){
                unidirected_edge<T> e(u,w);
                double peso = pesi.at(e); //non posso accedere ai valori della map tramite [] perché potrebbe potenzialmente scriverne di nuovi
                double nuova_dist = dist[u] + peso;

                //count restituisce 0 se non lo trova 1 se lo trova, verifichiamo e aggiorniamo la distanza se ancora non c'è la distanza associata al nodo w
                if(!dist.count(w) || nuova_dist < dist[w] ){ //verifichiamo se dist[w] non esiste o se è maggiore della nuova distanza
                    dist[w] = nuova_dist; //aggiorno la distanza
                    pred[w] = u; //aggiorno i predecessori
                    PQ.push({dist[w], w}); //aggiungo alla PQ
                }
            }
        }
    }
    return pred; //ritorna la mappa che assegna a ogni nodo il suo predecessore, in questo modo possiamo costruire tutti i cammini minimi

}
