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
#define IR_PIN_IN_1 49
#define IR_PIN_OUT_2 47
#define IR_PIN_IN_2 48

#define BUTTON_PIN_1 35
#define BUTTON_PIN_2 33
#define BUTTON_PIN_3 29
#define BUTTON_PIN_4 31

#define POT_PIN A10

#define SCALE_PIN_DOUT 20
#define SCALE_PIN_SCK 21

#define LCD_PIN_RS 41
#define LCD_PIN_E 34
#define LCD_PIN_D4 36
#define LCD_PIN_D5 37
#define LCD_PIN_D6 38
#define LCD_PIN_D7 39

#define BUZZER_PIN 30

#define NUM_LEDS 1
#define LED_PIN  32

#define FREQ1 10000

#define IRdistance 171.3f
#define coilConstant 8.5/51

#define THRESHOLD 50
#define CALNUM 5


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
void resetLED();
void setupButtons();
void setupPot();
void readButtons();
void readPot();
void ledColorRGB(uint8_t r, uint8_t g, uint8_t b);
void processCoin();
void calibrateCoin();

void setupBuzzer();

void setupLCD();

typedef struct coin_t{
  String name;
  uint32_t coil1; //47ohm 10kHz
  uint32_t coil2; //56ohm 8kHz
  uint32_t sizePot;
  float sizeIR;
  float mass;
  float value;
} coin_t;

const coin_t COIN1 = {
  "2 EURA",         //NAME
  856,              //COIL1
  0,                //COIL2
  638,                //SIZEPOT
  24.58,            //SIZEIR
  8.5f,            //MASS
  2.0f               //VALUE
};
const coin_t COIN2 = {
  "1 EURO",         //NAME
  865,              //COIL1
  0,                //COIL2
  682,                //SIZEPOT
  22.87,            //SIZEIR
  7.4f,            //MASS
  1.0f               //VALUE
};
const coin_t COIN3 = {
  "50 CENTI",         //NAME
  853,             //COIL1
  0,                //COIL2
  666,                //SIZEPOT
  23.59,            //SIZEIR
  7.70f,            //MASS
  0.5f               //VALUE
};
const coin_t COIN4 = {
  "20 CENTI",         //NAME
  859,              //COIL1
  0,                //COIL2
  695,                //SIZEPOT
  21.52,            //SIZEIR
  5.63f,            //MASS
  0.2f               //VALUE
};
const coin_t COIN5 = {
  "10 CENTI",         //NAME
  874,              //COIL1
  0,                //COIL2
  724,                //SIZEPOT
  19.29,            //SIZEIR
  4.03f,            //MASS
  0.1               //VALUE
};
const coin_t COIN6 = {
  "5 CENTI",         //NAME
  865,              //COIL1
  0,                //COIL2
  706,                //SIZEPOT
  20.8,            //SIZEIR
  3.83f,            //MASS
  0.05              //VALUE
};
const coin_t COIN7 = {
  "2 CENTA",         //NAME
  880,              //COIL1
  0,                //COIL2
  733,                //SIZEPOT
  18.11,            //SIZEIR
  2.93f,            //MASS
  0.02              //VALUE
};
const coin_t COIN8 = {
  "1 CENT",         //NAME
  917,              //COIL1
  0,                //COIL2
  759,                //SIZEPOT
  14.55,            //SIZEIR
  2.17f,            //MASS
  0.01              //VALUE
};


#endif