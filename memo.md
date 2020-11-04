# 20-11-04

## config.h

---

### dT계산 방식

- FIX_T - 정의된 시간(ms)로 dT를 고정합니다

- FLEX_T - 루프가 반복되는 시간 간격을 측정하여 dT를 계산합니다.

## PID.h

---

### class axis 

- purpose : 각 축의 에러를 쉽게 계산하고 PID값을 계산하기 위함 

#### Attributes

- kP, kI, kD - PID 계수
- dt - 미분, 적분 계산을 위한 시간 미분
- t_tmp - 미적분 계산을 위한 이전 루프의 millis() 함수 값
- e_tmp - 미분 계산을 위한 이전 루프의 오차
- err - 현재 루프의 오차
- inte ,diff - 적분값, 미분값
- PID_val - 계산된 PID값 

#### Methods

- cal_err(float target, float current) - err를 계산해서 갱신
- cal_dt() - dT를 계산해서 갱신
- cal_inte() - 적분값을 계산해서 갱신
- cal_diff() - 미분값을 계산해서 갱신
- cal_PID() - PID값을 계산해서 갱신
- get_PID() - PID값을 외부로 전달하는 함수
- get_kP(), get_kI(), get_kD() - PID계수를 외부로 전달하는 함수
- get_err() - 오차값을 외부로 전달하는 함수
- set_const(float kP, float kI, float kD) - PID계수를 새롭게 갱신하는 함수

## PID.ino

---

- class axis의 methods 정의

## main.ino

---

- 정의된 함수를 적절히 불러오는 main 파일



# TODO LIST

1. control.ino, control.h - PID값을 이용하여 드론을 제어
2. sensor.ino, sensor,h - 자이로, GPS 등의 센서값을 읽어와서 전달
3. communication.ino, communication.h - 조종기와 통신