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
  839,
  0,
  0,
  25.75,
  8.5f
};
const coin_t EUR_1 = {
  "1 EURO",
  846,
  0,
  0,
  23.25f,
  7.50f
};
const coin_t CENT_50 = {
  "50 CENTI",
  834,
  0,
  0,
  23.90f, //rucno promijenjeno
  7.80f
};
const coin_t CENT_20 = {
  "20 CENTI",
  842,
  0,
  0,
  22.25f,
  5.74f
};
const coin_t CENT_10 = {
  "10 CENTI",
  0,
  0,
  0,
  19.75f,
  4.10f
};
const coin_t CENT_5 = {
  "5 CENTI",
  0,
  0,
  0,
  21.25f,
  3.92f
};
const coin_t CENT_2 = {
  "2 CENTA",
  0,
  0,
  0,
  18.75f,
  3.06f
};
const coin_t CENT_1 = {
  "1 CENT",
  0,
  0,
  0,
  16.25f,
  2.30f
};


#endif