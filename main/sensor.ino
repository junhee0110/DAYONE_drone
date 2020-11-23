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



void balance::init() {
  // I2C 버스를 사용하기 위하여 Wire.begin()를 실행합니다. (I2Cdev
  // 라이브러리에서 이를 자동으로 실행치 않기 때문입니다)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 12; // 400kHz I2C clock (200kHz if CPU is 8MHz)

#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  // 시리얼 통신을 115200bps로 초기화합니다
  Serial.begin(115200);
   while (!Serial);
  // I2C 버스에 연결되어 있는 기기들을 초기화합니다
  Serial.println(F("I2C 기기들 초기화 중..."));
  this -> mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  
  // 연결을 검증합니다.
  Serial.println(F("기기 연결 시험 중..."));
  Serial.print(F("MPU6050 연결 "));
  Serial.println(mpu.testConnection() ? F("정상") : F("오류"));

  /*
     지자기 센서 HMC5883L은 MPU6050의 보조 I2C 버스에 연결되어 있어,
     MPU6050을 통하여 지자기 센서를 읽어 낼 수 있습니다. 이를 위한
     환경 구성을 진행합니다.

     1. MPU6050의 보조 I2C 버스를 직접 R/W하기 위하여 마스터 모드를
        중지하고, 바이패스 모드를 활성화합니다.

     2. HMC5883L 모드를 연속 측정 모드로 측정 간격을 75Hz로 설정합니다

     3. 바이패스 모드를 비활성화(중지)합니다

     4. MPU6050에게 mx, my, mz를 읽어들이기 위한 HMC5883L 레지스터 주소
        그리고 읽어들인 데이터의 MSB와 LSB를 바꿔야 하는지, 데이터 크기
        등을 설정합니다.

     5. 최종적으로 HMC5883L로부터 지자기 센서 데이터는 MPU6050을 통하여
        읽어들인다고 마스터 모드를 활성화합니다.

  */
  this -> mpu.setI2CMasterModeEnabled(0);
  this -> mpu.setI2CBypassEnabled(1);

  Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
  Wire.write(0x02);
  Wire.write(0x00);  // 연속 읽기 모드로 설정
  Wire.endTransmission();
  delay(5);

  Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
  Wire.write(0x00);
  Wire.write(B00011000);  // 75Hz
  Wire.endTransmission();
  delay(5);
  
  this -> mpu.setI2CBypassEnabled(0);
  
    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again
  
   Serial.println(F("Initializing DMP..."));
   this->devStatus =this->mpu.dmpInitialize();
   
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);
    if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
        Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
        Serial.println(F(")..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));

        pinMode(LED_PIN, OUTPUT);
    }

  // X 축 WORD
  /* 0x80은 7번째 비트를 1로 만듭니다. MPU6050 데이터 자료에 읽기 모드
     설정은 '1'로 쓰기 모드 설정은 '0'로 하여야 한다고 나와 있습니다. */
  this -> mpu.setSlaveAddress(0, HMC5883L_DEFAULT_ADDRESS | 0x80);
  this -> mpu.setSlaveRegister(0, HMC5883L_RA_DATAX_H);
  this -> mpu.setSlaveEnabled(0, true);
  this -> mpu.setSlaveWordByteSwap(0, false);
  this -> mpu.setSlaveWriteMode(0, false);
  this -> mpu.setSlaveWordGroupOffset(0, false);
  this -> mpu.setSlaveDataLength(0, 2);

  // Y 축 WORD
  this -> mpu.setSlaveAddress(1, HMC5883L_DEFAULT_ADDRESS | 0x80);
  this -> mpu.setSlaveRegister(1, HMC5883L_RA_DATAY_H);
  this -> mpu.setSlaveEnabled(1, true);
  this -> mpu.setSlaveWordByteSwap(1, false);
  this -> mpu.setSlaveWriteMode(1, false);
  this -> mpu.setSlaveWordGroupOffset(1, false);
  this -> mpu.setSlaveDataLength(1, 2);

  // Z 축 WORD
  this -> mpu.setSlaveAddress(2, HMC5883L_DEFAULT_ADDRESS | 0x80);
  this -> mpu.setSlaveRegister(2, HMC5883L_RA_DATAZ_H);
  this -> mpu.setSlaveEnabled(2, true);
  this -> mpu.setSlaveWordByteSwap(2, false);
  this -> mpu.setSlaveWriteMode(2, false);
  this -> mpu.setSlaveWordGroupOffset(2, false);
  this -> mpu.setSlaveDataLength(2, 2);

  // HMC5883L은 MPU6050을 통하여 읽을 수 있어, 마스터 모드를 ON합니다
  this -> mpu.setI2CMasterModeEnabled(1);

  // 기압계를 초기화합니다
  this -> baro.init();
  delay(100);

  // 평균 압력 정보를 얻기 위하여 미리 값을 초기화 합니다
  for (int i = 0; i < MOVAVG_SIZE; i++) {
    float p = this -> baro.getPressure();
    if (p == NULL)
      p = STANDARD_SEA_PRESSURE;

    this -> press_buff[i] = p;

  }
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
