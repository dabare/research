#include "sleep.h"
#include "switch.h"

void setup() {
  setupSleep();
  setupRadio();
  resetIdle();
}

int work_time = 0;
void loop() {
  idle();
  work();
}
byte data[12] = {1, 2, 0, 0, 0};
void work() {
  sw_1.Update(); sw_2.Update(); sw_3.Update();

  if (sw_1.working | sw_2.working | sw_3.working) {
    resetIdle();
  }

  if (sw_1.clicks) {
    data[2] = sw_1.clicks - 1;
    send(1, &data, sizeof(data));
  }

  //my room
  if (sw_1.clicks == 1) {
    data[2] = 0;
    //send(1, &data, sizeof(data));
  }
  if (sw_2.quickPressed) {
    data[2] = 1;
    send(1, &data, sizeof(data));
  }
  if (sw_3.quickPressed) {
    data[2] = 2;
    send(1, &data, sizeof(data));
  }

}


void send(byte _id, void* _data, int _size) {
  setupRadio();
  _radio.send(_id, _data, _size, NRFLite::NO_ACK);
}

void client() {
  while (_radio.hasData()) {
    _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.
  }
}

