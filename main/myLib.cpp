#include "Arduino.h"
#include "myLib.h"
#include <Servo.h>
#include "HX711.h"
#include <FastLED.h>

//-------------------------------- RGB LED ---------------------------------------//
CRGB leds[NUM_LEDS];
void setupLED(){
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  leds[0] = CRGB ( 10, 0, 0);
  FastLED.show();
  delay(40);
}
void blinkRed(){
      leds[0] = CRGB ( 0, 255, 0);
      FastLED.show();
      delay(400);
      leds[0] = CRGB ( 10, 0, 0);
      FastLED.show();
      delay(40);
}

//--------------------------------- TEST COIN ------------------------------------//
coin_t test_coin;

coin_t savedCoins[8];
void loadCoins(){
  savedCoins[0] = EUR_2;
  savedCoins[1] = EUR_1;
  savedCoins[2] = CENT_50;
  savedCoins[3] = CENT_20;
  savedCoins[4] = CENT_10;
  savedCoins[5] = CENT_5;
  savedCoins[6] = CENT_2;
  savedCoins[7] = CENT_1;
}

float coinProbability[8];
void calculateCoins(){
  uint32_t deltaCoil1, deltaCoil2;
  float deltaSizePot, deltaSizeIR, deltaMass;
  float probCoil1, probCoil2, probSizePot, probSizeIR, probMass;
  for(uint8_t i = 0; i < 8; i++){
    deltaCoil1 = abs(savedCoins[i].coil1 - test_coin.coil1);
    if(deltaCoil1 <= 2) probCoil1 = 100; //68.2% skalirano na 100
    else if(deltaCoil1 <= 4) probCoil1 = 40; //27.2% skalirano za isti faktor
    else if(deltaCoil1 <= 6) probCoil1 = 6;  //4.1% skalirano za isti faktor
    else probCoil1 = 0.3f;

    deltaCoil2 = abs(savedCoins[i].coil2 - test_coin.coil2);
    if(deltaCoil2 <= 2) probCoil2 = 100; //68.2% skalirano na 100
    else if(deltaCoil2 <= 4) probCoil2 = 40; //27.2% skalirano za isti faktor
    else if(deltaCoil2 <= 6) probCoil2 = 6;  //4.1% skalirano za isti faktor
    else probCoil2 = 0.3f;

    deltaSizePot = abs(savedCoins[i].sizePot - test_coin.sizePot);
    if(deltaSizePot <= 0.7f) probSizePot = 100; //68.2% skalirano na 100
    else if(deltaSizePot <= 1.4f) probSizePot = 40; //27.2% skalirano za isti faktor
    else if(deltaSizePot <= 2.1f) probSizePot = 6;  //4.1% skalirano za isti faktor
    else probSizePot = 0.3f;

    deltaSizeIR = abs(savedCoins[i].sizeIR - test_coin.sizeIR);
    if(deltaSizeIR <= 0.7f) probSizeIR = 100; //68.2% skalirano na 100
    else if(deltaSizeIR <= 1.4f) probSizeIR = 40; //27.2% skalirano za isti faktor
    else if(deltaSizeIR <= 2.1f) probSizeIR = 6;  //4.1% skalirano za isti faktor
    else probSizeIR = 0.3f;

    deltaMass = abs(savedCoins[i].mass - test_coin.mass);
    if(deltaMass <= 0.17f) probMass = 100; //68.2% skalirano na 100
    else if(deltaMass <= 0.34f) probMass = 40; //27.2% skalirano za isti faktor
    else if(deltaMass <= 0.51f) probMass = 6;  //4.1% skalirano za isti faktor
    else probMass = 0.3f;

    coinProbability[i] = exp((log(probCoil1)  + log(probSizeIR) + log(probMass)) / 3.0);
    Serial.print("Vjerojatnost da je kovanica ");
    Serial.print(savedCoins[i].name);
    Serial.print(" iznosi ");
    Serial.print(coinProbability[i]);
    Serial.println("%");
  }
}

void printCoin(){
  Serial.println(test_coin.name);
  Serial.print("Prva mjera vodljivosti kovanice = ");
  Serial.println(test_coin.coil1);
//  Serial.print("Druga mjera vodljivosti kovanice = ");
//  Serial.println(test_coin.coil2);
//  Serial.print("Promjer kovanice potenciometrom = ");
//  Serial.println(test_coin.sizePot);
  Serial.print("Promjer kovanice IR diodom = ");
  Serial.println(test_coin.sizeIR);
  Serial.print("Masa kovanice u gramima = ");
  Serial.println(test_coin.mass);
}

