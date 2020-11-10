#include "config.h"
#include "sensor.h"



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

  // I2C 버스에 연결되어 있는 기기들을 초기화합니다
  Serial.println(F("I2C 기기들 초기화 중..."));
  this -> mpu.initialize();

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

  this -> get_state();
  this -> angle_x = this -> angle_ax;
  this -> angle_y = this -> angle_ay;
}

void balance::get_state() {
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

  this -> heading *= (180 / PI);

  // 높이를 측정하여 altitude에 저장합니다.
  this -> altitude = this -> getAltitude();

  //angle_acc
  this -> angle_ax = atan(this->ay / sqrt(pow(this -> ax, 2) + pow(this -> az, 2)))*(180/PI);
  this -> angle_ay = atan(-this->ax / sqrt(pow(this -> ay, 2) + pow(this -> az, 2)))*(180/PI);

  //filter

  this -> angle_x = this -> angle_ax;
  this -> angle_y = this -> angle_ay;
}
float balance::getAltitude() {
  float temp;
  temp = baro.getTemperature();
  if (temp) {
    this -> temperature = temp;
  }
  //delay(100);

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
}

double balance::get_angle_x() {
  return this->angle_x;
}

double balance::get_angle_y() {
  return this->angle_y;
}
