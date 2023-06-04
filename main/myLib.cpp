#include "Arduino.h"
#include "myLib.h"
#include <Servo.h>
#include "HX711.h"
#include <FastLED.h>
#include <LiquidCrystal.h>

//varijable bitne cijelom programu
coin_t test_coin;
uint8_t calibrationNum = CALNUM;
void(* resetFunc) (void) = 0;
float ukupno;
bool kune = false;

//-------------------------------- LCD ------------------------------------------//

LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_E, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7);

void setupLCD(){
  lcd.begin(16, 2);
  lcd.print("STARTING");
  lcd.setCursor(0, 1);
  lcd.print("COINTROLLER");
}

void showUkupno(){
  
  if(!kune){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ukupno kuna ");
      lcd.setCursor(0, 1);
      lcd.print(ukupno * 7.5345);
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ukupno eura");
      lcd.setCursor(0, 1);
      lcd.print(ukupno);
    }

}


//-------------------------------- BUTTONS ---------------------------------------//

void setupButtons(){
  pinMode(BUTTON_PIN_1, INPUT);
  pinMode(BUTTON_PIN_2, INPUT);
  pinMode(BUTTON_PIN_3, INPUT);
  pinMode(BUTTON_PIN_4, INPUT);
}

uint8_t but1;
uint8_t but2;
uint8_t but3;
uint8_t but4;

void readButtons(){
  but1 = digitalRead(BUTTON_PIN_1);
  but2 = digitalRead(BUTTON_PIN_2);
  but3 = digitalRead(BUTTON_PIN_3);
  but4 = digitalRead(BUTTON_PIN_4);
  if(but4 == 0) {
    resetFunc();
  }
  if(but3 == 0){
    calibrationNum = 0;
    resetLED();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("KALIBRACIJA");
    lcd.setCursor(0, 1);
    lcd.print("Ubaci ");
    lcd.print(CALNUM);
    lcd.print("x isti");
    delay(100);
  }
  if(but2 == 0){
    ukupno = 0;
    showUkupno();
    delay(100);
  }
  if(but1 == 0){
    kune = !kune;
    showUkupno();
    delay(500);
  }
}


//----------------------------------- POTENCIOMETAR -------------------------------//

void setupPot(){
  pinMode(POT_PIN, INPUT);
}

uint32_t pot = 1023;
uint32_t potMin;
void readPotMin(){
  pot = analogRead(POT_PIN);
  if(pot < potMin){
    potMin = pot;
    test_coin.sizePot = potMin;
  }
}


bool prosla;
void readPot(){
  pot = analogRead(POT_PIN);
//  Serial.println(pot);
  if (pot < 830){ // AKO JE ZAPOCETO MJERENJE
    prosla = false;
    ledColorRGB(255, 165, 0); //narancasto
    memset(&test_coin, 0, sizeof(test_coin)); //dosla je nova kovanica koja se testira
    test_coin.name = "Test coin";
    potMin = 1024;
    while(!prosla){
      readPotMin();
      readCoil();
      readIR();
    }
  }
}

//-------------------------------- RGB LED ---------------------------------------//
CRGB leds[NUM_LEDS];

void setupLED(){
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  ledColorRGB(255, 0, 0);
}

void resetLED(){
  if(calibrationNum < CALNUM){
    ledColorRGB(0, 0, 255);
  }
  else{
    ledColorRGB(0, 10, 0);
  }
}

void ledColorRGB(uint8_t r, uint8_t g, uint8_t b){
  leds[0] = CRGB ( g, r, b);
  FastLED.show();
  delay(1);
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
  servoTop.write(180);
  delay(500);
  servoTop.write(90);
  delay(100);
}
//--------------------------------- BUZZER --------------------------------------//

void setupBuzzer(){
  pinMode(BUZZER_PIN, OUTPUT);
}

//--------------------------------- TEST COIN ------------------------------------//


coin_t savedCoins[8];
void loadCoins(){
  savedCoins[0] = COIN1;
  savedCoins[1] = COIN2;
  savedCoins[2] = COIN3;
  savedCoins[3] = COIN4;
  savedCoins[4] = COIN5;
  savedCoins[5] = COIN6;
  savedCoins[6] = COIN7;
  savedCoins[7] = COIN8;
  
}

