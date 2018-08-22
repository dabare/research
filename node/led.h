#define GREEN_LED 0 // digitla pin 0
#define RED_LED 4
#define MAX_BRI 200 // max brightness
#define MAX_IDLE_INDI_DElAY 500
#define IDLE_INDI_INTERVAL_ON 10
#define IDLE_INDI_INTERVAL_OFF 40
#define SLEEP_INDI_INTERVAL 100

int idleReset = 0; //use to off the led

void setupLED() {
#ifdef LED_ON
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
#endif
}

void idleBlink() {
#ifdef LED_ON
  if (!idleReset) {
    digitalWrite(RED_LED, HIGH);
    delay(IDLE_INDI_INTERVAL_ON);
    digitalWrite(RED_LED, LOW);
    delay(IDLE_INDI_INTERVAL_OFF);
  } else {
    idleReset--;
  }
#endif
}

void resetIdleBlink() {
  idleReset = MAX_IDLE_INDI_DElAY;
}

void sleepBlink() {
#ifdef LED_ON
  digitalWrite(RED_LED, HIGH);
  delay(SLEEP_INDI_INTERVAL);
  digitalWrite(RED_LED, LOW);
  delay(SLEEP_INDI_INTERVAL);
  digitalWrite(RED_LED, HIGH);
  delay(SLEEP_INDI_INTERVAL);
  digitalWrite(RED_LED, LOW);
#endif
}

void wakeBlink() {
#ifdef LED_ON
  digitalWrite(RED_LED, HIGH);
  delay(SLEEP_INDI_INTERVAL);
  digitalWrite(RED_LED, LOW);
  delay(SLEEP_INDI_INTERVAL);
  digitalWrite(RED_LED, HIGH);
  delay(SLEEP_INDI_INTERVAL);
  digitalWrite(RED_LED, LOW);
  delay(SLEEP_INDI_INTERVAL);
  digitalWrite(RED_LED, HIGH);
  delay(SLEEP_INDI_INTERVAL);
  digitalWrite(RED_LED, LOW);
#endif
}

void onGreen() {
#ifdef LED_ON
  digitalWrite(GREEN_LED, HIGH);
#endif
}

void offGreen() {
#ifdef LED_ON
  digitalWrite(GREEN_LED, LOW);
#endif
}

void onRed() {
#ifdef LED_ON
  digitalWrite(RED_LED, HIGH);
#endif
}

void offRed() {
#ifdef LED_ON
  digitalWrite(RED_LED, LOW);
#endif
}


