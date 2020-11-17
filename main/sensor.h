#ifndef SENSOR
#define SENSOR

#include <MS561101BA.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif

#define HMC5883L_DEFAULT_ADDRESS    0x1E
#define HMC5883L_RA_DATAX_H         0x03
#define HMC5883L_RA_DATAZ_H         0x05
#define HMC5883L_RA_DATAY_H         0x07

#define MOVAVG_SIZE             32
#define STANDARD_SEA_PRESSURE  1013.25

class balance {

  private:
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    int16_t mx, my, mz;
   bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


    float press_buff[MOVAVG_SIZE];
    int press_avg_i = 0;
    const float sea_press = 1030.4;

    MPU6050 mpu;
    MS561101BA baro = MS561101BA();

  public:
    void init();
    void get_state();
    float getAltitude();
    void pushAvg(float val);
    float getAvg(float *buff, int size);

    int16_t get_ax();
    int16_t get_ay();
    int16_t get_az();

    int16_t get_gx();
    int16_t get_gy();
    int16_t get_gz();

    int16_t get_mx();
    int16_t get_my();
    int16_t get_mz();

    float get_head();
    float get_alt();
    float get_temp();
    float get_press();
    
};
#endif
