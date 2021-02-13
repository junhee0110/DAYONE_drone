#ifndef PID_AXIS
#define PID_AXIS

class PID {
  
  private:
    float kP1, kP2, kI, kD;
    float angle_target, angle_current, rate_current;
    float dt;

  public:
    PID(float kP1, float kP2, float kI, float kD);
    
    void set_angle_target(float angle_target);
    void set_angle_current(float angle_current);
    void set_rate_current(float rate_current);
    void set_const(float kP1, float kP2, float kI, float kD);
    void set_dt(float dt);

    float cal_angle_err();
    float P_angle();
    float cal_rate_err();
    float cal_inte();
    float cal_diff();
    float cal_PID();

    float get_PID();
    float get_kP1();
    float get_kP2();
    float get_kI();
    float get_kD();
    float get_angle_err();

        
};

#endif
