#include "NRFLite.h"

#define PIN_RADIO_MOMI 8
#define PIN_RADIO_SCK  9

NRFLite _radio;

byte data[31];

typedef struct {
  uint8_t from;
  byte data;
}  * Packet;

Packet rxPacket =  (Packet)data;

void setup() {

  Serial.begin(115200);
  while (!Serial);
  Serial.println("Serial Ready");
  while (!_radio.initTwoPin(155, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {
    Serial.print(".");
  }

  _radio.printDetails();
  Serial.println();
}

unsigned long count = 0;
void loop() {

  if (_radio.hasData()) {
    _radio.readData(&data); // Note how '&' must be placed in front of the variable name.

    Serial.print(rxPacket->from);
    Serial.print(" : ");
    for (byte k = 0; k < 8; k++) {
      Serial.print(rxPacket->data >> k & 1 );
      Serial.print(",");
    }
    //for (byte i = 0; i < 32; i++) {
    //  Serial.print(data[i]); Serial.print(",");
    //}
    Serial.println();
  }
  return;
}
