#include <M5Unified.h>
#include <ServoEasing.hpp>
//ESP32ServoはServoEasing内で使っているのでここではinclude不要
#include <Avatar.h>

using namespace m5avatar;
Avatar avatar;

//二つのサーボモーターのピン番号をdefineしておきます。
#define SERVO_PIN_X 5
#define SERVO_PIN_Y 2
#define START_DEGREE_VALUE_X 90
#define START_DEGREE_VALUE_Y 90
ServoEasing servo_x;
ServoEasing servo_y;

uint32_t count;

void moveServo()
{
  int x = random(70, 110);
  int y = random(75, 92);
  int delay_time = random(600);
  servo_x.setEaseToD(x, 400 + delay_time);
  servo_y.setEaseToD(y, 400 + delay_time);
  synchronizeAllServosStartAndWaitForAllServosToStop();
  servo_y.setEaseToD(y + 2, 300 + delay_time); // サーボの鳴り防止
  synchronizeAllServosStartAndWaitForAllServosToStop();
}

void setup() {

  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;   // default=115200. if "Serial" is not needed, set it to 0.
  M5.begin(cfg);
  avatar.init(); // start drawing

  // attach(int aPin, int aInitialDegreeOrMicrosecond, int aMicrosecondsForServo0Degree, int aMicrosecondsForServo180Degree)
  //ピン番号、初期角度、最低ON時間(マイクロ秒)、最長ON時間(マイクロ秒)を設定します。
  servo_x.attach(SERVO_PIN_X,
                 START_DEGREE_VALUE_X,
                 DEFAULT_MICROSECONDS_FOR_0_DEGREE,
                 DEFAULT_MICROSECONDS_FOR_180_DEGREE);
  servo_y.attach(SERVO_PIN_Y,
                 START_DEGREE_VALUE_Y,
                 DEFAULT_MICROSECONDS_FOR_0_DEGREE,
                 DEFAULT_MICROSECONDS_FOR_180_DEGREE);
  //両方のサーボモーターにイージングを設定
  servo_x.setEasingType(EASE_QUADRATIC_IN_OUT);
  servo_y.setEasingType(EASE_QUADRATIC_IN_OUT);
  //モーターの動作速度設定。デフォルトを50度毎秒とします。
  setSpeedForAllServos(50);
}

void loop() {
  M5.update();  //ボタンの状態を更新

  
  if(M5.BtnA.wasPressed()){
    servo_y.setEaseTo(100, 50);
  }

  if(M5.BtnB.wasPressed()){
    //90度まで動きます。速度は1秒につき20度の速さです。
    servo_y.setEaseTo(90, 20);
    servo_x.setEaseTo(90, 20);
  }
  
  if(M5.BtnC.wasPressed()){
    //首振り(スタート地点を90として、～160,～20,～90と動く)
    servo_x.setEaseTo(160, 20);
    synchronizeAllServosStartAndWaitForAllServosToStop();
    servo_x.setEaseTo(20, 20);
    synchronizeAllServosStartAndWaitForAllServosToStop();
    servo_x.setEaseTo(90, 20);
  }
  //サーボモーターの動きがすべて終わるまで処理を停止します。
  synchronizeAllServosStartAndWaitForAllServosToStop();


}