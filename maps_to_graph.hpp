#pragma once
#include "unidirected_edges.hpp"
#include "unidirected_graph.hpp"

using namespace std;

unidirected_graph<int> circuit_graph_generator(map<int, array<double, 3>>& Res, map<int, array<double, 3>>& Vol){
  unidirected_graph<int> G;  //dichiarazione del grafo 
  for(auto& [n,info] : Res){
    G.add_edge(info.at(1), info.at(2)); //inizializzazione chiamando add_edge()
  }
  for(auto& [n,info] : Vol){
    G.add_edge(info.at(1), info.at(2));
  }
         
  return G;  /*in unidirect_edges.hpp abbiamo aggiunto un membro privato booleano per memorizzare se la direzione del componente
              è stata invertita o meno nella dichiarazione dell'arco, informazione poi necessaria in De Pina e per la matrice d'incidenza*/
}
