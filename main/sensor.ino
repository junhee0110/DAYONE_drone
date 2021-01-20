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
    
    MPU6050::CalibrateAccel(6);
    MPU6050::CalibrateGyro(6);
    MPU6050::PrintActiveOffsets();
    // turn on the DMP, now that it's ready
    MPU6050::setDMPEnabled(true);

    mpuIntStatus = MPU6050::getIntStatus();

    dmpReady = true;

    packetSize = MPU6050::dmpGetFIFOPacketSize();

    //HMC5883L setting

    MPU6050::setI2CMasterModeEnabled(0);
    MPU6050::setI2CBypassEnabled(1);

    Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.endTransmission();
    delay(5);

    Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
    Wire.write(0x00);
    Wire.write(B00011000); //75Hz
    Wire.endTransmission();
    delay(5);

    MPU6050::setI2CBypassEnabled(0);

    //X axis
    MPU6050::setSlaveAddress(0, HMC5883L_DEFAULT_ADDRESS | 0x80);
    MPU6050::setSlaveRegister(0, HMC5883L_RA_DATAX_H);
    MPU6050::setSlaveEnabled(0, true);
    MPU6050::setSlaveWordByteSwap(0, false);
    MPU6050::setSlaveWriteMode(0, false);
    MPU6050::setSlaveWordGroupOffset(0, false);
    MPU6050::setSlaveDataLength(0, 2);

    //Y axis
    MPU6050::setSlaveAddress(1, HMC5883L_DEFAULT_ADDRESS | 0x80);
    MPU6050::setSlaveRegister(1, HMC5883L_RA_DATAY_H);
    MPU6050::setSlaveEnabled(1, true);
    MPU6050::setSlaveWordByteSwap(1, false);
    MPU6050::setSlaveWriteMode(1, false);
    MPU6050::setSlaveWordGroupOffset(1, false);
    MPU6050::setSlaveDataLength(1, 2);

    //Z axis
    MPU6050::setSlaveAddress(2, HMC5883L_DEFAULT_ADDRESS | 0x80);
    MPU6050::setSlaveRegister(2, HMC5883L_RA_DATAZ_H);
    MPU6050::setSlaveEnabled(2, true);
    MPU6050::setSlaveWordByteSwap(2, false);
    MPU6050::setSlaveWriteMode(2, false);
    MPU6050::setSlaveWordGroupOffset(2, false);
    MPU6050::setSlaveDataLength(2, 2);

    MPU6050::setI2CMasterModeEnabled(1);
}
}

void gy86::get_dmp(){

  if(!dmpReady) return;
  if(MPU6050::dmpGetCurrentFIFOPacket(fifoBuffer)){

  MPU6050::dmpGetQuaternion(&q, fifoBuffer);
  MPU6050::dmpGetGravity(&gravity, &q);
  MPU6050::dmpGetYawPitchRoll(ypr, &q, &gravity);
  }
}

void gy86::get_heading(){
  mx = MPU6050::getExternalSensorWord(0);
  my = MPU6050::getExternalSensorWord(2);
  mz = MPU6050::getExternalSensorWord(4);

  heading = atan2(my, mx);
  if(heading < 0) heading += 2*M_PI;
}


float gy86::get_yaw(){
  return ypr[0];

}

float gy86::get_pitch(){
  return ypr[1];
}

float gy86::get_roll(){
  return ypr[2];
}

float gy86::get_head(){
  return heading;
}

void gy86_2::init(){
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  mpu.initialize();
  test_connection = mpu.testConnection();
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(51);
  mpu.setYGyroOffset(8);
  mpu.setZGyroOffset(21);
  mpu.setXAccelOffset(1150);
  mpu.setYAccelOffset(-50);
  mpu.setZAccelOffset(1060);


   // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);

    mpuIntStatus = mpu.getIntStatus();

    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize();

    //HMC5883L setting

    mpu.setI2CMasterModeEnabled(0);
    mpu.setI2CBypassEnabled(1);

    Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.endTransmission();
    delay(5);

    Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
    Wire.write(0x00);
    Wire.write(B00011000); //75Hz
    Wire.endTransmission();
    delay(5);

    mpu.setI2CBypassEnabled(0);

    //X axis
    mpu.setSlaveAddress(0, HMC5883L_DEFAULT_ADDRESS | 0x80);
    mpu.setSlaveRegister(0, HMC5883L_RA_DATAX_H);
    mpu.setSlaveEnabled(0, true);
    mpu.setSlaveWordByteSwap(0, false);
    mpu.setSlaveWriteMode(0, false);
    mpu.setSlaveWordGroupOffset(0, false);
    mpu.setSlaveDataLength(0, 2);

    //Y axis
    mpu.setSlaveAddress(1, HMC5883L_DEFAULT_ADDRESS | 0x80);
    mpu.setSlaveRegister(1, HMC5883L_RA_DATAY_H);
    mpu.setSlaveEnabled(1, true);
    mpu.setSlaveWordByteSwap(1, false);
    mpu.setSlaveWriteMode(1, false);
    mpu.setSlaveWordGroupOffset(1, false);
    mpu.setSlaveDataLength(1, 2);

    //Z axis
    mpu.setSlaveAddress(2, HMC5883L_DEFAULT_ADDRESS | 0x80);
    mpu.setSlaveRegister(2, HMC5883L_RA_DATAZ_H);
    mpu.setSlaveEnabled(2, true);
    mpu.setSlaveWordByteSwap(2, false);
    mpu.setSlaveWriteMode(2, false);
    mpu.setSlaveWordGroupOffset(2, false);
    mpu.setSlaveDataLength(2, 2);

    mpu.setI2CMasterModeEnabled(1);
}
}

void gy86_2::get_dmp(){

  if(!dmpReady) return;
  if(mpu.dmpGetCurrentFIFOPacket(fifoBuffer)){

  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  }
}

void gy86_2::get_heading(){
  mx = mpu.getExternalSensorWord(0);
  my = mpu.getExternalSensorWord(2);
  mz = mpu.getExternalSensorWord(4);

  heading = atan2(my, mx);
  if(heading < 0) heading += 2*M_PI;
}


float gy86_2::get_yaw(){
  return ypr[0];
}

float gy86_2::get_pitch(){
  return ypr[1];
}

float gy86_2::get_roll(){
  return ypr[2];
}

float gy86_2::get_head(){
  return heading;
=======
int16_t balance::get_ax() {
  return this->ax;
}
int16_t balance::get_ay() {
  return this->ay;
}
int16_t balance::get_az() {
  return this->az;
}

int16_t balance::get_gx() {
  return this->gx;
}
int16_t balance::get_gy() {
  return this->gy;
}
int16_t balance::get_gz() {
  return this->gz;
}

int16_t balance::get_mx() {
  return this->mx;
}
int16_t balance::get_my() {
  return this->my;
}
int16_t balance::get_mz() {
  return this->mz;
}

float balance::get_head() {
  return this->heading;
}

float balance::get_alt() {
  return this->altitude;
}
float balance::get_temp() {
  return this->temperature;
}
float balance::get_press() {
  return this->pressure;

