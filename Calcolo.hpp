#include <Eigen/Dense>
Eigen::VectorXd Calcolo(Eigen::MatrixXd& B, Eigen::MatrixXd& R , Eigen::VectorXd& v, Eigen::VectorXd& i0, double tol){
    Eigen::MatrixXd A = B.transpose()*R*B; //dato che la diagonale di R è sempre positiva e B è una matice di incidenza                                         incidenza allora A sarà sempre una matrice simmetrica e definita positiva.
    i0 = gradiente_coniugato(A, v, i0, tol);
    Eigen::VectorXd Vr = R*B*i0;

    return Vr;
}
