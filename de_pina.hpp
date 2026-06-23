#pragma once //per evitare errori di ridefinizione 
#include<vector>
#include<map>
#include<set>
#include<queue>
#include"lifo.hpp"
#include"unidirected_edges.hpp"
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

//differenza simmetrica tra vettori binari (XOR)
std::vector<int> diff_simmetrica(const std::vector<int>& a, const std::vector<int>& b){
    std::vector<int> diff(a.size()); //inizializzo il vettore di dimensione uguale ad a
    for(int i=0; i < a.size(); ++i){
        if(a[i] != b[i]){
            diff[i] = 1;
        }
        else{
            diff[i] = 0;
        }
    }
    return diff;
}

//creo una classe di lifted nodes, che è wrapper di T con l'aggiunta dell'attributo "segno"
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

    LiftedNode(const T& v, bool s){ //costruttore user_defined
        vertice = v;
        segno = s;
    }

    T getVertice() const {return vertice;} //getter
    bool getSegno() const {return segno;} //getter

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
void trova_ciclo_minimo(const unidirected_graph<T>& G, const std::vector<int>& S, const std::map<unidirected_edge<T>, double>& pesi_originali, std::vector<int>& C_out, std::vector<unidirected_edge<T>>& ciclo_archi_out){
//ciclo archi out è un vettore di archi che immagazina l'informazione del verso di percorrenza (attraverso le flag degli archi is inverted)

    std::list<unidirected_edge<T>> archi_tot = G.all_edges(); //lista di tutti gli archi (all_edges restituisce una lista, scelto perché mantiene l'ordine di inserimento)
    int m = archi_tot.size();

    std::set<T> nodi_originali = G.all_nodes(); //numero i vertici da 0 a n-1  (all_nodes restituisce un set scelto perché garantisce l'unicità)

    //costruzione del grafo lifted
    unidirected_graph<LiftedNode<T>> G_lifted;    //vedi riga 35 per la classe LiftedNode
   
    std::map<unidirected_edge<LiftedNode<T>>, double> pesi_lifted;

    //aggiungo archi nel lifting
    for(auto it= archi_tot.begin(); it!= archi_tot.end(); ++it){
        T u = (*it).from();
        T v = (*it).to();

        int indice = G.edge_number(*it); //prendo l'indice dell'arco nel grafo originale (per ogni arco)
        bool attivo = (S[indice] == 1); //S è il vettore di adiacenza, se S all'indice corrente è 1 allora attivo = true (se S[indice] == 0 allora attivo = false)

        if(attivo){ //creazione archi "incrociati" nel grafo liftato

            unidirected_edge<LiftedNode<T>> e1(LiftedNode<T>(u, false), LiftedNode<T>(v, true)); //(u-, v+)
            unidirected_edge<LiftedNode<T>> e2(LiftedNode<T>(u, true), LiftedNode<T>(v, false)); //(u+, v-)
            G_lifted.add_edge(e1.from(), e1.to()); //aggiungo all'arco liftato
            G_lifted.add_edge(e2.from(), e2.to());
            pesi_lifted[e1] = pesi_originali.at(*it);
            pesi_lifted[e2] = pesi_originali.at(*it);
        }

        else{ //creazione archi "paralleli" nel grafo liftato
            unidirected_edge<LiftedNode<T>> e1(LiftedNode<T>(u, false), LiftedNode<T>(v, false)); //(u-, v-)
            unidirected_edge<LiftedNode<T>> e2(LiftedNode<T>(u, true), LiftedNode<T>(v, true)); //(u+, v+)
            G_lifted.add_edge(e1.from(), e1.to());
            G_lifted.add_edge(e2.from(), e2.to());
            pesi_lifted[e1] = pesi_originali.at(*it);
            pesi_lifted[e2] = pesi_originali.at(*it);
        }
    }

    //best_C è il ciclo minimo
    int best_len = -1;
    std::vector<int> best_C(m, 0);  //m = numero di elementi del vettore (corrisponde al numero totale di archi nel grafo originale), inizialmente tutti inizializzati a 0)
    std::vector<unidirected_edge<T>> best_archi;    //corrisponde a ciclo_archi_out ma per il ciclo minimo "best_C", per tenere conto del verso di percorrenza

    for(auto it= nodi_originali.begin(); it!= nodi_originali.end(); ++it){ //cerco il miglior cammino tra v- e v+
        T v = *it;
        LiftedNode<T> sorgente(v, false); //=v-
        LiftedNode<T> destinazione(v, true); //=v+
    
        std::map<LiftedNode<T>, double> dist;
        std::map<LiftedNode<T>, LiftedNode<T>> pred = Dijkstra(G_lifted, sorgente, pesi_lifted, dist);    //crea la mappa dei predecessori per i percorsi minimi a partire dal nodo source (SSSP: single source shortest path)

        if(dist.find(destinazione) == dist.end()){ //ossia se v+ non è raggiungibile da v-
            continue; //si passa all'iterazione successiva del ciclo for (saltando il resto del corpo)
        }

        double len = dist[destinazione];
        if(best_len != -1 && len >= best_len){    //controllo best_len diverso da -1 per assicurarci di non essere alla prima iterazione del ciclo for sui nodi
            continue; //se len non è migliore salta il resto, non è il ciclo minimo
        }

        std::vector<LiftedNode<T>> cammino;  //un vettore è ordinato per ordine di inserimento, sarà [destinazione, ..., sorgente]
        LiftedNode<T> attuale = destinazione;
        while(!(attuale.getVertice() == sorgente.getVertice() && attuale.getSegno() == sorgente.getSegno())){//il while si ferma quando il nodo attuale è lo stesso nodo della sorgente e ha lo stesso segno della sorgente (quindi quando raggiunge esattamente la sorgente, che è l'unico nodo ad avere la proprietà di essere associato a se stesso nella mappa "pred")
            cammino.push_back(attuale); //aggiungo al cammino il nodo attuale
            attuale = pred[attuale]; //prendiamo il predecessore del nodo attuale per risalire di 1 passo la mappa dei predecessori
        }
        cammino.push_back(sorgente); //aggiungo la sorgente al fondo del cammino


        //costruisco il vettore di incidenza C e vettore archi orientati
        std::vector<int> C(m, 0); //m= numero di elementi del vettore (corrisponde al numero totale di archi nel grafo originale), inizialmente tutti inizializzati a 0
        std::vector<unidirected_edge<T>> archi_ciclo;    //sempre il vettore ausiliario per tenere traccia del verso di percorrenza delle maglie
        for(int k = cammino.size()-1; k > 0; --k){    //iteriamo al contrario poiché come si può vedere da riga 143 il cammino è nenorizzato dalla fine all'inizio
            T u_orig = cammino[k].getVertice();
            T v_orig = cammino[k-1].getVertice();
            if(u_orig == v_orig){
                continue; //se i nodi sono uguali non si ha realmente attraversato un arco, è a causa dal lifting
            }
            unidirected_edge<T> e_orig(u_orig, v_orig);    //dichiarandolo così manteniamo il verso di percorrenza
            int indice = G.edge_number(e_orig); //trova l'indice dell'arco nel grafo originale
            if(indice >= 0){    //se e_orig è stato trovato nel grafo
                C[indice] = (C[indice]+1) % 2; //0 diventa 1 e 1 diventa 0, così vengono inseriti solo gli archi percorsi un numero dispari di volte
            }
            archi_ciclo.push_back(e_orig); //ordine: sorgente, ..., destinazione e inoltre è stata preservata l'informazione sul verso di percorrenza
        }
        
        if(ps_mod2(C, S) == 1){  //controlla che il prodotto scalare tra C ed S sia 1 significa che abbiamo trovato un nuovo ciclo minimo!
            best_len = len;
            best_C.swap(C); // best_C = C ma evita copie
            best_archi.swap(archi_ciclo);
        }
    }
    
    C_out.swap(best_C); //ciclo migliore
    ciclo_archi_out.swap(best_archi);
}


