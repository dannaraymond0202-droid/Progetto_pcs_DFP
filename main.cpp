#include <iostream>
#include <fstream>
#include <map>
#include <array>
#include <set>
#include <vector>
#include <Eigen/Dense>
#include "cicli_dfs.hpp"
#include "de_pina.hpp"
#include "maps_to_graph.hpp"
#include "matrici.hpp"
#include "unidirected_graph.hpp"
#include "unidirected_edges.hpp"
#include "Calcolo.hpp"


using namespace std;


int main(){
    //Lettura File
    map<int, array<double, 3>> Res, Vol;
    string filename = "circuito.txt";
    int r = 1;
    int v = 1;
    ifstream ifs(filename);
    if (ifs.is_open()){//comando dii check dell'apertura per evitare errori 
      while(!ifs.eof()){//fino a quando non arriva alla fine fa ....
        string pip;
        array<double,3> valori;
        ifs >> pip;
        set<char> poi;
        for(int i = 0; i<pip.size(); i++){
          poi.insert(pip[i]);
        }
        if(poi.contains('R')){ //creazione delle due mappa Res e Vol
          poi.clear();
          ifs >> valori[0];
          ifs >> valori[1];
          ifs >> valori[2];
          Res[r] = valori;
          r++;
        }
        else if(poi.contains('V')){
          poi.clear();
          ifs >> valori[0];
          ifs >> valori[1]; 
          ifs >> valori[2];
          Vol[v] = valori;
          v++;
        }
      }
    }
    
    
    
    auto G = circuit_graph_generator(Res, Vol);
    
    char choice;
    do{
      cout << "digitare 1 se si desidera trovare i cicli fondamentali (maglie) attraverso il metodo basato sull'algoritmo DFS oppure 2 per il metodo di De Pina:\n";
      cin >> choice;
    }while(choice != "1" && choice != "2");
    
    
    vector<vector<unidirected_edge<int>>> maglie;
    if(choice == "1") maglie = fundamental_cycles(G);
    else maglie = De_Pina(G);

    int m = Res.size(); //numero di resistenze
    int n = maglie.size(); //numero di cicli trovati
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m); //creazione della matrice diagonale R che ha per elementi sulla diagonale le resistenze
    Eigen::VectorXd v = Eigen::VectorXd::Zero(n); //creazione del vettore dei termini noti
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n); //matrice di incidenza (resistori maglie)
    matrici(R, v, B, maglie, Res, Vol);
    
    Eigen::VectorXd i0 = Eigen::Vector::Zero(//numero dei cicli);
    const double tol =  1.0e-15;
    Eigen::VectorXd Vr = Calcolo(B,R,v,i0,tol);

    for(auto& [n, info] : Res){
        cout << "R" << n << ": V = " << Vr[n] << " volts, I = " << i0[n] << " amps.\n";
    }
    
    return 0;
}
