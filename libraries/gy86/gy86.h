#ifndef GY86
#define GY86

#include "Arduino.h"

#include "MS561101BA.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"


#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif

#define HMC5883L_DEFAULT_ADDRESS    0x1E
#define HMC5883L_RA_DATAX_H         0x03
#define HMC5883L_RA_DATAZ_H         0x05
#define HMC5883L_RA_DATAY_H         0x07

#define MOVAVG_SIZE             32
#define STANDARD_SEA_PRESSURE  1013.25

#define INTERRUPT_PIN 2

class gy86 :public MPU6050, public MS561101BA{

public:
    //Common part
    gy86();//initialize
    void init();//Setup
    void get_state();//Update state
    
    float get_yaw();
    float get_pitch();
    float get_roll();
    float get_alt();
    float get_head();

    //Barometer
    float getAltitude();//get altitude using temp and pressure
    void pushAvg(float val);//to use mean filter, update array
    float getAvg(float *buff, int size);//mean filter
    //Gyro
    //EMPTY

private:
    float yaw, pitch, roll;

    bool blinkState = false;
    bool mpu_connection;
    float heading, yaw_base;
    float altitude, temperature, pressure;

    
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

    volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

    // packet structure for InvenSense teapot demo
    uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

    float press_buff[MOVAVG_SIZE];
    int press_avg_i = 0;
    const float sea_press = 1026.4;



};


#endif