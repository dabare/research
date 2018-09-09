#include "radio.h"

void RX();
void TX(byte to);

void mySetup() {
  setupLED();
  setupRadio();
}

void myLoop() {
  switch (RADIO_ID) {
    case 4:
      TX(5);
      break;
    case 5:
      RX();
      break;
    case 6:
      TX(7);
      break;
    case 7:
      RX();
      break;
  }
}

void RX() {
  if (_radio.hasData()) {
    onRed();
    _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name
    offRed();
  }
}

void TX(byte to) {
  onGreen();
  _radio.send(to, &_radioData, sizeof(_radioData));
  offGreen();
}

