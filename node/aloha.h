#include "radio.h"

void sleep(int t);
void broadcast();

void mySetup() {
  setupLED();
  setupRadio(COMMON_ID);
  setupSW();

  //broadcast();
}

void myLoop() {
  broadcast();
  delay(1000);
  while (_radio.hasData()) {
    onRed();
   // delay(1000);
    _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.
    offRed();
  }
}

void sleep(int t) {
  offGreen();
  offRed();
  delay(t);
}

void broadcast() {
  onRed();
  _radio.send(COMMON_ID, &_radioData, sizeof(_radioData));
  offRed();
}



