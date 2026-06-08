#include <Eigen/Dense>
Eigen:MatrixXd Calcolo(Eigen::MatrixXd& B, Eigen::MatrixXd& R , Eigen::VectorXd& v, Eigen::VectorXd& i0, double tol){
    Eigen::MatrixXd A = B.transpose()*R*B; //dato che la diagonale di R è sempre positiva e B è una matice di incidenza                                         incidenza allora A sarà sempre una matrice simmetrica e definita positiva.
    Eigen::VectorXd i = gradiente_coniugato(A, v, i0, tol);
    Eigen::VectorXd Vr = R*B*i0;
    Eigen::MatrixXd Risultato;
    for(int i = 0; i<Vr.size(); i++){
        Risultato(i,0) = Vr[i];
        Risultato(i,1) = Vr[i]/R[i][i];
    }
    
    return Risultato;
}
