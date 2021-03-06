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


void setup() {
  setupMemory();

  txPacket->from = RADIO_ID;
  eraseData();

  setupRadio(RADIO_ID);

  setupNeon();
  setupSleep();
}

int cycle;

bool LONG_DS = false;//long dutycycle

void loop() {
  wake();

  cycle = WORK_TIME + LONG_DS * PAD * (RADIO_ID % (ZONE_RADIUS + 1));

  broadcast();
  reduceNeighbourLife();

  while (cycle--) {
    work();
  }

  sleep();//sleeps for one second

  LONG_DS = !LONG_DS;
}

void sleep() {
  _radio.powerDown();
  watchdogSleep1s();
}

void wake() {
  setupRadio(RADIO_ID);
  _radio.hasData();
}

void work() {
  checkNeon();
  if (_radio.hasData()) {
    _radio.readData(&rxData);

    if (checkRxHash()) {
      mergeData();
      addNeighbour();
    }
  }
}

byte tmpBroadcast = 0;
void broadcast() {
  tmpBroadcast = ZONE_RADIUS;
  addMyData();
  calculateTxHash();

  _radio.send(BCAST_ID, &txData, sizeof(txData), NRFLite::REQUIRE_ACK);
  while (tmpBroadcast) {
    _radio.send(RADIO_ID - tmpBroadcast, &txData, sizeof(txData), NRFLite::REQUIRE_ACK);
    _radio.send(RADIO_ID + tmpBroadcast, &txData, sizeof(txData), NRFLite::REQUIRE_ACK);
    tmpBroadcast--;
  }
}

void addMyData() {
  txPacket->data[0] |= 1 << RADIO_ID;//add my id, make the bit 1
  if (!isFenceOK()) {
    txPacket->data[0] ^= 1 << RADIO_ID;//if fence not working make the bit to 0
  }
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
  if (rxPacket->from > RADIO_ID) {
    if (rxPacket->from > ZONE_RADIUS) {
      rxPacket->data[0] >>= rxPacket->from - ZONE_RADIUS;
      rxPacket->data[0] <<= rxPacket->from - ZONE_RADIUS;

      txPacket->data[0] <<= 8 - (rxPacket->from - ZONE_RADIUS);
      txPacket->data[0] >>= 8 - (rxPacket->from - ZONE_RADIUS);
    } else {
      rxPacket->data[0] >>= rxPacket->from;
      rxPacket->data[0] <<= rxPacket->from;

      txPacket->data[0] <<= 8 - rxPacket->from;
      txPacket->data[0] >>= 8 - rxPacket->from;
    }
  } else {
    if (rxPacket->from < 7 - ZONE_RADIUS) {
      rxPacket->data[0] <<= 7 - rxPacket->from - ZONE_RADIUS;
      rxPacket->data[0] >>= 7 - rxPacket->from - ZONE_RADIUS;

      txPacket->data[0] >>= rxPacket->from + ZONE_RADIUS;
      txPacket->data[0] <<= rxPacket->from + ZONE_RADIUS;
    } else {
      rxPacket->data[0] <<= 7 - rxPacket->from;
      rxPacket->data[0] >>= 7 - rxPacket->from;

      txPacket->data[0] >>= rxPacket->from;
      txPacket->data[0] <<= rxPacket->from;
    }
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
      if ((rxPacket->data[0] & (1 << rxPacket->from)) > 0) { //checks weater the fence working by checking its corresponding bit == 1
        neighboursRecord[i - 1] = NEIGHBOUR_LIFE;
      } else {
        neighboursRecord[i - 1] = 0;
      }
      return;
    }
    i--;
  }
  if (neighbourCount < MAX_NEIGHBOURS) {
    neighbours[neighbourCount] = rxPacket->from;
    if ((rxPacket->data[0] & (1 << rxPacket->from)) > 0) { //checks weater the fence working by checking its corresponding bit == 1
      neighboursRecord[neighbourCount++] = NEIGHBOUR_LIFE;
    } else {
      neighboursRecord[neighbourCount++] = 0;
    }
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

