#include <Eigen/Dense>
#include "gradiente_coniugato.hpp"
Eigen::MatrixXd Calcolo(Eigen::MatrixXd& B, Eigen::MatrixXd& R , Eigen::VectorXd& v, double tol){
    Eigen::MatrixXd A = B.transpose()*R*B; //dato che la diagonale di R è sempre positiva e B è una matice di incidenza allora A sarà sempre una matrice simmetrica e definita positiva
    Eigen::VectorXd i0 = Eigen::VectorXd::Zero(B.cols()); //B colonne = numero di maglie
    Eigen::VectorXd i = gradiente_coniugato(A, v, i0, tol); // correnti di maglia 
    Eigen::VectorXd Vr = R*B*i; //potenziale su ogni resistenza che ha come dimensione il numero delle resistenze
    Eigen::MatrixXd Risultato = Eigen::MatrixXd::Zero(Vr.size(), 2); //definisco Il risultato come una matrice con la prima colonna pari al potenziale associato alle resistenze e 
                                                                    // e come seconda colonna le correnti associate alle resistenze
    for(int j = 0; j <Vr.size(); j++){
        Risultato(j,0) = Vr[j];
        Risultato(j,1) = Vr[j]/R(j,j); // legge di Ohm
    }
    
    return Risultato;
}
