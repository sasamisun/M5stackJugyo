#include <M5Unified.h>
#include <ServoEasing.hpp>
//ESP32ServoはServoEasing内で使っているのでここではinclude不要
#include <Avatar.h>

using namespace m5avatar;
Avatar avatar;

//二つのサーボモーターのピン番号をdefineしておきます。
#define S1 2  //上下に動くサーボモーター
#define S2 5  //左右に動くサーボモーター

//サーボモーター2つ分のインスタンスを生成します。
ServoEasing servo1;
ServoEasing servo2;

uint32_t count;

void setup() {

  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;   // default=115200. if "Serial" is not needed, set it to 0.
  M5.begin(cfg);
  avatar.init(); // start drawing

  // attach(int aPin, int aInitialDegreeOrMicrosecond, int aMicrosecondsForServo0Degree, int aMicrosecondsForServo180Degree)
  //ピン番号、初期角度、最低ON時間(マイクロ秒)、最長ON時間(マイクロ秒)を設定します。
  servo1.attach(S1, 90, 500, 2400);
  servo2.attach(S2, 90, 500, 2400);

  //サーボモーターの動きがすべて終わるまで処理を停止します。
  synchronizeAllServosStartAndWaitForAllServosToStop();

  //両方のサーボモーターにイージングを設定
  servo1.setEasingType(EASE_CIRCULAR_OUT);
  servo2.setEasingType(EASE_CIRCULAR_OUT);

  //モーターの動作速度設定
  setSpeedForAllServos(60);
  


}

void loop() {
  M5.update();  //ボタンの状態を更新

  
  if(M5.BtnA.wasPressed()){
    servo1.setEaseTo(120, 500);
  }

  if(M5.BtnB.wasPressed()){
    //500ミリ秒かけて90度まで動きます。
    servo1.setEaseTo(40, 500);
    servo2.setEaseTo(90, 500);
  }
  
  if(M5.BtnC.wasPressed()){
    //首振り(スタート地点を90として、～160,～20,～90と動く)
    servo2.setEaseTo(160, 1000);
    synchronizeAllServosStartAndWaitForAllServosToStop();
    servo2.setEaseTo(20, 1500);
    synchronizeAllServosStartAndWaitForAllServosToStop();
    servo2.setEaseTo(90, 1000);
  }
  //サーボモーターの動きがすべて終わるまで処理を停止します。
  synchronizeAllServosStartAndWaitForAllServosToStop();

}