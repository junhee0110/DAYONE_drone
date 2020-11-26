#include "config.h"
#include "sensor.h"
#include <I2Cdev.h>
#define OUTPUT_READABLE_YAWPITCHROLL
#include <MPU6050_6Axis_MotionApps20.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

void balance::get_state() {
  
    // if programming failed, don't try to do anything
    if (!dmpReady) return;
    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet 
        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            Serial.print("quat\t");
            Serial.print(q.w);
            Serial.print("\t");
            Serial.print(q.x);
            Serial.print("\t");
            Serial.print(q.y);
            Serial.print("\t");
            Serial.println(q.z);
        #endif

        #ifdef OUTPUT_READABLE_EULER
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
            Serial.print("euler\t");
            Serial.print(euler[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(euler[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(euler[2] * 180/M_PI);
        #endif

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("Yaw\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print("Pitch\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.print("Roll\t");
            Serial.println(ypr[2] * 180/M_PI);
        #endif

        #ifdef OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            Serial.print("areal\t");
            Serial.print(aaReal.x);
            Serial.print("\t");
            Serial.print(aaReal.y);
            Serial.print("\t");
            Serial.println(aaReal.z);
        #endif

        #ifdef OUTPUT_READABLE_WORLDACCEL
            // display initial world-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
            Serial.print("aworld\t");
            Serial.print(aaWorld.x);
            Serial.print("\t");
            Serial.print(aaWorld.y);
            Serial.print("\t");
            Serial.println(aaWorld.z);
        #endif
    
        #ifdef OUTPUT_TEAPOT
            // display quaternion values in InvenSense Teapot demo format:
            teapotPacket[2] = fifoBuffer[0];
            teapotPacket[3] = fifoBuffer[1];
            teapotPacket[4] = fifoBuffer[4];
            teapotPacket[5] = fifoBuffer[5];
            teapotPacket[6] = fifoBuffer[8];
            teapotPacket[7] = fifoBuffer[9];
            teapotPacket[8] = fifoBuffer[12];
            teapotPacket[9] = fifoBuffer[13];
            Serial.write(teapotPacket, 14);
            teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
        #endif

        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
    
  // MPU6050으로부터 가속도 자이로 센서의 가공하지 않은 데이터를 읽어
  // 들입니다
  this -> mpu.getMotion6(&(this->ax), &(this->ay), &(this->az), &(this->gx), &(this->gy), &(this->gz));

  // MPU6050을 통하여 HMC5883L 지자기 센서 데이터 값을 읽어 들입니다
  this -> mx = this -> mpu.getExternalSensorWord(0);
  this -> my = this -> mpu.getExternalSensorWord(2);
  this -> mz = this -> mpu.getExternalSensorWord(4);

  // 어디를 향하고 있는 heading 값을 구합니다. 만약 - 값이면 이를 +
  // 값으로 바꿉니다
  this -> heading = atan2(this -> my, this -> mx);
  if (this -> heading < 0) this -> heading += 2 * M_PI;

  this -> heading *= (180/3.14);
  
  // 높이를 측정하여 altitude에 저장합니다.
  this -> altitude = this -> getAltitude();

}

float balance::getAltitude() {
  float temp;
  temp = baro.getTemperature();
  if (temp) {
    this -> temperature = temp;
  }
  delay(100);

  this -> pressure = baro.getPressure();

  if (this -> pressure != NULL) {
    pushAvg(this -> pressure);
  }

  this -> pressure = this -> getAvg(this -> press_buff, MOVAVG_SIZE);

  return ((pow((this -> sea_press / this -> pressure), 1 / 5.257) - 1.0) * (this -> temperature + 273.15)) / 0.0065;
}

void balance::pushAvg(float val) {
  this -> press_buff[this -> press_avg_i] = val;
  this -> press_avg_i = (this -> press_avg_i + 1) % MOVAVG_SIZE;
}


float balance::getAvg(float * buff, int size) {
  float sum = 0.0;
  for (int i = 0; i < size; i++) {
    sum += buff[i];
  }
  return sum / size;
}

int16_t balance::get_ax(){
  return this->ax;
}
int16_t balance::get_ay(){
  return this->ay;
}
int16_t balance::get_az(){
  return this->az;
}

int16_t balance::get_gx(){
  return this->gx;
}
int16_t balance::get_gy(){
  return this->gy;
}
int16_t balance::get_gz(){
  return this->gz;
}

int16_t balance::get_mx(){
  return this->mx;
}
int16_t balance::get_my(){
  return this->my;
}
int16_t balance::get_mz(){
  return this->mz;
}

float balance::get_head(){
  return this->heading;
}

float balance::get_alt(){
  return this->altitude;
  
}
float balance::get_temp(){
  return this->temperature;
}
float balance::get_press(){
  return this->pressure;
}
