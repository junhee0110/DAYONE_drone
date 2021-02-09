<<<<<<< HEAD
// File name: motor.h
// Purpose: control motor
// Author: Lee Junhee, Hong Miru
// Last Edit: 20-02-09

#ifndef MOTOR
#define MOTOR

#include "Servo.h"

class motor: public Servo{
    private:
        int output;
        int pinNum;

    public:
        motor(int pin);
        void init();
        void rotate(int value);
};

//range of writeSignal : 700~2000
#endif
=======
// File name : motor.h
// Last Update : 2021-02-09
// Purpose : Control Motor
// Author : Hong Miru
//------------- Edit log --------------------

#ifndef CONTROL
#define CONTROL

class motor_control {

  private:
    int pin_num;
    int motor_num;
    bool x_sign;
    bool y_sign;
    bool dir;
    float output_val = 0.0;
    
  public:
    motor_control(int pin_num, int motor_num);
    void cal_output(axis &x, axis &y);
    void output();
};

#endif

/*
 * Motor Number 
 *  (FORWARD)
 *    2 0
 *    3 1   
 *   y
 *   |- x
 */
>>>>>>> 2b12d49d7d5fb19bae2d4828ec90b2b45553d3c5
