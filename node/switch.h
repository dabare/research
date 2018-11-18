#include "clickButton.h"
#define SW 2 // digitla pin 2
#define FENCE_OK LOW
int neonLife = 0;
bool neonStat = false;

void setupSW() {
  pinMode(SW, INPUT_PULLUP);
}

bool isFenceOK() {
  return true;
  if(!(digitalRead(SW) ^ FENCE_OK)){
    neonLife = NEON_LIFE;
    neonStat = true;
  }
  if(!neonLife--){
    neonStat = 0;
  }
  return neonStat;
}


