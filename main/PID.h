// File Name : PID.h
// Purpose : declares class PID
// last edit : 2021-02-13
// Author : Hwang Tae-on, Lee Junhee
//------------- Edit log --------------------
// 21-02-08 First Edition of double loop PID  (Tae-on)
// 21-02-13 Second Edition of double loop PID (Junhee)

#ifndef PID_AXIS
#define PID_AXIS

class PID {
  
  private:
    //Identifier
    int number; // YAW, PITCH, ROLL, X, Y, Z

    float kP1, kP2, kI, kD; // PID Constants
    float angle_target, angle_current, rate_target; // values for calculate error
    float dt; // sampling time
    float angle_err, rate_err, rate_err_tmp; // Error values
    float P_first, I_val, PID_val; // PID values

  public:
    PID(int num, float kP1 = 1.0f, float kP2 = 1.0f, float kI = 1.0f, float kD = 1.0f); //Constructor
    
    // Set Method (From other instance)
    void set_first_target(float angle_target);
    void set_first_current(float angle_current);
    void set_rate_target(float rate_target);
    void set_const(float kP1, float kP2, float kI, float kD);
    void set_dt(float dt);

    // Calculation Method
    void cal_angle_err();
    void P_angle();
    void cal_rate_err();
    void cal_PID();

    // Get Method (To other instance)
    float get_PID();
    float get_kP1();
    float get_kP2();
    float get_kI();
    float get_kD();
    int get_num();
};

#endif
