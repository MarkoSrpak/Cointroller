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

#define SCALE_PIN_DOUT 30
#define SCALE_PIN_SCK 31

#define IRdistance 175

void setupServos();

void startCoils();
void stopCoils();
void readCoil();

void startIR();
void readIR();

void setupScale();
void readScale();

typedef struct coin_t{
  String name;
  uint32_t coil1;
  uint32_t coil2;
  float sizePot;
  float sizeIR;
  float mass;
} coin_t;

#endif