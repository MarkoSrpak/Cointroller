#ifndef MYLIB_H
#define MYLIB_H
#include <Arduino.h>


#define SERVO_PIN_0   10
#define SERVO_PIN_1   9
#define SERVO_PIN_2   8
#define SERVO_PIN_3   7
#define SERVO_PIN_4   6
#define SERVO_PIN_5   5
#define SERVO_PIN_6   4
#define SERVO_PIN_7   3
#define SERVO_PIN_TOP 2

#define COIL_PIN_OUT_1 40
#define COIL_PIN_OUT_2 44
#define COIL_PIN_IN_1 A8
#define COIL_PIN_IN_2 A9

#define IR_PIN_OUT_1 45
#define IR_PIN_IN_1 46
#define IR_PIN_OUT_2 47
#define IR_PIN_IN_2 48

#define SCALE_PIN_DOUT 20
#define SCALE_PIN_SCK 21

#define NUM_LEDS 1
#define LED_PIN  32

#define FREQ1 10000

#define IRdistance 171.3f
#define coilConstant 8.5/51

void setupServos();

void startCoils();
void stopCoils();
void readCoil();

void startIR();
void readIR();

void setupScale();
void readScale();

void loadCoins();

void setupLED();

typedef struct coin_t{
  String name;
  uint32_t coil1; //47ohm 10kHz
  uint32_t coil2; //56ohm 8kHz
  float sizePot;
  float sizeIR;
  float mass;
} coin_t;

const coin_t EUR_2 = {
  "2 EURA",
  840,
  0,
  0,
  25.77,
  8.45f
};
const coin_t EUR_1 = {
  "1 EURO",
  847,
  0,
  0,
  23.33f,
  7.50f
};
const coin_t CENT_50 = {
  "50 CENTI",
  833,
  0,
  0,
  23.93f, //rucno promijenjeno
  7.73f
};
const coin_t CENT_20 = {
  "20 CENTI",
  842,
  0,
  0,
  22.32f,
  5.73f
};
const coin_t CENT_10 = {
  "10 CENTI",
  858,
  0,
  0,
  19.68f,
  4.17f
};
const coin_t CENT_5 = {
  "5 CENTI",
  849,
  0,
  0,
  21.34f,
  3.90f
};
const coin_t CENT_2 = {
  "2 CENTA",
  868,
  0,
  0,
  18.42f,
  3.13f
};
const coin_t CENT_1 = {
  "1 CENT",
  899,
  0,
  0,
  15.13f,
  2.33f
};


#endif