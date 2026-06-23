#include<Eigen/Dense> //per matrici e vettori
#include"unidirected_edges.hpp" 
#include <map>
#include <array>
#include <vector>

void matrici(Eigen::MatrixXd& R, Eigen::VectorXd& v, Eigen::MatrixXd& B, const vector<vector<unidirected_edge<int>>>& maglie, const std::map<int, array<double, 3>>& Res, const std::map<int, array<double, 3>>& Vol){
    //è una funzione void e inizializza le variabili modificando tutto tramite riferimento
    for(auto& [key, obj]: Res){
        R(key-1, key-1) = obj[0];    //riempie la diagonale della matrice R con i valori in ohm dei vari resistori
    }

    int i = 0;
    for(auto& vet : maglie){ //vet è un vettore di archi
        for(auto& [key, obj]: Vol){
            unidirected_edge<int> a(obj[1], obj[2]);    //creiamo l'arco corrispondente ad ogni generatore di tensione in modo tale che sia fedele all'informazione originale del verso
            for(auto& e : vet){ //ciclo su ogni arco del vettore corrispondente alla singola maglia
                if(e == a && e.is_inv() == a.is_inv()){ //se hanno entrambi verso di percorrenza invertito o nessuno dei due lo ha significa che questa maglia sta attraversando questo generatore dal morsetto positivo
                    v(i) = v(i) - obj[0];
                }
                if(e == a && e.is_inv() != a.is_inv()){ //altrimenti significa che questa maglia sta attraversando questo generatore dal morsetto negativo
                    v(i) = v(i) + obj[0];
                }
            }
        }
        i += 1;
    }
    
    int j = 0;    //indice che tiene traccia delle maglie che consideriamo una per volta
    //creiamo ora la matrice di incidenza B
    for(auto& vet : maglie){ //vet è un vettore di archi
        for(auto& [key, obj]: Res){
            unidirected_edge<int> b(obj[1], obj[2]);    //ora trattandosi di resistori il verso per convenzione non dipende più dall'ordine dei nodi nella netlist ma dal nodo minore al maggiore
            for(auto& e : vet){ //ciclo su ogni arco del vettore corrispondente alla singola maglia
                if(e == b && !(e.is_inv())){ //significa che stiamo attraversando la maglia nello stesso verso del resistore
                    B(key-1, j) = 1;
                }
                if(e == b && e.is_inv()){ //significa che stiamo attraversando la maglia nel senso opposto rispetto al resistore
                    B(key-1, j) = -1;
                } 
                //se la resistenza "key" non appartiene alla maglia j lasciamo 0 nella posizione corrispondente (jey-1, j) della matrice B
            }
        }
        j += 1;
    }
}
