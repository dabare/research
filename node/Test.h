#include "radio.h"

void work();
void sleep();
void broadcastSYNC();

struct SyncPacket { // Any packet up to 32 bytes can be sent.
  uint8_t from;
  int sleep;
  int work;
  byte a[6];
};

struct RadioPacket { // Any packet up to 32 bytes can be sent.
  uint8_t from;
  int sleep;
  int work;
};


SyncPacket _sync;

RadioPacket _radioData;

RadioPacket _gotData;
RadioPacket _toSendData;

void mySetup() {
  setupLED();
  setupMemory();

  if (RADIO_ID == ROOT_ID) {
    CURRENT_ID = RADIO_ID;

    WORK_TIME = 100;
    SLEEP_TIME = 100;

    _sync.from = RADIO_ID;
    _sync.sleep = SLEEP_TIME;
    _sync.work = WORK_TIME;
  }

  setupRadio(CURRENT_ID);
}

int cycle;
void myLoop() {
  cycle = WORK_TIME;
  if (CURRENT_ID != INIT_ID) {
    broadcastSYNC();
  }
  while (cycle--) {
    work();
  }
  cycle = SLEEP_TIME;
  while (cycle--) {
    sleep();
  }
}

void sleep() {
  _radio.powerDown();
  offRed();
}

void work() {
  onRed();
  if (_radio.hasData()) {
    onGreen();

    cycle = WORK_TIME;
    if (CURRENT_ID == INIT_ID) {
      CURRENT_ID = RADIO_ID;
      setupRadio(CURRENT_ID);

      _radio.readData(&_sync);
      WORK_TIME = _sync.work;
      SLEEP_TIME = _sync.sleep;
      _sync.from = RADIO_ID;

      cycle = WORK_TIME;
    } else {
      _radio.readData(&_gotData);
    }

    offGreen();
  }
}

void broadcastSYNC() {
  //_radio.send(INIT_ID, &_sync, sizeof(_sync), NRFLite::NO_ACK);
  _radio.send(INIT_ID, &_sync, sizeof(_sync), NRFLite::REQUIRE_ACK);
}
