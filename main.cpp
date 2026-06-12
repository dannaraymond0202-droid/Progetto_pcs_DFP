#include <iostream>
#include <fstream>
#include <map>
#include <array>
#include <unordered_set>
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
    string filename = "circuito.txt";    //percorso file omesso poiché il file di input è gia collocato nella cartella build del progetto
    int re = 1;    //contatori per inserire i valori relativi a ogni componente nella giusta posizione delle mappe Res e Vol
    int vo = 1;
    ifstream ifs(filename);
    if (ifs.is_open()){    //selezione di check dell'apertura per evitare errori 
      while(!ifs.eof()){    //fino a quando non arriva alla fine del file fa ....
        string codice_componente;    //R oppure V e il numero sequenziale associato
        array<double,3> valori;
        ifs >> codice_componente;
        unordered_set<char> caratteri_codice_componente;
        for(int i = 0; i<codice_componente.size(); i++){
          caratteri_codice_componente.insert(codice_componente[i]);    //conversione necessaria a controllare se il carattere R oppure V sono presenti
        }
        if(caratteri_codice_componente.contains('R')){ //inizializzazione delle due mappe Res e Vol
          caratteri_codice_componente.clear();
          ifs >> valori[0];    //valore in ohm
          ifs >> valori[1];    //nodo di partenza
          ifs >> valori[2];    //nodo di arrivo
          Res[re] = valori;
          re++;
        }
        else if(caratteri_codice_componente.contains('V')){
          caratteri_codice_componente.clear();
          ifs >> valori[0];    //valore in volt
          ifs >> valori[1];    //nodo di partenza
          ifs >> valori[2];    //nodo di arrivo
          Vol[vo] = valori;
          vo++;
        }
      }
    }
    
    
    
    auto G = circuit_graph_generator(Res, Vol);
    
    char choice;
    do{
      cout << "digitare 1 se si desidera trovare i cicli fondamentali (maglie) attraverso il metodo basato sull'algoritmo DFS oppure 2 per il metodo di De Pina:\n";
      cin >> choice;
    }while(choice != '1' && choice != '2');
    
    
    vector<vector<unidirected_edge<int>>> maglie;
    if(choice == '1') maglie = fundamental_cycles(G);
    else maglie = De_Pina(G);

    int m = Res.size(); //numero di resistenze
    int n = maglie.size(); //numero di cicli trovati
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m); //creazione della matrice diagonale R che ha per elementi sulla diagonale le resistenze
    Eigen::VectorXd v = Eigen::VectorXd::Zero(n); //creazione del vettore dei termini noti
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n); //matrice di incidenza (resistori maglie)
    matrici(R, v, B, maglie, Res, Vol);
    
    Eigen::VectorXd Ir = Eigen::VectorXd::Zero(Res.size());
    const double tol =  1.0e-15;
    Eigen::MatrixXd Risultato = Calcolo(B,R,v,tol); //matrice in cui nella prima colonna ci sono le Vr e nella seconda le Ir (numero di righe = numero di resistenze)

    for(auto& [num, info] : Res){
        cout << "R" << num << ": V = " << Risultato(num-1,0) << " volts, I = " << Risultato(num-1,1) << " amps.\n";
    }
    
    return 0;
}