//algoritmo di De Pina
template<typename T>
std::vector<std::vector<unidirected_edge<T>>> De_Pina(const unidirected_graph<T>& G){
    lifo<T> pila;
    std::map<unidirected_edge<T>, double> pesi; //dichiarazione
    for(auto& e : G.all_edges()) pesi[e] = 1.0;    //inizializzazione di tutti i pesi a 1 per default
    T sorgente = *(G.all_nodes().begin());
    unidirected_graph<T> albero = graph_visit(G, sorgente, pila);
    unidirected_graph<T> coalbero = G - albero;

    // m = numero di archi  k = dimensione della base (|E| - |V| + 1)
    std::list<unidirected_edge<T>> archi_tot = G.all_edges();
    int m = G.all_edges().size();
    int k = m - G.all_nodes().size() + 1;    //dimensione della base di cicli minimi ossia il "numero ciclomatico"

    //dichiaro e inizializzo i vettori di adiacenza booleani S_i, uno per ogni arco del coalbero
    std::list<unidirected_edge<T>> back_edges = coalbero.all_edges();
    std::vector<std::vector<int>> S_tot;
    for(auto it=back_edges.begin(); it!= back_edges.end(); ++it){    //iteriamo con l'iteratore it
        std::vector<int> S_i(m, 0); //creo il vettore S_i di lunghezza m, tutto inizializzato a 0
        int indice = G.edge_number(*it); //indice dell'arco nel grafo originario

        if(indice >= 0){  //se il back_edge nel grafo originale viene trovato
            S_i[indice] = 1;    //allora S_i avrà un elemento pari a 1 nella posizione ocrrispondente all'indice del back edge nel grafo originario
        }
        S_tot.push_back(S_i);    //aggiungiamo al vettore di vettori il nostro S_i
    }

    //costruisco la base di cicli
    std::vector<std::vector<unidirected_edge<T>>> risultato; //vettore di vettori di archi (tutti i cicli orientati)

    for(int i = 0; i < k; ++i){
        std::vector<int> Ci;    //vettore di incidenza
        std::vector<unidirected_edge<T>> ciclo_orientato;    //quello che verrà inserito nel vettore di vettori "risultato"
        trova_ciclo_minimo(G, S_tot[i], pesi, Ci, ciclo_orientato); //modifico tramite riferimento Ci e ciclo_orientato

        risultato.emplace_back(); //aggiungo un vettore vuoto al fondo del vettore di vettori
        risultato.back().swap(ciclo_orientato); //scambia l'ultimo elemento(vuoto) con il ciclo orientato, senza fare copie temporanee inutili grazie a emplace_back + swap

        //aggiorno Sj per j>i (affinché per ogni j>i <Ci, Sj> = 0)
        for(int j = i + 1; j < k; ++j){ //stiamo già iterando su i nel ciclo appena più esterno
            if(ps_mod2(Ci, S_tot[j]) == 1){
                S_tot[j] = diff_simmetrica(S_tot[j], Ci);
            }
        }

    }

    return risultato; //non restituiamo più un vettore di vettori di incidenza ma un vettore di vettori di archi orientati (serve per le maglie)

}
