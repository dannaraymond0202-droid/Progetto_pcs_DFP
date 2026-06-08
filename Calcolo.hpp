#include <Eigen/Dense>
#include "gradiente_coniugato.hpp"
Eigen::MatrixXd Calcolo(Eigen::MatrixXd& B, Eigen::MatrixXd& R , Eigen::VectorXd& v, double tol){
    Eigen::MatrixXd A = B.transpose()*R*B; //dato che la diagonale di R è sempre positiva e B è una matice di incidenza allora A sarà sempre una matrice simmetrica e definita positiva
    Eigen::VectorXd i0 = Eigen::VectorXd::Zero(B.cols());
    Eigen::VectorXd i = gradiente_coniugato(A, v, i0, tol);
    Eigen::VectorXd Vr = R*B*i;
    Eigen::MatrixXd Risultato = Eigen::MatrixXd::Zero(Vr.size(), 2);
    for(int j = 0; j <Vr.size(); j++){
        Risultato(j,0) = Vr[j];
        Risultato(j,1) = Vr[j]/R(j,j);
    }
    
    return Risultato;
}
