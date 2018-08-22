/*

  Demonstrates simple RX and TX operation.
  Please read the notes in NRFLite.h for a description of all library features.

  Radio    Arduino
  CE    -> 9
  CSN   -> 10 (Hardware SPI SS)
  MOSI  -> 11 (Hardware SPI MOSI)
  MISO  -> 12 (Hardware SPI MISO)
  SCK   -> 13 (Hardware SPI SCK)
  IRQ   -> No connection
  VCC   -> No more than 3.6 volts
  GND   -> GND

*/

#include <SPI.h>
#include <NRFLite.h>

const static uint8_t RADIO_ID = 0;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;

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
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  
  Serial.begin(115200);
  Serial.println("Start");
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
  {
    Serial.println("Cannot communicate with radio");
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

void loop() {

  if (_radio.send(3, &_radioData, sizeof(_radioData))) {
    Serial.println("Success");
    digitalWrite(6,HIGH);
    digitalWrite(7,LOW);
    delay(100);
    digitalWrite(6,LOW);
    digitalWrite(7,HIGH);
    delay(100);
  } else {
    //Serial.println("Falil");
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    delay(100);
  }

  return ;
  
  if (_radio.hasData()){
    _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.

    String msg = "Got from Radio ";
    msg += _radioData.Id;
    msg += ", count ";
    msg += _radioData.count;

    //Serial.println(msg);

    msg = "Sending ";
    msg += _radioData.count;

    //Serial.println(msg);
    //delay(50);
    //if (_radio.send(2, &_radioData, sizeof(_radioData)), NRFLite::NO_ACK) {
    if (_radio.send(2, &_radioData, sizeof(_radioData))) {
      Serial.println("Success");
    } else {
      Serial.println("Falil");
    }
  }

}

