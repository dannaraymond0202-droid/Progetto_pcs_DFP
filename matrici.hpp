#include<Eigen/Dense> //per matrici e vettori
#include"unidirected_edge.hpp"
#include"unidirected_graph.hpp"


template<typename T>
int matrici(){
    int m = Res.size(); //numero di resistenze
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m); //creazione della matrice diagonale R che ha per elementi sulla diagonale le resistenze
    for(auto& [key, obj]: Res){
        R(key-1, key-1) = obj[0];
    }

    int n = maglie.size(); //numero di cicli trovati
    Eigen::VectorXd v = Eigen::VectorXd::Zero(n); //creazione del vettore dei termini noti
    int i = 0;
    for(auto& vet: maglie){ //vet è un vettore di archi
        for(auto& [key, obj]: Vol){
            unidirected_edge<T> a(obj[1], obj[2]);
            for(auto& e : vet){ //ciclo su ogni arco del vettore della singola maglia
                if(e == a && is_inv(e) == is_inv(a)){ //significa che stiamo attraversando la maglia dal morsetto positivo
                    v(i) = v(i) - obj[0];
                }
                if(e == a && !(is_inv(e) == is_inv(a))){ //significa che stiamo attraversando la maglia dal morsetto negativo
                    v(i) = v(i) + obj[0];
                }
            }
        }
        i += 1;
    }
    
    
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n); //matrice di incidenza
    int i = 0;
    for(auto& vet: maglie){ //vet è un vettore di archi
        for(auto& [key, obj]: Res){
            unidirected_edge<T> a(obj[1], obj[2]);
            for(auto& e : vet){ //ciclo su ogni arco del vettore della singola maglia
                if(e == a && !(is_inv(e))){ //significa che stiamo attraversando la maglia nello stesso vero del resistore
                    B(i, key-1) = 1;
                }
                if(e == a && is_inv(e)){ //significa che stiamo attraversando la maglia nel senso opposto rispetto al resistore
                    B(i, key-1) = -1;
                }
                else{
                    B(i, key-1) = 0; //significa che il resistore non appartiene alla maglia
                }
            }
        }
        i += 1;
    }

    return 0;
}
