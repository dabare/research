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
void setMessageToPkt();
void deleteDeadMsgs();
void shiftMessageUp(byte index);

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
  msgs[0] = {0, 0, 0};
  msgs[1] = {1, 0, 0};
  msgs[2] = {2, 0, 0};
  msgs[3] = {3, 0, 0};
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
  calculateTxHash();
  bool pktSent = _radio.send(INIT_ID, &txData, sizeof(txData), NRFLite::REQUIRE_ACK) == 1;
  if (!pktSent) {
    txFailCount++;
  } else {
    txFailCount = 0;
    eraseData();
  }

  if (txFailCount == MAX_TX_RETRY || pktSent) {
    txFailCount = 0;
    eraseData();
    setMessageToPkt();
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

void setMessageToPkt() {
  txPacket->msg.from = msgs[msgToSend].from;
  txPacket->msg.to = msgs[msgToSend].to;
  txPacket->msg.msg = msgs[msgToSend].msg;
  msgLife[msgToSend]--;
  msgToSend++;
  msgToSend %= msgCount;
}

void deleteDeadMsgs() {
  byte index = 0;
  while (index < msgCount) {
    if (!msgLife[index]) {
      shiftMessageUp(index);
    }
    index++;
  }
  msgLife[0] = MESSAGE_LIFE;
}

void shiftMessageUp(byte index) {
  while (index < msgCount - 1) {
    msgs[index].from = msgs[index + 1].from;
    msgs[index].to = msgs[index + 1].to;
    msgs[index].msg = msgs[index + 1].msg;
    msgLife[index] = msgLife[index + 1];

    index++;
  }
  msgCount--;
  msgToSend %= msgCount;
}

/**
   checks if the given message is in the current message list
   returns the index if it can be found
   returns -1 if not in the list
*/
byte findMessage(byte from, byte to, byte msg) {
  byte index = 0;
  while (index < msgCount) {
    if (msgs[index].from == from && msgs[index].to == to && msgs[index].msg == msg) {
      return index;
    }
    index++;
  }
  return -1;
}

void addMessage() {
  if (msgCount < MAX_MESSAGES) {
    msgs[msgCount].from = rxPacket->msg.from;
    msgs[msgCount].to = rxPacket->msg.to;
    msgs[msgCount].msg = rxPacket->msg.msg;
    msgLife[msgCount] = MESSAGE_LIFE;
    msgCount++;
  } else {//if no space replace with the lowerst msgLife
    byte index = 0;
    byte minIndex = 0;
    while (index < msgCount) {
      if (msgLife[index] < msgLife[minIndex]) {
        minIndex = index;
      }
      index++;
    }
    msgs[minIndex].from = rxPacket->msg.from;
    msgs[minIndex].to = rxPacket->msg.to;
    msgs[minIndex].msg = rxPacket->msg.msg;
    msgLife[minIndex] = MESSAGE_LIFE;
  }
}

/**
   checks if the  message in the received packet is in the list
   if not in the list and not a ACK message add it to the list
   if it is a ACK message remove from the list
*/
void updateMessages() {
  byte index = findMessage(rxPacket->msg.from, rxPacket->msg.to, rxPacket->msg.msg & REMOVE_ACK_BIT);
  if ((rxPacket->msg.msg & MESSAGE_ACK_BIT) && (index != -1)) {
    shiftMessageUp(index);
  } else if (!(rxPacket->msg.msg & MESSAGE_ACK_BIT)) {
    addMessage();
  }
}

