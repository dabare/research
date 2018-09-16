#include "radio.h"

void work();
void sleep();
void broadcast();
void mergeData();
void eraseData();

byte rxData[PACKET_SIZE];
byte txData[PACKET_SIZE];

typedef struct {
  uint8_t from;
  byte data;
}  * Packet;


Packet rxPacket =  (Packet)rxData;
Packet txPacket =  (Packet)txData;

void mySetup() {
  setupLED();
  setupMemory();

  txPacket->from = RADIO_ID;
  eraseData();

  //setupRadio(RADIO_ID);
  setupRadio(INIT_ID);
}

int cycle;

bool ADDorSUB = false;

void myLoop() {
  cycle = WORK_TIME;

  broadcast();
  while (cycle--) {
    work();
  }
  cycle = SLEEP_TIME + ADDorSUB * PAD * (RADIO_ID % PAD_MOD);
  while (cycle--) {
    sleep();
  }
  ADDorSUB = !ADDorSUB;
}

void sleep() {
  _radio.powerDown();
  offRed();
}

void work() {
  onRed();
  if (_radio.hasData()) {
    onGreen();

    _radio.readData(&rxData);

    mergeData();

    offGreen();
  }
}

void broadcast() {
  _radio.send(INIT_ID, &txData, sizeof(txData), NRFLite::REQUIRE_ACK);

  //_radio.send(INIT_ID, &data, sizeof(data), NRFLite::NO_ACK);
  
  /*for (byte i = MIN_ID; i <= MAX_ID; i++) {
    _radio.send(i, &txData, sizeof(txData), NRFLite::REQUIRE_ACK);
  }*/
  eraseData();
}

void mergeData() {
  txPacket->data |= rxPacket->data;
  txPacket->data |= (1 << rxPacket->from);
}

void eraseData() {
  txPacket->data = 1 << RADIO_ID;
}

