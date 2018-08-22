/*

  Demonstrates simple RX and TX operation using 2 pins for the radio.
  Only AVR architectures (ATtiny/ATmega) support 2 pin operation.
  Please read the notes in NRFLite.h for a description of all library features.

  Radio circuit
  Follow the 2-Pin Hookup Guide on https://github.com/dparson55/NRFLite

  Connections
  Arduino 9  > MOMI of 2-pin circuit
  Arduino 10 > SCK of 2-pin circuit

*/
#include <NRFLite.h>
#define PIN_RADIO_MOMI 1
#define PIN_RADIO_SCK  3

const static uint8_t RADIO_ID = 0;       // Our radio's id.  The transmitter will send to this id.


struct RadioPacket // Any packet up to 32 bytes can be sent.
{
  uint8_t Id;
  uint32_t count;
  uint32_t FailedTxCount;
};

NRFLite _radio;
RadioPacket _radioData;

void setup()
{
  pinMode(0, OUTPUT);
  pinMode(4, OUTPUT);

  if (!_radio.initTwoPin(RADIO_ID, PIN_RADIO_MOMI, PIN_RADIO_SCK))
  {
    while (1); // Wait here forever.
  }

  /*
    By default, 'init' configures the radio to use a 2MBPS bitrate on channel 100 (channels 0-125 are valid).
    Both the RX and TX radios must have the same bitrate and channel to communicate with each other.
    You can assign a different bitrate and channel as shown below.
    _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, 0)
    _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE1MBPS, 75)
    _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE2MBPS, 100) // THE DEFAULT
  */
  _radioData.count = 100;
  _radio.send(2, &_radioData, sizeof(_radioData));
}
byte i;
void loop() {

  if (_radio.send(i, &_radioData, sizeof(_radioData))) {
    digitalWrite(0, HIGH);
    digitalWrite(4, LOW);
    delay(100);
    digitalWrite(0, LOW);
    digitalWrite(4, HIGH);
    delay(100);
  } else {
    //Serial.println("Falil");
    digitalWrite(0, LOW);
    digitalWrite(4, LOW);
    delay(100);
  }
  i++;
  i %= 6;
 
}

