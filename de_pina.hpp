#pragma once //per evitare errori di ridefinizione 
#include<vector>
#include<map>
#include<set>
#include<queue>
#include"lifo.hpp"
#include"unidirected_edge.hpp"
#include"unidirected_graph.hpp"
#include"graph_visit.hpp"

//prodotto scalare modulo 2
int ps_mod2(const std::vector<int>& a, const std::vector<int>& b) {
    int somma = 0;
    for(int i=0; i < a.size(); ++i){
        somma += a[i]*b[i]; //prodotto scalare
    }
    int mod = somma % 2; //modulo 2: 0 se pari, 1 se dispari
    return mod;
}

//differenza simmetrica vettori binari(XOR)
std::vector<int> diff_simmetrica(const std::vector<int>& a, const std::vector<int>& b){
    std::vector<int> res(a.size()); //inizializzo il vettore di dimensione uguale ad a
    for(int i=0; i < a.size(); ++i){
        if(a[i] != b[i]){
            res[i] = 1;
        }
        else{
            res[i] = 0;
        }
    }
    return res;
}

//creo una classe di lfted nodes
template<typename T>
class LiftedNode {
    private: 
    T vertice;
    bool segno; // false = "-" true = "+"

public:

    LiftedNode() {  //costruttore di default
        vertice = T();
        segno = false;
    }

    LiftedNode(const T& v, bool s){ //costruttore
        vertice = v;
        segno = s;
    }

    T getVertice() const {return vertice;} //getter
    bool getSegno() const {return segno;}

    bool operator<(const LiftedNode& other) const { //confronta this con other (riferimento costante a LiftedNode)   confronta due nodi
        if (vertice < other.vertice) {return true;}    //dentro la classe non serve usare il getter possiamo accedere ai membri privati
        if (other.vertice < vertice ) {return false;} 
        else{
            return segno < other.segno;  //false = 0 true = 1    (quindi - verrà prima di +)
        }
    }

    bool operator==(const LiftedNode& other) const { //operatore per definire quando un nodo è uguale a un altro
    if((vertice == other.vertice)&&(segno == other.segno)){
        return true;
    }
    else {
        return false;
    }
}
};


//trova il ciclo minimo C (vettore di incidenza del ciclo) con <C, S> = 1 usando Dijkstra
template<typename T>
std::vector<int> trova_ciclo_minimo(const unidirected_graph<T>& G, const std::vector<int>& S, const std::map<unidirected_edge<T>, double>& pesi_originali){

    std::list<unidirected_edge<T>> archi_tot = G.all_edges(); //lista di tutti gli archi (all_edges restituisce una lista, scelto perché mantiene l'ordine di inserimento)
    int m = 0;
    for(auto it=archi_tot.begin(); it!= archi_tot.end(); ++it){ //conto il numero di archi
        ++m;
    }

    std::set<T> nodi_originali = G.all_nodes(); //numero i vertici da 0 a n-1  (all_nodes restituisce un set scelto perché garantisce l'unicità)

    //costruzione del grafo lifted
    unidirected_graph<LiftedNode<T>> G_lifted;
   
    std::map<unidirected_edge<LiftedNode<T>>, double> pesi_lifted;

    //aggiungo archi nel lifting
    for(auto it= archi_tot.begin(); it!= archi_tot.end(); ++it){
        T u = (*it).from();
        T v = (*it).to();

        int indice = G.edge_number(*it); //prendo l'indice dell'arco nel grafo originale (per ogni arco)
        bool attivo = (S[indice] == 1); //S è il vettore di adiacenza, se S all'indice corrente è 1 allora attivo = true (se S[indice] == 0 allora attivo = false)

        double peso = 1.0;  //default assumiamo peso = 1.0
        if(pesi_originali.find(*it) != pesi_originali.end()){ //se nella mappa originale ci sono dei pesi diversi lo sovrascrive in peso
            peso = pesi_originali.at(*it); //accede al valore la cui chiave è l'arco corrente
        }

        if(attivo){ //archi incrociati

            unidirected_edge<LiftedNode<T>> e1(LiftedNode<T>(u, false), LiftedNode<T>(v, true)); //(u-, v+)
            unidirected_edge<LiftedNode<T>> e2(LiftedNode<T>(u, true), LiftedNode<T>(v, false)); //(u+, v-)
            G_lifted.add_edge(e1.from(), e1.to()); //aggiungo all'arco liftato
            G_lifted.add_edge(e2.from(), e2.to());
            pesi_lifted[e1] = peso;
            pesi_lifted[e2] = peso;
        }

        else{ //archi parallleli
            unidirected_edge<LiftedNode<T>> e1(LiftedNode<T>(u, false), LiftedNode<T>(v, false)); //(u-, v-)
            unidirected_edge<LiftedNode<T>> e2(LiftedNode<T>(u, true), LiftedNode<T>(v, true)); //(u+, v+)
            G_lifted.add_edge(e1.from(), e1.to());
            G_lifted.add_edge(e2.from(), e2.to());
            pesi_lifted[e1] = peso;
            pesi_lifted[e2] = peso;
        }
    }

    //best_C è il ciclo minimo
    int best_len = -1;
    std::vector<int> best_C(m, 0);  //m= numero di elementi del vettore (corrisponde al numero totale di archi nel grafo originale), inizialmente tutti inizializzati a 0

    for(auto it= nodi_originali.begin(); it!= nodi_originali.end(); ++it){ //cerco il miglior cammino tra v- e v+
        T v = *it;
        LiftedNode<T> sorgente(v, false); //v-
        LiftedNode<T> destinazione(v, true); //v+
    
        std::map<LiftedNode<T>, double> dist;
        std::map<LiftedNode<T>, LiftedNode<T>> pred= Dijkstra(G_lifted, sorgente, pesi_lifted, dist);

        if(dist.find(destinazione) == dist.end()){ //se v+ non è raggiungibile da v-
            continue; //si passa all'iterazione successiva del ciclo for (saltando il resto del corpo)
        }

        double len = dist[destinazione];
        if(best_len != -1 && len >= best_len){
            continue; //se len non è migliore salta il resto, non è il ciclo minimo
        }

        std::vector<LiftedNode<T>> cammino;  //un vettore è ordinato per ordine di inserimento
        LiftedNode<T> corrente = destinazione;
        while(!(corrente.getVertice() == sorgente.getVertice() && corrente.getSegno() == sorgente.getSegno())){//il while si ferma quando il nodo corrente è lo stesso nodo della sorgente e ha lo stesso segno della sorgente (quindi quando raggiunge esattamente la dorgente)
            cammino.push_back(corrente); //aggiungo al cammino il nodo corrente
            corrente = pred[corrente]; //predecessore
        }

        cammino.push_back(sorgente); //aggiungo la sorgente

        //inverto l'ordine
        std::vector<LiftedNode<T>> reversed;
        if(!cammino.empty()){
            for(int i = cammino.size()-1; i >= 0; --i){
                reversed.push_back(cammino[i]);
            }
            cammino = reversed;
        }

        //costruisco il vettore di incidenza C
        std::vector<int> C(m, 0); //m= numero di elementi del vettore (corrisponde al numero totale di archi nel grafo originale), inizialmente tutti inizializzati a 0
        for(int k = 0; k< cammino.size()-1; ++k){
            T u_orig = cammino[k].getVertice();
            T v_orig = cammino[k+1].getVertice();
            if(u_orig == v_orig){
                continue; //se i nodi sono uguali non si ha realmente attraversato un arco, è causa dal lifting
            }
            unidirected_edge<T> e_orig(u_orig, v_orig);
            int indice = G.edge_number(e_orig); //trova l'indice dell'arco nel grafo originale
            if(indice >= 0){
                C[indice] = (C[indice]+1) % 2; //0 diventa 1 e 1 diventa 0, così vengono inseriti solo gli archi percorsi un numero dispari di volte
            }
        }
        
        if(ps_mod2(C, S) == 1){  //controlla che il prodotto scalare tra C ed S sia 1
            best_len = len;
            best_C = C;
        }
    }
    
    return best_C;

}


