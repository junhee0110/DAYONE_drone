void balance::init() {
  // I2C 버스를 사용하기 위하여 Wire.begin()를 실행합니다. (I2Cdev
  // 라이브러리에서 이를 자동으로 실행치 않기 때문입니다)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 12; // 400kHz I2C clock (200kHz if CPU is 8MHz)

#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

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
