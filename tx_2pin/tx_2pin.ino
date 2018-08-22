/*

Demonstrates simple RX and TX operation using 2 pins for the radio.
Only AVR architectures (ATtiny/ATmega) support 2 pin operation.
Please read the notes in NRFLite.h for a description of all library features.

Radio circuit
* Follow the 2-Pin Hookup Guide on https://github.com/dparson55/NRFLite

Connections
* Arduino 9  > MOMI of 2-pin circuit
* Arduino 10 > SCK of 2-pin circuit

*/

#include <NRFLite.h>

const static uint8_t RADIO_ID = 2;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 0; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_MOMI = 1;
const static uint8_t PIN_RADIO_SCK = 3;

struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
    uint32_t FailedTxCount;
};

NRFLite _radio;
RadioPacket _radioData;

void setup()
{
    analogWrite(4,0);
    pinMode(2, INPUT_PULLUP);
    
    if (!_radio.initTwoPin(RADIO_ID, PIN_RADIO_MOMI, PIN_RADIO_SCK))
    {
        while (1); // Wait here forever.
    }

    _radioData.FromRadioId = RADIO_ID;

    _radio.send(0, &_radioData, sizeof(_radioData));
    _radio.send(0, &_radioData, sizeof(_radioData));
    _radio.send(0, &_radioData, sizeof(_radioData));
    _radio.send(0, &_radioData, sizeof(_radioData));
    _radio.send(0, &_radioData, sizeof(_radioData));
    _radio.send(0, &_radioData, sizeof(_radioData));

}
void loop(){
  _radio.powerDown();
  
     if (digitalRead(2) && _radio.hasData()){
      //analogWrite(2,255);
      analogWrite(0,0);analogWrite(4,100);
        _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.
        ++_radioData.OnTimeMillis;
        _radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData));
        //delay(1);
       // analogWrite(2,0);
        analogWrite(0,100);analogWrite(4,0);
    }else{
      //_radio.powerDown();
    }
    //k++;
}
