#ifndef SENSOR
#define SENSOR

#include <MS561101BA.h>
#include "I2Cdev.h"
#include "MPU6050.h"
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

    
    float heading;
    float altitude, temperature, pressure;
    

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
