#include <iostream>
using namespace std;
//standard per il verso di percorrenza delle correnti: senso ORARIO

map<int, array<double, 3>> Res, Vol;  //raymond

auto G = circuit_graph_generator(Res, Vol);

char choice;
do{
  cout << "digitare 1 se si desidera trovare i cicli fondamentali (maglie) attraverso il metodo basato sull'algoritmo DFS oppure 2 per il metodo di De Pina:\n";
  cin >> choice;
}while(choice != "1" && choice != "2");

if(choice == "1")...
else ...
