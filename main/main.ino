#include "myLib.h"


void setup() {
  setupServos();
  startIR();
  setupScale();
  setupLED();
  startCoils();
  loadCoins();

  delay(1000);
  Serial.begin(9600);
  Serial.println("Pocetak rada");
}

void loop() {
  //delay(10);
  readIR();
  readCoil();
}