//algoritmo di De Pina
template<typename T>
std::vector<std::vector<unidirected_edge<T>>> De_Pina(const unidirected_graph<T>& G){
    lifo<T> pila;
    std::map<unidirected_edge<T>, double> pesi;
    T sorgente = *(G.all_nodes().begin());
    unidirected_graph<T> albero = graph_visit(G, sorgente, pila);
    unidirected_graph<T> coalbero = G - albero;

    // m = numero di archi  k = dimensione della base (|E| - |V| + 1)
    std::list<unidirected_edge<T>> archi_tot = G.all_edges();
    int m = 0;
    for(auto it=archi_tot.begin(); it!= archi_tot.end(); ++it){ //conto il numero di archi
        ++m;
    }
    int k = m - G.all_nodes().size() + 1;

    //inizializzo i vettori Si, uno per ogni arco del coalbero
    std::list<unidirected_edge<T>> back_edges = coalbero.all_edges();
    std::vector<std::vector<int>> S_tot;
    for(auto it=back_edges.begin(); it!= back_edges.end(); ++it){
        std::vector<int> S(m, 0); //creo un vettore di lunghezza m, tutto inizializzato a 0
        int indice = G.edge_number(*it); //indice dell'arco nel grafo originario

        if(indice >= 0){  //se il back_edge nel grafo originle viene trovato
            S[indice] = 1;
        }
        S_tot.push_back(S);
    }

    //costruisco la base di cicli
    std::vector<std::vector<int>> vettori_cicli; //vettore di vettori di incidenza dei cicli della base

    for(int i = 0; i < k; ++i){
        std::vector<int> Ci = trova_ciclo_minimo(G, S_tot[i], pesi); //trova vettore di incidenza del ciclo minimo t.c. <Ci, Si> = 1
        vettori_cicli.push_back(Ci);

        //agiorno Sj per j>i (affinché per ogni j>i <Ci, Sj> = 0)
        for(int j = i + 1; j< k; ++j){ //stiamo già iterando su i nel ciclo appena più esterno
            if(ps_mod2(Ci, S_tot[j]) == 1){
                S_tot[j] = diff_simmetrica(S_tot[j], Ci);
            }
        }

    }

    //converto i vettori di incidenza in liste di archi
    std::vector<std::vector<unidirected_edge<T>>> risultato;
    for(int i = 0; i < vettori_cicli.size(); ++i){
        std::vector<unidirected_edge<T>> ciclo;
        int indice = 0;
        for(auto it=archi_tot.begin(); it!= archi_tot.end(); ++it){  //indice e it sono diversi ma viaggiano in parallelo
            if(vettori_cicli[i][indice] == 1){ //se l'arco numero it è presente nell'i-esimo ciclo allora lo aggiungo
                ciclo.push_back(*it);
            }
            ++indice;
        }

        risultato.push_back(ciclo);
    }

    return risultato;

}
