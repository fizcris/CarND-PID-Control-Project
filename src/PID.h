#ifndef PID_H
#define PID_H

class PID {
 public:
  /**
   * Constructor
   */
  PID();

  /**
   * Destructor.
   */
  virtual ~PID();

  /**
   * Initialize PID.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
  void Init(double Kp_, double Ki_, double Kd_, double _outMax, double _outMin);

  /**
   * Update the PID error variables given cross track error.
   * @param cte The current cross track error
   */
  void UpdateError(double cte);

  /**
   * Calculate the total PID error.
   * @output The total PID error
   */
  double TotalError();

  /**
   * Check steering limits.
   */
  double checkLimits();



 private:
     /**
   * Initial Step.
   */
  bool initialStep;
  /**
   * PID Errors
   */
  double p_error;
  double i_error;
  double d_error;

    /**
   * Limits
   */
  double outMax;
  double outMin;

  /**
   * PID Coefficients
   */ 
  double Kp;
  double Ki;
  double Kd;
};

#endif  // PID_H