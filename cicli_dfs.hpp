#pragma once //per evitare errori di ridefinizione 
#include<unordered_map>
#include<vector>
#include"lifo.hpp"
#include"unidirected_edges.hpp"
#include"unidirected_graph.hpp"
#include"graph_visit.hpp"


template<typename T>
bool exists_path(const unidirected_graph<T>& tree, const T& u,const T& v, std::unordered_map<T, bool>& visited, lifo<T>& path){  
    visited[u] = true; //contrassegna il primo nodo come trovato
    path.put(u); //lo inserisce nella pila

    if(u==v){ //se è raggiunto il nodo di destinazione
        return true; 
    }

    for(auto& n: tree.neighbours(u)){ //per ogni vicino del nodo
        if(!visited[n]){ //se non è visitato
            if(exists_path(tree, n, v, visited, path)){ //chiama sè stessa
                return true; //propaga il successo comunicandolo al chiamante (istanza di exists_path precedente) e non fa get
            }
        }
    }

    path.get(); //estrae l'elemento dallo stack se il nodo di arrivo non è stato trovato
    return false;
}

template<typename T>
std::vector<unidirected_edge<T>> find_path(const unidirected_graph<T>& albero, const T& u, const T& v){
    std::unordered_map<T, bool> visited;    //per tenere traccia dei nodi visitati
    for(const T& node : albero.all_nodes()) {
        visited[node] = false; //inizializza tutti i nodi come non visitati
    }
    lifo<T> stack; //pila che verrà riempita tramite riferimento dalla funzione helper con i nodi del cammino
    if(!exists_path(albero, u, v, visited, stack)){    //nella chiamata a exists path viene visitato il grafo riempiendo man mano anche lo stack /chiamato per riferimento)
        return {};    //se non trova il percorso ritorna vettore vuoto come flag di errore
    }
    
    std::vector<T> nodes; //creo un vettore di nodi prendendoli dalla pila

    while(!stack.empty()){    //converte da stack a vettore di nodi
        nodes.push_back(stack.get());
    }


    std::vector<unidirected_edge<T>> edges;
    for(int i=nodes.size()-1; i>0; --i){ //converte la sequenza di nodi in una sequenza di archi iterando al contrario perché la pila ( e quindi il vettore di nodi) è al contraio
        edges.push_back(unidirected_edge<T>(nodes[i-1], nodes[i]));    //riempiendo in questo modo il vettore di archi si mantiene l'informazione del verso di percorrenza della maglia
    }
    
    return edges;

}

template<typename T>
std::vector<std::vector<unidirected_edge<T>>> fundamental_cycles(const unidirected_graph<T>& G){
    lifo<T> pila;
    T sorgente = *(G.all_nodes().begin());
    unidirected_graph<T> albero = graph_visit(G, sorgente, pila); //albero DFS
    unidirected_graph<T> coalbero = G-albero;    //coalbero i cui archi si chiamano back edges

    std::vector<std::vector<unidirected_edge<T>>> cicli;

    for(const unidirected_edge<T>& e : coalbero.all_edges()){    //iteriamo sui back edges (ogni back edge corrisponde a un ciclo
        std::vector<unidirected_edge<T>> ciclo = find_path(albero, e.from(), e.to());    //troviamo il percorso tra i 2 nodi nell'albero
        ciclo.push_back(e); //aggiungiamo l'arco del coalbero che chiude il ciclo
        cicli.push_back(ciclo); //push_back aggiunge al fondo del vettore di vettori il ciclo che abbiamo trovato
        
    }

    return cicli;
    
}
