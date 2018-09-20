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
void setMessage();
void deleteDeadMsgs();

byte rxData[PACKET_SIZE];
byte txData[PACKET_SIZE];

struct message {
  byte from;
  byte to;
  byte msg;
};

typedef struct {
  byte from;
  struct message msg;
  byte data[PACKET_SIZE - 2 - sizeof(message)];// two bytes for 'from' and 'hash'
  byte hash;
}  * Packet;

byte neighbourCount = 0;
byte neighbours[MAX_NEIGHBOURS];
byte neighboursRecord[MAX_NEIGHBOURS];

byte msgCount = 0;
byte msgToSend = 0;
struct message msgs[MAX_MESSAGES];
byte msgLife[MAX_MESSAGES];

Packet rxPacket =  (Packet)rxData;
Packet txPacket =  (Packet)txData;

bool txFailCount = 0;

void mySetup() {
  setupLED();
  setupMemory();

  txPacket->from = RADIO_ID;
  eraseData();

  //setupRadio(RADIO_ID);
  setupRadio(INIT_ID);

  //default message
  msgCount = 4;
  msgToSend = 0;
  msgs[0] = {0, 1, 2};
  msgs[1] = {1, 2, 4};
  msgs[2] = {2, 3, 8};
  msgs[3] = {3, 4, 16};
  msgLife[0] = MESSAGE_LIFE;
  msgLife[1] = 1;
  msgLife[2] = 2;
  msgLife[3] = 3;

}

int cycle;

bool ADDorSUB = false;

void myLoop() {
  wake();

  cycle = WORK_TIME;

  broadcast();
  reduceNeighbourLife();
  deleteDeadMsgs();

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
  setupRadio(INIT_ID);
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
  setMessage();
  calculateTxHash();
  if (_radio.send(INIT_ID, &txData, sizeof(txData), NRFLite::REQUIRE_ACK) != 1) {
    txFailCount++;
  } else {
    txFailCount = 0;
    eraseData();
  }

  if (txFailCount == MAX_TX_RETRY) {
    txFailCount = 0;
    eraseData();
  }
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

void setMessage() {
  txPacket->msg.from = msgs[msgToSend].from;
  txPacket->msg.to = msgs[msgToSend].to;
  txPacket->msg.msg = msgs[msgToSend].msg;
  msgLife[msgToSend]--;
  msgToSend++;
  msgToSend %= msgCount;
}

void deleteDeadMsgs() {
  byte tmp = 0;
  while (tmp < msgCount) {
    if (!msgLife[tmp]) {
      byte tmp2 = tmp;
      while (tmp2 < msgCount - 1) {
        msgs[tmp2].from = msgs[tmp2 + 1].from;
        msgs[tmp2].to = msgs[tmp2 + 1].to;
        msgs[tmp2].msg = msgs[tmp2 + 1].msg;
        msgLife[tmp2] = msgLife[tmp2 + 1];

        tmp2++;
      }
      msgCount--;
      msgToSend %= msgCount;
    }
    tmp++;
  }
  msgLife[0] = MESSAGE_LIFE;
}

