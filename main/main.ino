#include "myLib.h"


void setup() {
  setupLED();
  setupLCD();
  setupBuzzer();
  setupButtons();
  setupPot();
  setupServos();
  startIR();
  setupScale();
  startCoils();
  loadCoins();

  delay(1000);

  resetLED();
  Serial.begin(9600);
  Serial.println("Pocetak rada");
}

void loop() {
  readButtons();
  readPot();
}
