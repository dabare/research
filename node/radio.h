#include "globals.h"
#include "led.h"
#include "switch.h"
#include "NRFLite.h"
#include <EEPROM.h>
#include "sleep.h"

#define PIN_RADIO_MOMI 1
#define PIN_RADIO_SCK  3

const static uint8_t DESTINATION_RADIO_ID = 0; // Id of the radio we will transmit to.

byte data[6] = {1, 1, 0, 0, 0};

struct RadioPacket // Any packet up to 32 bytes can be sent.
{
  uint8_t FromRadioId;
  uint32_t OnTimeMillis;
  uint32_t FailedTxCount;
};

NRFLite _radio;
RadioPacket _radioData;

void setupRadio() {
#ifdef LED_ON
  digitalWrite(RED_LED , HIGH);
#endif

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

  while (!_radio.initTwoPin(RADIO_ID, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {//radio error
#ifdef LED_ON
    digitalWrite(RED_LED , HIGH);
    delay(20);
    digitalWrite(RED_LED , LOW);
    delay(100);
#endif
  }
  //_radio.powerDown();
#ifdef LED_ON
  digitalWrite(RED_LED , LOW);
#endif
}

void setupRadio(uint8_t id) {
#ifdef LED_ON
  digitalWrite(RED_LED , HIGH);
#endif

  while (!_radio.initTwoPin(id, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {//radio error
#ifdef LED_ON
    digitalWrite(RED_LED , HIGH);
    delay(20);
    digitalWrite(RED_LED , LOW);
    delay(100);
#endif
  }

#ifdef LED_ON
  digitalWrite(RED_LED , LOW);
#endif
}

