#include "globals.h"

#include "NRFLite.h"

#define PIN_RADIO_MOMI 4
#define PIN_RADIO_SCK  3

uint8_t RADIO_ID = 2;           
struct RadioPacket // Any packet up to 32 bytes can be sent.
{
  uint8_t FromRadioId;
  uint32_t OnTimeMillis;
  uint32_t FailedTxCount;
};

NRFLite _radio;
RadioPacket _radioData;

void setupRadio() {
  while (!_radio.initTwoPin(RADIO_ID, PIN_RADIO_MOMI, PIN_RADIO_SCK));
}

