#include "Test.h"
//#include "SimpleProtocol.h"
//#include "Protocol.h"
//#include "sequence.h"
//#include "aloha.h"


void setup() {
  mySetup();
}

void loop() {
  _radio.send(INIT_ID, &txData, sizeof(txData), NRFLite::REQUIRE_ACK);
  //myLoop();
}

//unsuccessfull broadcast 3mA
//listening 3.2mA
//idle listen 4.7mA

