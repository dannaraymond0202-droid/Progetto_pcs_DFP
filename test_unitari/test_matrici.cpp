

#include <map>
#include <array>
#include <Eigen/Dense>
#include <iostream>
#include "../matrici.hpp"
#include "../unidirected_edges.hpp"


int main(){
    std::map<int, std::array<double,3>> mappa_R; // inizializzo e aggiungo gli elementi in map 
    mappa_R[1] = {20.2, 1 ,2};
    mappa_R[2] = {15.7, 2, 4};
    mappa_R[3] = {7.1, 4, 5};
    mappa_R[4] = {9.5, 3, 1};
    mappa_R[5] = {13.5, 3, 5};


    std::map<int, std::array<double,3>> mappa_V;
    mappa_V[1] =  {35.7, 2, 3};
    mappa_V[2] =  {23.2, 5, 1};
    mappa_V[3] =  {30.6, 3, 4};
    


    vector<vector<unidirected_edge<int>>> maglie; // inizializzo e aggiungo le maglie
    maglie = {{unidirected_edge<int> (1,2), unidirected_edge<int> (2,3), unidirected_edge<int> (3,1)},
              {unidirected_edge<int> (4,5), unidirected_edge<int> (5,3), unidirected_edge<int> (3,4)},
              {unidirected_edge<int> (2,4), unidirected_edge<int> (4,3), unidirected_edge<int> (3,2)},
              {unidirected_edge<int> (1,3),unidirected_edge<int> (3,5), unidirected_edge<int> (5,1)}};


    int m = mappa_R.size(); 
    int n = maglie.size(); 
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m);
    Eigen::VectorXd v = Eigen::VectorXd::Zero(n); 
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n);
    matrici(R, v, B, maglie, mappa_R, mappa_V);


    Eigen::MatrixXd R_esa = Eigen::MatrixXd::Zero(5, 5); // R_esatto
    R_esa(0,0) = 20.2;
    R_esa(1,1) = 15.7;
    R_esa(2,2) = 7.1;
    R_esa(3,3) = 9.5;
    R_esa(4,4) = 13.5;
    if(R != R_esa) {
        std::cout<<"\nR è sbagliato.\n";
        return EXIT_FAILURE;
    }
    Eigen::MatrixXd B_esa = Eigen::MatrixXd::Zero(5, 4); 
    B_esa(0,0) = 1;
    B_esa(0,1) = 0;
    B_esa(0,2) = 0;
    B_esa(0,3) = 0;


    B_esa(1,0) = 0;
    B_esa(1,1) = 0;
    B_esa(1,2) = 1;
    B_esa(1,3) = 0;


    B_esa(2,0) = 0;
    B_esa(2,1) = 1;
    B_esa(2,2) = 0;
    B_esa(2,3) = 0;


    B_esa(3,0) = -1;
    B_esa(3,1) = 0;
    B_esa(3,2) = 0;
    B_esa(3,3) = 1;


    B_esa(4,0) = 0;
    B_esa(4,1) = -1;
    B_esa(4,2) = 0;
    B_esa(4,3) = 1;
    if( B != B_esa){
        std::cout<<"\nB è sbagliato.\n con valore: \n"<<B<<"\n";
        
         return EXIT_FAILURE;
    }


    Eigen::VectorXd v_esa = Eigen::VectorXd::Zero(4); 
    v_esa(0) = - 35.7;
    v_esa(1) = - 30.6;
    v_esa(2) =  30.6 + 35.7;
    v_esa(3) = - 23.2;
    if (v != v_esa){
        std::cout<<"\nv è sbagliato.\n"<<v<<"\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
