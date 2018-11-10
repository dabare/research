#include "radio.h"

void work();
void sleep();
void wake();
void broadcast();
void mergeData();
void eraseData();
void addNeighbour();
void reduceNeighbourLife();
void addMyData();
void calculateTxHash();
bool checkRxHash();

byte rxData[PACKET_SIZE];
byte txData[PACKET_SIZE];

typedef struct {
  byte from;
  byte data[PACKET_SIZE - 2];// two bytes for 'from' and 'hash'
  byte hash;
}  * Packet;

byte neighbourCount = 0;
byte neighbours[MAX_NEIGHBOURS];
byte neighboursRecord[MAX_NEIGHBOURS];


Packet rxPacket =  (Packet)rxData;
Packet txPacket =  (Packet)txData;


void mySetup() {
  setupLED();
  setupMemory();

  txPacket->from = RADIO_ID;
  eraseData();

  setupRadio(RADIO_ID);
  //setupRadio(INIT_ID);
}

int cycle;

bool ADDorSUB = false;

void myLoop() {
  wake();

  cycle = WORK_TIME;

  broadcast();
  reduceNeighbourLife();

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

void wake() {
  setupRadio(RADIO_ID);
  _radio.hasData();
}

void work() {
  onRed();
  if (_radio.hasData()) {
    onGreen();

    _radio.readData(&rxData);

    if (checkRxHash()) {
      mergeData();
      addNeighbour();
    }

    offGreen();
  }
}

void broadcast() {
  addMyData();
  calculateTxHash();
  bool pktSent = 
  _radio.send(INIT_ID, &txData, sizeof(txData), NRFLite::REQUIRE_ACK) == 1 |
  _radio.send(RADIO_ID - 1, &txData, sizeof(txData), NRFLite::REQUIRE_ACK) == 1 |
  _radio.send(RADIO_ID + 1, &txData, sizeof(txData), NRFLite::REQUIRE_ACK) == 1 |
  _radio.send(RADIO_ID - 2, &txData, sizeof(txData), NRFLite::REQUIRE_ACK) == 1 |
  _radio.send(RADIO_ID + 2, &txData, sizeof(txData), NRFLite::REQUIRE_ACK) == 1
  ;

 /* bool pktSent =
    _radio.send(INIT_ID, &txData, sizeof(txData), NRFLite::REQUIRE_ACK) == 1 ||
    _radio.send(5, &txData, sizeof(txData), NRFLite::REQUIRE_ACK) == 1 
    ;*/
}

void addMyData() {
  txPacket->data[0] |= 1 << RADIO_ID;//add my id

  byte i = neighbourCount;//add my neighbours
  while (i) {
    if (neighboursRecord[i - 1]) {
      txPacket->data[0] |= 1 << neighbours[i - 1];
    } else {
      txPacket->data[0] &= (1 << neighbours[i - 1]) ^ 0b11111111;
    }
    i--;
  }
}

void mergeData() {
  if(rxPacket->from > RADIO_ID){
    rxPacket->data[0] >>= rxPacket->from;
    rxPacket->data[0] <<= rxPacket->from;
    
    txPacket->data[0] <<= 8 - rxPacket->from;
    txPacket->data[0] >>= 8 - rxPacket->from;
  }else{
    rxPacket->data[0] <<= 7 - rxPacket->from;
    rxPacket->data[0] >>= 7 - rxPacket->from;

    txPacket->data[0] >>= 1 + rxPacket->from;
    txPacket->data[0] <<= 1 + rxPacket->from;
  }
  txPacket->data[0] |= rxPacket->data[0];
}

void eraseData() {
  txPacket->data[0] = 0;
}

void addNeighbour() {
  byte i = neighbourCount;
  while (i) {
    if (neighbours[i - 1] == rxPacket->from) {
      neighboursRecord[i - 1] = NEIGHBOUR_LIFE;
      return;
    }
    i--;
  }
  if (neighbourCount < MAX_NEIGHBOURS) {
    neighbours[neighbourCount] = rxPacket->from;
    neighboursRecord[neighbourCount++] = NEIGHBOUR_LIFE;
  }
}

void reduceNeighbourLife() {
  byte i = neighbourCount;
  while (i) {
    if (neighboursRecord[i - 1]) {
      neighboursRecord[i - 1]--;
    }
    i--;
  }
}

void calculateTxHash() {
  txPacket->hash = txPacket->from ^ txPacket->data[0];
}

bool checkRxHash() {
  if (rxPacket->hash == rxPacket->from ^ rxPacket->data[0]) {
    return true;
  }
  return false;
}

