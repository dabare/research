#include "globals.h"
#include "led.h"
#include "switch.h"
#include "NRFLite.h"
#include <EEPROM.h>
#include "sleep.h"

#define PIN_RADIO_MOMI 1
#define PIN_RADIO_SCK  3

NRFLite _radio;

void setupMemory() {
  while (RADIO_ID == 255) {//eeprom error
    //EEPROM.write(0, 3);
#ifdef LED_ON
    digitalWrite(GREEN_LED , HIGH);
    delay(20);
    digitalWrite(GREEN_LED , LOW);
    delay(100);
#endif
    RADIO_ID = EEPROM.read(0);
  }
}

void setupRadio(uint8_t id) {
  while (!_radio.initTwoPin(id, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {//radio error
#ifdef LED_ON
    digitalWrite(RED_LED , HIGH);
    delay(20);
    digitalWrite(RED_LED , LOW);
    delay(100);
#endif
  }
}
