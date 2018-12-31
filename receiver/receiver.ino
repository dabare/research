#include "NRFLite.h"
#include "Keyboard.h"

#define PACKET_SIZE 32

#define PIN_RADIO_MOMI 8
#define PIN_RADIO_SCK  9
#define LISTEN 1
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
  //while (!Serial);
  Serial.println("Serial  Ready");
  while (!_radio.initTwoPin(255, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {
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
  Keyboard.begin();
  delay(1000);
  //Keyboard.println("Ready");
}


bool checkRxHash() {
  if (rxPacket->hash == rxPacket->from ^ rxPacket->data[0]) {
    return true;
  }
  return false;
}

unsigned long count = 0;
void loop() {
  //countPackets();
  if (_radio.hasData()) {
    _radio.readData(&data); // Note how '&' must be placed in front of the variable name.
    if(rxPacket->from != 4) return;
    if (!checkRxHash()) {
      Serial.print("Droped message from:");
      Serial.print(rxPacket->from);
      Serial.println(" due to invalied hash");

      //Keyboard.print("Droped message from:");
      //Keyboard.print(rxPacket->from);
      //Keyboard.println(" due to invalied hash");
      return;
    }
    Serial.print(rxPacket->from);
    
    if (rxPacket->from == LISTEN) {
      Keyboard.print(rxPacket->from);
      Keyboard.print(" : ");
    }
    Serial.print(" : ");
    for (byte k = 0; k < 8; k++) {
      Serial.print(rxPacket->data[0] >> k & 1 );
      Serial.print(",");
      if (rxPacket->from == LISTEN) {
        Keyboard.print(rxPacket->data[0] >> k & 1 );
        Keyboard.print(",");
      }
    }
    //for (byte i = 0; i < 32; i++) {
    //  Serial.print(data[i]); Serial.print(",");
    //}
    if (rxPacket->from == LISTEN) {
      Keyboard.println();
    }
    Serial.print("  hash: ");
    Serial.print(rxPacket->hash);
    Serial.println();
  }
  return;
}
int countt = 0;


void countPackets() {
  while (!_radio.hasData());
  _radio.readData(&data);
  Keyboard.println(countt);
  while (countt < 100) {
    if (_radio.hasData()) {
      _radio.readData(&data);
      countt++;
    }
  }
  Keyboard.println(0
  );
  countt = 0;
  Keyboard.println(countt);
  while (countt < 10000) {
    if (_radio.hasData()) {
      _radio.readData(&data);
      countt++;
    }
  }
  Keyboard.println(countt);
  while(1);
}

