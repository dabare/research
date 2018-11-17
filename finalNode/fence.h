#define SW 2 // digitla pin 2 is still available 

#define FENCE_OK HIGH

//extern ClickButton sw(SW, LOW,HIGH);

void setupSW() {
  pinMode(SW, INPUT_PULLUP);
}

bool isFenceOK() {
  return !(digitalRead(SW) ^ FENCE_OK);
}


