#define LED_ON
//#define RADIO_ID 4

#define PACKET_SIZE 32

#define MIN_ID 0
#define MAX_ID 7
#define INIT_ID 255// broadcast id
#define ROOT_ID 1

#define DEFAULT_SLEEP_TIME 70//475
#define DEFAULT_WORK_TIME 30//325

#define MAX_NEIGHBOURS 10
#define NEIGHBOUR_LIFE 15

#define MAX_MESSAGES 25
#define MESSAGE_LIFE 100

#define MESSAGE_ACK_BIT 0b10000000
#define REMOVE_ACK_BIT 0b01111111

#define MAX_TX_RETRY 10

#define PAD 50
#define PAD_MOD 3

int SLEEP_TIME = DEFAULT_SLEEP_TIME;
int WORK_TIME = DEFAULT_WORK_TIME;

uint8_t RADIO_ID = 255;             // Our radio's id.

uint8_t CURRENT_ID = INIT_ID;

#define ZONE_RADIOUS 2

#define NEON_LIFE 5000
