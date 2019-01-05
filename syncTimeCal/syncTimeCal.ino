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
  while (!_radio.initTwoPin(254, PIN_RADIO_MOMI, PIN_RADIO_SCK)) {
    Serial.print(".");
  }

  _radio.printDetails();

  Serial.println();
  Keyboard.begin();
  delay(1000);
  //Keyboard.println("Ready");
}

unsigned long count = 0;
void loop() {
  //countPackets();
  if (_radio.hasData()) {
    _radio.readData(&data); // Note how '&' must be placed in front of the variable name.

    if (rxPacket->data[1] == 's') {
      Keyboard.print(rxPacket->data[0]);

      Keyboard.press(KEY_RIGHT_ARROW);
      Keyboard.release(KEY_RIGHT_ARROW);

      Keyboard.print(rxPacket->from);

      Keyboard.press(KEY_RIGHT_ARROW);
      Keyboard.release(KEY_RIGHT_ARROW);


      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(';');
      Keyboard.releaseAll();
      Keyboard.press(KEY_DOWN_ARROW);
      Keyboard.release(KEY_DOWN_ARROW);
      Keyboard.press(KEY_HOME);
      Keyboard.release(KEY_HOME);
    }else{
      Keyboard.print(rxPacket->from);

      Keyboard.press(KEY_RIGHT_ARROW);
      Keyboard.release(KEY_RIGHT_ARROW);

      Keyboard.print("B");

      Keyboard.press(KEY_RIGHT_ARROW);
      Keyboard.release(KEY_RIGHT_ARROW);


      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(';');
      Keyboard.releaseAll();
      Keyboard.press(KEY_DOWN_ARROW);
      Keyboard.release(KEY_DOWN_ARROW);
      Keyboard.press(KEY_HOME);
      Keyboard.release(KEY_HOME);
    }
  }
}

