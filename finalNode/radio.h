#include "globals.h"
#include "fence.h"
#include "NRFLite.h"
#include <EEPROM.h>
#include "sleep.h"

#define PIN_RADIO_MOMI 1
#define PIN_RADIO_SCK  3

NRFLite _radio;

void setupMemory() {
  while (RADIO_ID == 255) {//eeprom error
    RADIO_ID = EEPROM.read(0);
  }
}

void setupRadio(uint8_t id) {
  while (!_radio.initTwoPin(id, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {//radio error
    reboot();
  }
}

