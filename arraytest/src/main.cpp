#include <M5Unified.h>

float datas1[] = {10.0, 2.0, 5.0, -1};

// put function declarations here:
void myFunction(float* data){
  while(*data != -1){
    Serial.println(*data);
    data++;
  }
}

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Serial.println("start");
  myFunction(datas1);
}

void loop() {
  // put your main code here, to run repeatedly:
}

