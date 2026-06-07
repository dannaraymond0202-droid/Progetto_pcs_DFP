#include <iostream>
#include <ostream>
#include <fstream>
#include <map>
#include <array>
#include <set>
#include <vector>
#include "cicli_dfs.hpp"
#include "de_pina.hpp"

using namespace std;

//standard per il verso di percorrenza delle correnti: senso ORARIO

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
      cout<<"Ho letto correttamente R";
      poi.clear();
      ifs >> valori[0];
      ifs >> valori[1];
      ifs >> valori[2];
      Res[r] = valori;
      r ++;
    }
    else if(poi.contains('V')){
      cout<<"Ho letto correttamnete V"; 
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


vector<vector<unidirected_edge<int>>> cycles;
if(choice == "1") cycles = fundamental_cycles(G);
else cycles = De_Pina(G);
