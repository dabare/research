#include "radio.h"

void work();
void idle();
void sleep();
void client();
void server();
void passToNext();
void initSignal();

void mySetup() {
  setupSleep();
  setupLED();
  setupRadio();
  setupSW();
  resetIdleBlink();

  passToNext();
}

void myLoop() {
  for (int i = 0; i < 255; i++) {
    work();
    idle();
  }
 // sleep();
}

void idle() {
#ifdef LED_ON
  idleBlink();
#endif
}

void work() {
  client();
}

void sleep() {
  _radio.powerDown();
  watchdogSleep1s();
}

void client() {
  while (_radio.hasData()) {
    resetIdleBlink();
    onGreen();
    _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.
    delay(1000);
    passToNext();
  }
}

void server() {
  passToNext();
}

void passToNext() {
  onGreen();
  while (RADIO_ID != MAX_ID && _radio.send(RADIO_ID + 1, &_radioData, sizeof(_radioData)) != 1);
  offGreen();
}

void initSignal() {
  onGreen();
  delay(1000);
  _radio.send(RADIO_ID + 1, &_radioData, sizeof(_radioData));
  offGreen();
}