float coinProbability[8];
void calculateCoins(){
  uint32_t deltaCoil1, deltaCoil2;
  float deltaSizePot, deltaSizeIR, deltaMass;
  float probCoil1, probCoil2, probSizePot, probSizeIR, probMass;
  for(uint8_t i = 0; i < 8; i++){
    deltaCoil1 = abs((float) savedCoins[i].coil1 - (float) test_coin.coil1);
    if(deltaCoil1 <= 3) probCoil1 = 100; //68.2% skalirano na 100
    else if(deltaCoil1 <= 6) probCoil1 = 40; //27.2% skalirano za isti faktor
    else if(deltaCoil1 <= 9) probCoil1 = 6;  //4.1% skalirano za isti faktor
    else probCoil1 = 0.3f;
    deltaCoil2 = abs((float) savedCoins[i].coil2 - (float) test_coin.coil2);
    if(deltaCoil2 <= 3) probCoil2 = 100; //68.2% skalirano na 100
    else if(deltaCoil2 <= 6) probCoil2 = 40; //27.2% skalirano za isti faktor
    else if(deltaCoil2 <= 9) probCoil2 = 6;  //4.1% skalirano za isti faktor
    else probCoil2 = 0.3f;
    deltaSizePot = abs((float) savedCoins[i].sizePot - (float) test_coin.sizePot);
    if(deltaSizePot <= 4) probSizePot = 100; //68.2% skalirano na 100
    else if(deltaSizePot <= 8) probSizePot = 40; //27.2% skalirano za isti faktor
    else if(deltaSizePot <= 12) probSizePot = 6;  //4.1% skalirano za isti faktor
    else probSizePot = 0.3f;
    deltaSizeIR = abs(savedCoins[i].sizeIR - test_coin.sizeIR);
    if(deltaSizeIR <= 0.9f) probSizeIR = 100; //68.2% skalirano na 100
    else if(deltaSizeIR <= 1.8f) probSizeIR = 40; //27.2% skalirano za isti faktor
    else if(deltaSizeIR <= 2.7f) probSizeIR = 6;  //4.1% skalirano za isti faktor
    else probSizeIR = 0.3f;
    deltaMass = abs(savedCoins[i].mass - test_coin.mass);
    if(deltaMass <= 0.17f) probMass = 100; //68.2% skalirano na 100
    else if(deltaMass <= 0.34f) probMass = 40; //27.2% skalirano za isti faktor
    else if(deltaMass <= 0.51f) probMass = 6;  //4.1% skalirano za isti faktor
    else probMass = 0.3f;

    coinProbability[i] = exp((log(probCoil1) + log(probCoil2)  + log(probSizePot) + log(probSizeIR) + log(probMass)) / 5.0);
    //coinProbability[i] = (probCoil1 + probCoil2 + probSizePot + probSizeIR + probMass) / 5.0;
/*    Serial.println(probCoil1);
    Serial.println(probCoil2);
    Serial.println(probSizePot);
    Serial.println(probSizeIR);
    Serial.println(probMass);*/
    Serial.print("Vjerojatnost da je kovanica ");
    Serial.print(savedCoins[i].name);
    Serial.print(" iznosi ");
    Serial.print(coinProbability[i]);
    Serial.println("%");
  }
  Serial.println("+------------------------------------------+");
  delay(100);
  uint8_t count = 0;
  for(int i = 0; i < 8; i++){
    if(coinProbability[i] > THRESHOLD) count++;
  }
  if (count != 1) { // NIJE PREPOZNATA KOVANICA
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LAZNA KOVANICA");
    ledColorRGB(255, 0, 0);
    tone(BUZZER_PIN, 1000);
    delay(100);
    ledColorRGB(0, 0, 0);
    delay(100);
    ledColorRGB(255, 0, 0);
    delay(100);
    ledColorRGB(0, 0, 0);
    noTone(BUZZER_PIN);
    delay(100);
    ledColorRGB(255, 0, 0);
    servoSort[7].write(90);
    delay(100);
    servoTop.write(180);
    delay(1000);
    servoTop.write(90);
    delay(1000);
    servoSort[7].write(0);
    
  } else{
    ledColorRGB(0, 255, 100);
    delay(100);
    ledColorRGB(0, 0, 0);
    delay(100);
    ledColorRGB(0, 255, 100);
    delay(100);
    ledColorRGB(0, 0, 0);
    delay(100);
    ledColorRGB(0, 255, 100);
    for(int i = 1; i <= 7; i++){
      if(coinProbability[i] > THRESHOLD) {
        ukupno += savedCoins[i].value;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(savedCoins[i].name);
        lcd.setCursor(0, 1);
        lcd.print(coinProbability[i]);
        lcd.print("%");
        servoSort[i-1].write(90);
        delay(100);
        servoTop.write(180);
        delay(1000);
        servoTop.write(90);
        delay(1000);
        servoSort[i-1].write(0);
      }
    }
    if(coinProbability[0] > THRESHOLD){
      ukupno += savedCoins[0].value;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(savedCoins[0].name);
      lcd.setCursor(0, 1);
      lcd.print(coinProbability[0]);
      lcd.print("%");
      servoTop.write(180);
      delay(1000);
      servoTop.write(90);
      delay(1000);
    }
  }
}

coin_t calibrationCoin[CALNUM];

