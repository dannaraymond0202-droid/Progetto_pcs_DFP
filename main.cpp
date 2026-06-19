#include <iostream>
#include <fstream>
#include <map>
#include <array>
#include <vector>
#include <string>
#include <unordered_set>
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
    int re = 0;   //contatori per inserire i valori relativi a ogni componente nella giusta posizione delle mappe Res e Vol
    int vo = 0;
    char input;
    int bin;
    array<double, 3> valori;
    cout << "\n Si digiti 0 si ha una netlist.txt nella cartella build \n o 1 se la si vuole creare: \n";
    while(!(cin >> bin) || (bin != 0 && bin != 1)){
        cout<< "Per favore inserire 0 o 1: \n";
        cin.clear();
        string sbagliato;
        getline(cin, sbagliato);   
    }

    if (bin == 1){ //Caso Creazione dal terminale 
        cout << "Digitare R o V per inserire una resistenza o un generatore di potenziale nella netlist \n";
        do{
            while(!(cin >> input)){
                cin.clear();
                string sbagliato;
                getline(cin, sbagliato); 
            }
            
            if(input == 'B') break;

            else if(input == 'R'){
                re ++; //itero per poter identificare ogni componente
                cout << "Inserire il valore della resistenza in Ohm: \n";
                while(!(cin >> valori[0])){ //https://en.cppreference.com/cpp/io/basic_ios/clear
                    cout<< "Per favore inserire un valore corretto: \n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato); //insieme ad clear getline è essenziale per gestire valori inattesi ripristinando cin  senza dare un ciclo infinito. 
                    }
                cout<<"Inserire primo nodo: \n"; 
                while(!(cin >> valori[1])){ 
                    cout<< "Per favore inserire un valore corretto: \n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato);   
                }
                cout<<"Inserire secondo nodo: \n";
                while(!(cin >> valori[2])){
                    cout<< "Per favore inserire un valore corretto: \n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato);   
                }         
                Res[re] = valori;
            }
            else if(input == 'V'){  
                vo ++; //itero per poter identificare ogni componente
                cout << "Inserire il valore del generatore di potenziale in Volt: \n";
                while(!(cin >> valori[0])){
                     cout<< "Per favore inserire un valore corretto: \n";
                     cin.clear();
                     string sbagliato;
                     getline(cin, sbagliato); //essenziale per gestire valori inattesi cancellando cin  
                }
                cout<<"Inserire primo nodo: \n";
                while(!(cin >> valori[1])){ 
                    cout<< "Per favore inserire un valore corretto: \n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato);   
                }
                cout<<"Inserire Secondo nodo: \n";
                while(!(cin >> valori[2])){
                    cout<< "Per favore inserire un valore corretto: \n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato);   
                }         
                Vol[vo] = valori; 
            }        
            
            cout<<"Inserisci B per terminale l'inserimento o R e V per continuare \n";
            
        }while(true);
   
    }
    else if(bin == 0){ //Caso Lettura File
        string filename = "Netlist.txt";    //percorso file omesso poiché il file di input è gia collocato nella cartella build del progetto  
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
                    re++;
                    ifs >> valori[0];    //valore in ohm
                    ifs >> valori[1];    //nodo di partenza
                    ifs >> valori[2];    //nodo di arrivo
                    Res[re] = valori;
          
                }
                else if(caratteri_codice_componente.contains('V')){
                    caratteri_codice_componente.clear();
                    vo++;
                    ifs >> valori[0];    //valore in volt
                    ifs >> valori[1];    //nodo di partenza
                    ifs >> valori[2];    //nodo di arrivo
                    Vol[vo] = valori;

                }    
            }
        }
    }
    

    //Fine Input.
    auto G = circuit_graph_generator(Res, Vol);
    cout <<" Grafo generato";
    char choice;
    do{
      cout << "digitare 1 se si desidera trovare i cicli fondamentali (maglie) attraverso il metodo basato sull'algoritmo DFS oppure 2 per il metodo di De Pina:"<<" \n";
      cin >> choice;
    }while(choice != '1' && choice != '2');// il do while esegue almeno una volta il corpo e poi continua fino a che il carattere non è tra quelli previsti
    
    
    vector<vector<unidirected_edge<int>>> maglie;
    if(choice == '1') maglie = fundamental_cycles(G);
    else maglie = De_Pina(G);

    int m = Res.size(); //numero di resistenze
    int n = maglie.size(); //numero di cicli trovati
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m); //creazione della matrice diagonale R che ha per elementi sulla diagonale le resistenze
    Eigen::VectorXd v = Eigen::VectorXd::Zero(n); //creazione del vettore dei termini noti
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n); //matrice di incidenza (con dimensione = #resistori x #maglie)
    matrici(R, v, B, maglie, Res, Vol);
    
    Eigen::VectorXd Ir = Eigen::VectorXd::Zero(Res.size());
    const double tol =  1.0e-15;
    Eigen::MatrixXd Risultato = Calcolo(B,R,v,tol); //matrice in cui nella prima colonna ci sono le Vr e nella seconda le Ir (numero di righe = numero di resistenze)
    cout << "Risultato:\n";
    for(auto& [num, info] : Res){
        cout << "R" << num << ": V = " << Risultato(num-1,0) << " volts, I = " << Risultato(num-1,1) << " amps.\n";
    }

    cout << "Nota: l'ordine dell'output è lo stesso dell'ordine di inserimento \n";
    return 0;
}
