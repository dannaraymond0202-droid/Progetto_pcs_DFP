#pragma once
#include "unidirected_edges.hpp"
#include "unidirected_graph.hpp"

using namespace std;

unidirected_graph<int> circuit_graph_generator(map<int, array<double, 3>>& Res, map<int, array<double, 3>>& Vol){
  unidirected_graph<int> G;
  for(auto& [n,info] : Res){
    G.add_edge(info.at(1), info.at(2));
  }
  for(auto& [n,info] : Vol){
    G.add_edge(info.at(1), info.at(2));
  }

  return G;
}
