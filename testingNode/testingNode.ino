#include "NRFLite.h"


#define PACKET_SIZE 32

#define PIN_RADIO_MOMI 8
#define PIN_RADIO_SCK  9

NRFLite _radio;

byte data[32];

struct message {
  byte from;
  byte to;
  byte msg;
};

typedef struct {
  byte from;
  struct message msg;
  byte data[PACKET_SIZE - 2 - sizeof(message)];
  byte hash;
}  * Packet;

Packet rxPacket =  (Packet)data;

void setup() {

  Serial.begin(115200);
  while (!Serial);
  Serial.println("Serial Ready");
  while (!_radio.initTwoPin(255, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {
    Serial.print(".");
  }

  _radio.printDetails();
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
    if(!checkRxHash()){
      return;
    }
    Serial.print(rxPacket->from);
    Serial.print(" : ");
    for (byte k = 0; k < 8; k++) {
      Serial.print(rxPacket->data[0] >> k & 1 );
      Serial.print(",");
    }
    //for (byte i = 0; i < 32; i++) {
    //  Serial.print(data[i]); Serial.print(",");
    //}
    Serial.print(" msgFrom: ");
    Serial.print(rxPacket->msg.from);
    Serial.print(" msgTo: ");
    Serial.print(rxPacket->msg.to);
    Serial.print(" msg: ");
    for (byte k = 0; k < 8; k++) {
      Serial.print(rxPacket->msg.msg >> k & 1 );
      Serial.print(",");
    }
    Serial.print("  hash: ");
    Serial.print(rxPacket->hash);
    Serial.println();
  }
  return;
}
