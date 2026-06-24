#include <Eigen/Dense>


Eigen::VectorXd   gradiente_coniugato(Eigen::MatrixXd& A, Eigen::VectorXd& b, Eigen::VectorXd& x0,double tol){
    Eigen::VectorXd r0 = b - A*x0; // 
    Eigen::VectorXd p0 = r0;
    float n  = std::sqrt(A.size());  // faccio la radice perché il comado .size() in una matrice ritorna 9 = 3*3 se n = 3;
    int k = 0;
    Eigen::VectorXd x; // definisco x come vettore soluzione
   
    while (k<10*n && r0.norm()>tol){
        double alpha_k = ((p0.transpose()*r0) / (p0.transpose()*A *p0)).value(); //definisco ad ogni iterazione alpha_k
        x = x0 + alpha_k*p0;
        r0 = b - A*x;
        double betha_k = ((p0.transpose()*A*r0) / (p0.transpose()*A *p0)).value();  //definisco ad ogni iterazione  beta_k
        p0 = r0 - betha_k*p0;
        k = k+1;
        x0  = x;
    }
    
    return x;

}
