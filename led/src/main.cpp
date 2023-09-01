#include <M5Unified.h>

void setup() {
  M5.begin();
  pinMode(2, OUTPUT);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  //スイッチを押すと0　話すと1
  int sw = digitalRead(5);

  if(sw){
    digitalWrite(2,HIGH);
  }else{
    digitalWrite(2,LOW);
  }

}

