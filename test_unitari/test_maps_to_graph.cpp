#include <map>
#include <array>
#include <cstdlib> //per EXIT_SUCCESS EXIT_FAILURE
#include"../maps_to_graph.hpp"


using namespace std;

int main() {
    
    map<int, array<double, 3>> Res;
    Res[0] = {100.0, 2, 3};
    Res[1] = {220.0, 3, 4};
    Res[2] = {330.0, 4, 1};
    Res[3] = {470.0, 5, 1};

    map<int, array<double, 3>> Vol;
    Vol[0] = {10.0, 1, 2};
    Vol[1] = {5.0, 3, 5};

    unidirected_graph<int> G = circuit_graph_generator(Res, Vol);

    unidirected_edge<int> e1(2,3);
    if(G.edge_number(e1) == -1){ return EXIT_FAILURE;}
    int i1 = G.edge_number(e1);
    unidirected_edge<int> e_g1 = G.edge_at(i1);
    if(e1.is_inv() != e_g1.is_inv()){ return EXIT_FAILURE;}

    unidirected_edge<int> e2(3,4);
    if(G.edge_number(e2) == -1){ return EXIT_FAILURE;}
    int i2 = G.edge_number(e2);
    unidirected_edge<int> e_g2 = G.edge_at(i2);
    if(e2.is_inv() != e_g2.is_inv()){ return EXIT_FAILURE;}

    unidirected_edge<int> e3(4,1);
    if(G.edge_number(e3) == -1){ return EXIT_FAILURE;}
    int i3 = G.edge_number(e3);
    unidirected_edge<int> e_g3 = G.edge_at(i3);
    if(e3.is_inv() != e_g3.is_inv()){ return EXIT_FAILURE;}

    unidirected_edge<int> e4(5,1);
    if(G.edge_number(e4) == -1){ return EXIT_FAILURE;}
    int i4 = G.edge_number(e4);
    unidirected_edge<int> e_g4 = G.edge_at(i4);
    if(e4.is_inv() != e_g4.is_inv()){ return EXIT_FAILURE;}

    unidirected_edge<int> e5(1,2);
    if(G.edge_number(e5) == -1){ return EXIT_FAILURE;}
    int i5 = G.edge_number(e5);
    unidirected_edge<int> e_g5 = G.edge_at(i5);
    if(e5.is_inv() != e_g5.is_inv()){ return EXIT_FAILURE;}

    unidirected_edge<int> e6(3,5);
    if(G.edge_number(e6) == -1){ return EXIT_FAILURE;}
    int i6 = G.edge_number(e6);
    unidirected_edge<int> e_g6 = G.edge_at(i6);
    if(e6.is_inv() != e_g6.is_inv()){ return EXIT_FAILURE;}
    

    return EXIT_SUCCESS;

}
