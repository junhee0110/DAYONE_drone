#include "sensor.h"

void gy86::init(){
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  MPU6050::initialize();
  test_connection = MPU6050::testConnection();
  devStatus = MPU6050::dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  MPU6050::setXGyroOffset(51);
  MPU6050::setYGyroOffset(8);
  MPU6050::setZGyroOffset(21);
  MPU6050::setXAccelOffset(1150);
  MPU6050::setYAccelOffset(-50);
  MPU6050::setZAccelOffset(1060);

   // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
    CalibrateAccel(6);
    MPU6050::CalibrateGyro(6);
    MPU6050::PrintActiveOffsets();
    // turn on the DMP, now that it's ready
    MPU6050::setDMPEnabled(true);
}