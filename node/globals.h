#define LED_ON
//#define RADIO_ID 4

#define PACKET_SIZE 32

#define MIN_ID 0
#define MAX_ID 7
#define INIT_ID 155
#define ROOT_ID 1

#define DEFAULT_SLEEP_TIME 375
#define DEFAULT_WORK_TIME 225

#define PAD 50

#define PAD_MOD 2

int SLEEP_TIME = DEFAULT_SLEEP_TIME;
int WORK_TIME = DEFAULT_WORK_TIME;

uint8_t RADIO_ID = 255;             // Our radio's id.

uint8_t CURRENT_ID = INIT_ID;
