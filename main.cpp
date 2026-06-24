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
    char choice;
    cout << "\n Si digiti 0 se si ha una netlist.txt nella cartella build \no 1 se la si vuole creare:\n";
    while(!(cin >> choice) || (choice != '0' && choice != '1')){
        cout<< "Per favore inserire 0 o 1: \n";
        cin.clear();
        string sbagliato;
        getline(cin, sbagliato);   
    }

    if (choice == '1'){ //Caso Creazione dal terminale 
        cout << "Digitare R per inserire una resistenza o V per un generatore di tensione nella netlist:\n";
        do{
            while(!(cin >> input)){
                cin.clear();
                string sbagliato;
                getline(cin, sbagliato); 
            }
            array<double, 3> valori;
            if(input == 'R'){
                re ++; //itero per poter indicizzare correttamente ogni componente in Res
                cout << "Inserire il valore della resistenza in Ohm: \n";
                while(!(cin >> valori[0])){ // https://en.cppreference.com/cpp/io/basic_ios/clear
                    cout<< "Per favore inserire un valore corretto:\n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato); //insieme a clear, getline è essenziale per gestire valori inattesi ripristinando cin senza portare ad un ciclo infinito. 
                    }
                cout<<"Inserire primo nodo: \n"; 
                while(!(cin >> valori[1])){ 
                    cout<< "Per favore inserire un valore corretto:\n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato);   
                }
                cout<<"Inserire secondo nodo: \n";
                while(!(cin >> valori[2])){
                    cout<< "Per favore inserire un valore corretto:\n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato);   
                }         
                Res[re] = valori;
            }
            else if(input == 'V'){  
                vo ++; //itero per poter indicizzare correttamente ogni componente in Vol
                cout << "Inserire il valore del generatore di tensione in Volt: \n";
                while(!(cin >> valori[0])){
                     cout<< "Per favore inserire un valore corretto:\n";
                     cin.clear();
                     string sbagliato;
                     getline(cin, sbagliato); //essenziale per gestire valori inattesi cancellando cin  
                }
                cout<<"Inserire primo nodo: \n";
                while(!(cin >> valori[1])){ 
                    cout<< "Per favore inserire un valore corretto:\n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato);   
                }
                cout<<"Inserire Secondo nodo:\n";
                while(!(cin >> valori[2])){
                    cout<< "Per favore inserire un valore corretto:\n";
                    cin.clear();
                    string sbagliato;
                    getline(cin, sbagliato);   
                }         
                Vol[vo] = valori; 
            }        
            
            if (input != 'B') cout<<"Inserisci B per terminare l'inserimento oppure R o V per continuare:\n";
            
        }while(input != 'B');
   
    }
    else if(choice == '0'){ //Caso Lettura File
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
    //Controliamo se il Grafo ha almeno un ciclo, assumendo che sia sempre connesso, grazie al numero ciclomatico k.
    int k = G.all_edges().size() - G.all_nodes().size() +1;
    if (k == 0) {
        cerr<< "Il grafo generato non è ciclico quindi la corrente è sempre 0 in tutto il circuito\n";
        return 1;
    } 


    
    do{
      cout << "digitare 0 se si desidera trovare i cicli fondamentali (maglie) attraverso il metodo basato sull'algoritmo DFS oppure 1 per il metodo di De Pina:"<<" \n";
      cin >> choice;
    }while(choice != '0' && choice != '1');// il do while esegue almeno una volta il corpo e poi continua fino a che il carattere non è tra quelli previsti
    
    
    vector<vector<unidirected_edge<int>>> maglie;
    if(choice == '0') maglie = fundamental_cycles(G);
    else maglie = De_Pina(G);

    int m = Res.size(); //numero di resistenze
    int n = maglie.size(); //numero di cicli trovati
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m); //creazione della matrice diagonale R che ha per elementi sulla diagonale le resistenze
    Eigen::VectorXd v = Eigen::VectorXd::Zero(n); //creazione del vettore dei termini noti
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n); //matrice di incidenza (con dimensione = #resistori x #maglie)
    matrici(R, v, B, maglie, Res, Vol);
    
    const double tol =  1.0e-15;
    
    Eigen::MatrixXd Risultato = Calcolo(B,R,v,tol); //matrice tale che nella prima colonna ci sono le Vr e nella seconda le Ir (numero di righe = numero di resistenze)
    cout << "Risultato:\n";
    for(auto& [num, info] : Res){
        cout << "R" << num << ": V = " << Risultato(num-1,0) << " volts, I = " << Risultato(num-1,1) << " amps.\n";
    }

    cout << "\nNota: l'ordine dell'output è lo stesso dell'ordine di inserimento \n\n";
    return 0;
}
