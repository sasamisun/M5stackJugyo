#include <M5Unified.h>

int ippaku = 200;

void setup() {
  M5.begin();
  M5.Speaker.setVolume(60);
  M5.Speaker.tone(1000,ippaku*2);
  delay(ippaku*2);
  
}

void loop() {

}