void printCoin(){
  Serial.println("+----------------------------+");
  Serial.println(test_coin.name);
  Serial.print("Prva mjera vodljivosti kovanice = ");
  Serial.println(test_coin.coil1);
  Serial.print("Druga mjera vodljivosti kovanice = ");
  Serial.println(test_coin.coil2);
  Serial.print("Promjer kovanice potenciometrom = ");
  Serial.println(test_coin.sizePot);
  Serial.print("Promjer kovanice IR diodom = ");
  Serial.println(test_coin.sizeIR);
  Serial.print("Masa kovanice u gramima = ");
  Serial.println(test_coin.mass);
  Serial.println("+----------------------------+");
  if(calibrationNum < CALNUM) calibrateCoin();
}

void calibrateCoin(){
  calibrationCoin[calibrationNum] = test_coin;
  calibrationNum++;
  if(calibrationNum == CALNUM){
    float coil1sum = 0;
    float coil2sum = 0;
    float sizeIRsum = 0;
    float sizePotsum = 0;
    float masssum = 0;
    for(int i = 0; i < CALNUM; i++){
      coil1sum += (float) calibrationCoin[i].coil1;
      coil2sum += (float) calibrationCoin[i].coil2;
      sizeIRsum += calibrationCoin[i].sizeIR;
      sizePotsum += calibrationCoin[i].sizePot;
      masssum += calibrationCoin[i].mass;
    }
    float coil1avg = coil1sum/CALNUM;
    float coil2avg = coil2sum/CALNUM;
    float sizeIRavg = sizeIRsum/CALNUM;
    float sizePotavg = sizePotsum/CALNUM;
    float massavg = masssum/CALNUM;
    float coil1sq = 0;
    float coil2sq = 0;
    float sizeIRsq = 0;
    float sizePotsq = 0;
    float masssq = 0;
    for(int i = 0; i < CALNUM; i++){
      coil1sq += pow((float) calibrationCoin[i].coil1 - coil1avg, 2.0);
      coil2sq += pow((float) calibrationCoin[i].coil2 - coil2avg, 2.0);
      sizeIRsq += pow((float) calibrationCoin[i].sizeIR - sizeIRavg, 2.0);
      sizePotsq += pow((float) calibrationCoin[i].sizePot - sizePotavg, 2.0);
      masssq += pow((float) calibrationCoin[i].mass - massavg, 2.0);
    }
    float coil1sig = sqrt(coil1sq / (float) CALNUM);
    float coil2sig = sqrt(coil2sq / (float) CALNUM);
    float sizeIRsig = sqrt(sizeIRsq / (float) CALNUM);
    float sizePotsig = sqrt(sizePotsq / (float) CALNUM);
    float masssig = sqrt(masssq / (float) CALNUM);
    Serial.println("+---------- REZULTAT USREDNJAVANJA ----------+");
    Serial.print("coil1 = ");
    Serial.print(coil1avg);
    Serial.print("   s = ");
    Serial.println(coil1sig);

    Serial.print("coil2 = ");
    Serial.print(coil2avg);
    Serial.print("   s = ");
    Serial.println(coil2sig);
    
    Serial.print("sizeIR = ");
    Serial.print(sizeIRavg);
    Serial.print("   s = ");
    Serial.println(sizeIRsig);

    Serial.print("sizePot = ");
    Serial.print(sizePotavg);
    Serial.print("   s = ");
    Serial.println(sizePotsig);

    Serial.print("mass = ");
    Serial.print(massavg);
    Serial.print("   s = ");
    Serial.println(masssig);

    Serial.println("+--------------------------------------------+");
  }
}




//-------------------------------------------------- ZAVOJNICE -------------------------------//
void startCoils(){
  pinMode(COIL_PIN_IN_1, INPUT);
  pinMode(COIL_PIN_IN_2, INPUT);
  tone(COIL_PIN_OUT_1, FREQ1);
}

void stopCoils(){
  noTone(COIL_PIN_OUT_1);
}


uint32_t coil1, coil2;
uint32_t coil1flag = 0, coil2flag;
uint32_t coil1min = 1023, coil2min = 1023;
void readCoil(){
  coil1 = analogRead(COIL_PIN_IN_1);
  coil2 = analogRead(COIL_PIN_IN_2);
//  Serial.println(coil1);
  if(coil1 < 1023) {
    
    if(coil1flag == 0) coil1flag = 1;
    if(coil1 < coil1min) coil1min = coil1;
  }else {
    if(coil1flag == 1){
      test_coin.coil1 = coil1min; //sacuvati coil1min kovanice
      coil1min = 1023;
      coil1flag = 0;
    }
  }
  if(coil2 < 1023) {
    //Serial.println(coil2);
    if(coil2flag == 0) coil2flag = 1;
    if(coil2 < coil2min) coil2min = coil2;
  }else {
    if(coil2flag == 1){
      test_coin.coil2 = coil2min; //sacuvati coil2min kovanice
      coil2min = 1023;
      coil2flag = 0;
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


  //KRAJ PUTA KOVANICE
  processCoin();
  
}

//------------------------------------ PROCESS COIN -----------------------//

void processCoin(){
  prosla = true;
  printCoin();
  calculateCoins();
  startCoils();
  delay(1000);
  resetLED();
  showUkupno();
  
}