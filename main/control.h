#ifndef CONTROL
#define CONTROL

class motor_control {

  private:
    int pin_num;
    bool dir;
    float output_val = 0.0;
    
  public:
    motor_control(int pin_num, bool dir);
    void cal_output(axis &one, axis &two);
    void cal_output(axis &one, axis &two, axis &three);
    void cal_output(axis &one, axis &two, axis &three, axis &four);
    void output();
};

#endif