//------------------------------- SERVO MOTORI -----------------------------------//
Servo servoSort[8];
Servo servoTop;
void setupServos(){
  delay(100);
  servoSort[0].attach(SERVO_PIN_0);
  delay(100);
  servoSort[1].attach(SERVO_PIN_1);
  delay(100);
  servoSort[2].attach(SERVO_PIN_2);
  delay(100);
  servoSort[3].attach(SERVO_PIN_3);
  delay(100);
  servoSort[4].attach(SERVO_PIN_4);
  delay(100);
  servoSort[5].attach(SERVO_PIN_5);
  delay(100);
  servoSort[6].attach(SERVO_PIN_6);
  delay(100);
  servoSort[7].attach(SERVO_PIN_7);
  delay(100);

  servoTop.attach(SERVO_PIN_TOP);

  delay(100);
  for(uint8_t i = 0; i < 8; i++){
    servoSort[i].write(0);
    delay(100);
  }
//  servoTop.write(0);
  delay(100);
}

//-------------------------------------------------- ZAVOJNICE -------------------------------//
void startCoils(){
  pinMode(COIL_PIN_IN_1, INPUT);
  tone(COIL_PIN_OUT_1, FREQ1);
}

void stopCoils(){
  noTone(COIL_PIN_OUT_1);
}


uint32_t coil1;
uint32_t coil1flag = 0;
uint32_t coil1min = 1023;
void readCoil(){
  coil1 = analogRead(COIL_PIN_IN_1);
  if(coil1 < 1023) {
  //  Serial.println(coil1);
    if(coil1flag == 0) coil1flag = 1;
    if(coil1 < coil1min) coil1min = coil1;
  }else {
    if(coil1flag == 1){
      test_coin.coil1 = coil1min; //sacuvati coil1min kovanice
      coil1min = 1023;
      coil1flag = 0;
    }
  }
}

//-------------------------------------------- IR DIODE ------------------------------------//
void startIR(){
  pinMode(IR_PIN_IN_1, INPUT);
  pinMode(IR_PIN_OUT_1, OUTPUT);
  pinMode(IR_PIN_IN_2, INPUT);
  pinMode(IR_PIN_OUT_2, OUTPUT);
  delay(100);
  digitalWrite(IR_PIN_OUT_1, true);
  digitalWrite(IR_PIN_OUT_2, true);
}

uint32_t IRflag1 = 0, IRflag2 = 0;
unsigned long IRdelta1 = 0, IRdelta2 = 0;
unsigned long IRtime1 = 0;
uint32_t IRcoin1, IRcoin2;
float diametar = 0.0f;
void readIR(){
  IRcoin1 = digitalRead(IR_PIN_IN_1);
  IRcoin2 = digitalRead(IR_PIN_IN_2);
  if(IRcoin1 == 1 && IRflag1 == 0){
    IRflag1 = 1;
    IRtime1 = millis();
    memset(&test_coin, 0, sizeof(test_coin)); //dosla je nova kovanica koja se testira
    test_coin.name = "Test coin";
  }
  if(IRcoin1 == 0 && IRflag1 == 1){
    IRflag1 = 0;
    IRdelta1 = millis() - IRtime1;
  }
  if(IRcoin2 == 1 && IRflag2 == 0){
    IRflag2 = 1;
    IRdelta2 = millis() - IRtime1;
    diametar = (float) IRdistance / IRdelta2 * IRdelta1;
    test_coin.sizeIR = diametar; //sacuvati promjer kovanice
  }
  if(IRcoin2 == 0 && IRflag2 == 1){
    IRflag2 = 0;
    readScale(); //dok je prosla kovanica poceti citati vagu!
  }
}

//----------------------------------- TENZOMETAR ------------------------------------//
HX711 scale;
uint32_t tare = 0;
void setupScale(){
  scale.begin(SCALE_PIN_DOUT, SCALE_PIN_SCK);
  delay(1000);
  tare = scale.read_average(20);
  delay(100);
}
float coinMass;
void readScale(){
  stopCoils();
  delay(1100);
  if(scale.is_ready()) {
    coinMass = (uint32_t) ( (scale.read_average(5) - tare) >> 7);
    coinMass = (float) coinMass * coilConstant; // 7.8 / 47;
  }else {
    coinMass = 0;
    Serial.println("Error, masa nije izmjerena");
  }
  test_coin.mass = coinMass; //sacuvati masu kovanice
  printCoin(); //KRAJ PUTA KOVANICE
  calculateCoins();
  blinkRed();
  startCoils();
  
  //DODATI OTVORITI PRAVA VRATASCA DOLJE
  delay(100);
  servoTop.write(180);
  delay(1000);
  servoTop.write(90);
  delay(200);
  
}
