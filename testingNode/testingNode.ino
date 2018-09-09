#include "NRFLite.h"

#define PIN_RADIO_MOMI 8
#define PIN_RADIO_SCK  9

NRFLite _radio;

byte data[32];


void setup() {

  Serial.begin(115200);
  while (!Serial);
  while (!_radio.initTwoPin(155, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {
    Serial.print(".");
  }
  _radio.printDetails();
}

void loop() {
  if (_radio.hasData()) {
    _radio.readData(&data); // Note how '&' must be placed in front of the variable name.
    for (byte i = 0; i < 32; i++) {
      Serial.print(data[i]);Serial.print(",");
    }
    Serial.println();
  }
}
