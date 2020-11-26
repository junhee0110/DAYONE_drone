#include "gy86.h"


gy86::gy86(){
    MPU6050();
    MS561101BA();
}

void gy86::init(){

    MPU6050::initialize();
    #ifdef INTERRUPT
    pinMode(INTERRUPT_PIN, INPUT);
    #endif
    mpu_connection = MPU6050::testConnection();

    //To directly Read/Write sub I2C bus, Enable BypassMode
    MPU6050::setI2CMasterModeEnabled(0);
    MPU6050::setI2CBypassEnabled(1);

    // 연속 읽기 모드로 설정
    Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
    Wire.write(0x02);
    Wire.write(0x00);  
    Wire.endTransmission();
    delay(5);

    // 측정 간격을 75Hz
    Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
    Wire.write(0x00);
    Wire.write(B00011000);  
    Wire.endTransmission();
    delay(5);

    MPU6050::setI2CBypassEnabled(0);

    //Initializing DMP6
    devStatus = MPU6050::dmpInitialize();

    //Set gyro offset
    MPU6050::setXGyroOffset(220);
    MPU6050::setYGyroOffset(76);
    MPU6050::setZGyroOffset(-85);
    MPU6050::setZAccelOffset(1788);

    //re-check that it worked well
    if (devStatus == 0) {
        MPU6050::CalibrateAccel(6);
        MPU6050::CalibrateGyro(6);
        MPU6050::PrintActiveOffsets();

        MPU6050::setDMPEnabled(true);

        #ifdef INTERRPUT
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN),  dmpDataReady, RISING);
        #endif

        mpuIntStatus = MPU6050::getIntStatus();
        dmpReady = true;
        
        packetSize = MPU6050::dmpGetFIFOPacketSize();}

        else{
            //Error
            // 1 = initial memory load failed
            // 2 = DMP configuration updates failed

            //HAVE TO DISCUSS
            //HOW TO SEND ERROR MESSAGE
        }

        yaw_base = atan2(MPU6050::getExternalSensorWord(2),MPU6050::getExternalSensorWord(0));
        if (yaw_base < 0) yaw_base += 2 *M_PI;

        #ifdef MPU_LED
        pinMode(MPU_LED, OUTPUT);
        #endif
}

void gy86::get_state(){

    DMP6:
    if (!dmpReady) goto HMC; // if programming failed, don't try to do anything
    if (MPU6050::dmpGetCurrentFIFOPacket(fifoBuffer)){
        
        MPU6050::dmpGetQuaternion(&q, fifoBuffer);
        MPU6050::dmpGetGravity(&gravity, &q);
        MPU6050::dmpGetYawPitchRoll(ypr, &q, &gravity);
        pitch = ypr[1] * 180/M_PI, roll = ypr[2] * 180/M_PI; //Yaw => HMC

        //#ifdef REALACCEL
        MPU6050::dmpGetLinearAccel(&aaReal, &aa, &gravity);
        //#endif

        #ifdef MPU_LED
        blinkState = !blinkState;
        digitalWrite(MPU_LED, HIGH);
        #endif
    }

    HMC:
        heading = atan2(MPU6050::getExternalSensorWord(2),MPU6050::getExternalSensorWord(0));
        if (heading < 0) heading += 2 *M_PI;

        yaw = heading - yaw_base;
        if (yaw < 0) yaw += 2*M_PI;
        if (yaw > 180) yaw -= 2*M_PI; // yaw = -180~180

        yaw *= (180/M_PI);
    ALT:
    altitude = getAltitude();

}

float gy86::getAltitude(){
    float temp;
    temp = MS561101BA::getTemperature();
    if (temp) {
        temperature = temp;
    }
    delay(100);

    pressure = MS561101BA::getPressure();

    if(pressure != NULL){
        pushAvg(pressure);
    }

    pressure = getAvg(press_buff, MOVAVG_SIZE);

    return ((pow(sea_press/pressure, 1/ 5.257)-1)*(temperature +273.15)) / 0.0065;

}

void gy86::pushAvg(float val){
    press_buff[press_avg_i] = val;
    press_avg_i = (press_avg_i + 1) % MOVAVG_SIZE;
}

 float gy86::getAvg(float *buff, int size){
     float sum = 0.0;
     for (int i = 0; i< size; i++){
         sum += buff[i];
     }

     return sum /size;
 }

 
float gy86::get_yaw(){
    return yaw;
}
float gy86::get_pitch(){
    return pitch;
}
float gy86::get_roll(){
    return roll;
}
float gy86::get_alt(){
    return altitude;
}
float gy86::get_head(){
    return heading;
}