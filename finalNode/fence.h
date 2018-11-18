#define SW 2 // digitla pin 2 still available , not used

#define NEON_1 0
#define NEON_2 4

#define NEON_OK LOW
int neonLife = 0;
bool neonStat = false;

void setupNeon() {
  pinMode(NEON_1, INPUT_PULLUP);
  pinMode(NEON_2, INPUT_PULLUP);
}

void checkNeon(){
  if(!(digitalRead(NEON_1) ^ NEON_OK) || !(digitalRead(NEON_2) ^ NEON_OK)){
    neonLife = NEON_LIFE;
    neonStat = true;
  }
}

bool isFenceOK() {
  return !(digitalRead(NEON_1) ^ NEON_OK) || !(digitalRead(NEON_2) ^ NEON_OK);
  if(neonLife){
    neonLife--;
  }else{
    neonStat = 0;
  }
  return neonStat;
}


