#include "Test.h"
//#include "SimpleProtocol.h"
//#include "Protocol.h"
//#include "sequence.h"
//#include "aloha.h"


void setup() {
  mySetup();
}

void loop() {
  //_radio.send(INIT_ID, &txData, sizeof(txData), NRFLite::REQUIRE_ACK);
  myLoop();
}


//unsuccessfull broadcast 3mA
//listening 3.2mA
//idle listen 4.7mA
/**
 * 0 --  1542535828143
10000 -- 1542536305654
477511 mili seconds for 10000 packets
47.7511 mili seconds for 1 packet

 */
