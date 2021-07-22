#include "PID.h"
#include <iostream>

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_, double _outMax, double _outMin) {

  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;

  p_error= i_error = i_error  = 0;

  outMax = _outMax;
  outMin = _outMin;

  initialStep = true;
}

void PID::UpdateError(double cte) {

  d_error= cte - p_error;
  p_error = cte;
  i_error += cte;
  
  // Get initial correct error for I and D
  if (initialStep ){
    i_error = d_error = 0;
    initialStep =false;
  }

}

double PID::TotalError() {
  // Compute integral term
  double int_term = Ki * i_error;

  // Check integral + antiwindup
  if(int_term> outMax) int_term = Ki * outMax;
  else if(int_term< outMin) int_term= Ki * outMin;

  // Compute output
  double sol = Kp * p_error + int_term + Kd * d_error ; 

  //Sateurate  output
  if(sol> outMax) sol= outMax;
  else if(sol< outMin) sol= outMin;

  std::cout << std::endl;
  std::cout << "out:" << sol << " P: " << Kp*p_error <<  "  I: " << int_term << "  D: " << Kd*d_error << std::endl;
  std::cout << std::endl;

  return sol;
}
