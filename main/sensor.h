#ifndef GY86_
#define GY86_

#include "MS561101BA.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"

#define HMC5883L_DEFAULT_ADDRESS    0x1E
#define HMC5883L_RA_DATAX_H         0x03
#define HMC5883L_RA_DATAZ_H         0x05
#define HMC5883L_RA_DATAY_H         0x07

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif

class gy86 : public MPU6050, public MS561101BA {

  private:
    // MPU control/status vars
    bool test_connection = false;
    bool dmpReady = false;  // set true if DMP init was successful
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer

    // orientation/motion vars
    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    int16_t mx, my, mz;
    float heading;

    // packet structure for InvenSense teapot demo
    uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };

  public:
    void init();
    void get_dmp();
    void get_heading();
    float get_yaw();
    float get_pitch();
    float get_roll();
    float get_head();
};

class gy86_2{
  private:
    private:
    MPU6050 mpu;
    MS561101BA baro;

    // MPU control/status vars
    bool test_connection = false;
    bool dmpReady = false;  // set true if DMP init was successful
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer

    // orientation/motion vars
    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    int16_t mx, my, mz;
    float heading;

    // packet structure for InvenSense teapot demo
    uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };

  public:
    void init();
    void get_dmp();
    void get_heading();
    float get_yaw();
    float get_pitch();
    float get_roll();
    float get_head();
};

#endif
