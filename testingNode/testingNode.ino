#include "NRFLite.h"


#define PACKET_SIZE 32

#define PIN_RADIO_MOMI 8
#define PIN_RADIO_SCK  9

NRFLite _radio;

byte data[32];


typedef struct {
  byte from;
  byte data[PACKET_SIZE - 2];// two bytes for 'from' and 'hash'
  byte hash;
}  * Packet;

Packet rxPacket =  (Packet)data;

void setup() {

  Serial.begin(115200);
  while (!Serial);
  Serial.println("Serial Ready");
  while (!_radio.initTwoPin(0, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {
    Serial.print(".");
  }

  _radio.printDetails();
  Serial.println();
  byte x = 0b11111111;
  x >>= 4;
  x <<= 4;

  for (byte k = 0; k < 8; k++) {
    Serial.print(x >> (7 - k) & 1 );
    Serial.print(",");
  }
  Serial.println();
}


bool checkRxHash() {
  if (rxPacket->hash == rxPacket->from ^ rxPacket->data[0]) {
    return true;
  }
  return false;
}

unsigned long count = 0;
void loop() {

  if (_radio.hasData()) {
    _radio.readData(&data); // Note how '&' must be placed in front of the variable name.
    if (!checkRxHash()) {
      Serial.print("Droped message from:");
      Serial.print(rxPacket->from);
      Serial.println(" due to invalied hash");
         return;
    }
    Serial.print(rxPacket->from);
    Serial.print(" : ");
    for (byte k = 0; k < 8; k++) {
      Serial.print(rxPacket->data[0] >> (7 - k) & 1 );
      Serial.print(",");
    }
    //for (byte i = 0; i < 32; i++) {
    //  Serial.print(data[i]); Serial.print(",");
    //}

    Serial.print("  hash: ");
    Serial.print(rxPacket->hash);
    Serial.println();
  }
  return;
}
