#include "kalman_filter.h"
#include "tools.h"
#include <iostream>


using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;



KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {

  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */

  x_ = F_ * x_;
  MatrixXd Ft_ = F_.transpose();
  P_ = F_ * P_ * Ft_ + Q_;

}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
  VectorXd z_pred = H_ * x_;
  VectorXd y = z - z_pred;
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd K =  P_ * Ht * Si;

  //new state
  cout << "what is y value " << y << endl;
  x_ = x_ + (K * y);

  long x_size = x_.size();

  MatrixXd I;
  I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;

}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
    * z = raw_measurement input(ro,phi,ro dot)
  */
    Tools tools;

    //cartesian to polar
    VectorXd h_ = tools.CalculateHX(x_);

    VectorXd y = z - h_;


    if (y(1)>M_PI){
        y(1) -= 2*M_PI;
    }
    if (y(1)<-M_PI){
        y(1) += 2*M_PI;
    }


    MatrixXd H_ = tools.CalculateJacobian(x_);

    MatrixXd Ht = H_.transpose();
    MatrixXd S = H_ * P_ * Ht + R_;
    MatrixXd Si = S.inverse();
    MatrixXd K =  P_ * Ht * Si;

    //new state
    cout << "what is y value " << y << endl;
    cout << "what is K value " << K << endl;
    cout << "what is S value " << S << endl;
    cout << "what is H_ value " << H_ << endl;
    cout << "what is P_ value " << P_ << endl;


    x_ = x_ + (K * y);

    long x_size = x_.size();

    MatrixXd I;
    I = MatrixXd::Identity(x_size, x_size);
    P_ = (I - K * H_) * P_;



}
