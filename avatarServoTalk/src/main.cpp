#include <M5Unified.h>
#include <ServoEasing.hpp>
#include <Avatar.h>
#include <AquesTalkTTS.h>
#include <tasks/LipSync.h>    // セリフに合わせてアバターの口を動かすプログラムを読み込みます。
#include <BeraFace.h>

//二つのサーボモーターのピン番号をdefineしておきます。
#define SERVO_PIN_X 5
#define SERVO_PIN_Y 2
#define START_DEGREE_VALUE_X 90
#define START_DEGREE_VALUE_Y 90
ServoEasing servo_x;
ServoEasing servo_y;

using namespace m5avatar;
Avatar avatar;

float *gazeVp,*gazeHp;
float gazeV,gazeH;

// AquesTalkのライセンスキーを入力します。ライセンスがある場合はライセンスキーを入力します。
// ライセンスキーがない場合は適当な文字列を入力します。
const char* AQUESTALK_KEY = "XXXX-XXXX-XXXX-XXXX";

void setup()
{
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  M5.begin(cfg);

  // attach(int aPin, int aInitialDegreeOrMicrosecond, int aMicrosecondsForServo0Degree, int aMicrosecondsForServo180Degree)
  //ピン番号、初期角度、最低ON時間(マイクロ秒)、最長ON時間(マイクロ秒)を設定します。
  servo_x.attach(SERVO_PIN_X,
                 START_DEGREE_VALUE_X,
                 DEFAULT_MICROSECONDS_FOR_0_DEGREE,
                 DEFAULT_MICROSECONDS_FOR_180_DEGREE);
  servo_y.attach(SERVO_PIN_Y,
                 START_DEGREE_VALUE_Y-20,
                 DEFAULT_MICROSECONDS_FOR_0_DEGREE,
                 DEFAULT_MICROSECONDS_FOR_180_DEGREE);
  //両方のサーボモーターにイージングを設定
  servo_x.setEasingType(EASE_QUADRATIC_IN_OUT);
  servo_y.setEasingType(EASE_QUADRATIC_IN_OUT);
  //モーターの動作速度設定
  setSpeedForAllServos(5);

  TTS.create(AQUESTALK_KEY);          // アバターを喋らせるためのクラスを定義します。
  avatar.addTask(lipSync, "lipSync"); // セリフに合わせて口を動かすよう設定します。
  avatar.setFace(new BeraFace());
  avatar.init(); // start drawing
    avatar.setSpeechFont(&fonts::lgfxJapanGothic_12); //表示するフォントを指定(これをしないと日本語が表示されない)
    TTS.play("yahho-", 90);                           // 「こんにちは」と速度80でいいます。
    avatar.setSpeechText("やっぽー");                 //吹き出しを表示
    avatar.setMouthOpenRatio(0.7);                    //口をあける
    delay(2000);                                      //2秒まつ
    avatar.setMouthOpenRatio(0);                      //口を閉じる
    avatar.setSpeechText("");                         //吹き出しを消す

    gazeV=0;
    gazeVp = &gazeV;
    gazeH=0;
    gazeHp = &gazeH;
    avatar.getGaze(gazeVp,gazeHp);
}

void loop()
{
  
  M5.update();
  if(M5.BtnA.wasPressed()){
    avatar.setExpression(Expression::Angry);
  }
  if(M5.BtnB.wasPressed()){
    avatar.setExpression(Expression::Happy);
  }
  if(M5.BtnC.wasPressed()){
    avatar.setExpression(Expression::Sleepy);
  }

  if(M5.BtnA.pressedFor(1000)){
    avatar.setExpression(Expression::Doubt);
  }
  if(M5.BtnB.pressedFor(1000)){
    avatar.setExpression(Expression::Sad);
  }
  if(M5.BtnC.pressedFor(1000)){
    avatar.setExpression(Expression::Neutral);
  }
  avatar.getGaze(gazeVp,gazeHp);
  if(gazeV < -0.5){
    servo_y.setEaseTo(45, 10);
  }else{
    servo_y.setEaseTo(60, 10);
  }
  if(gazeH < -0.5){
    servo_x.setEaseTo(60, 10);
  }else if(gazeH > 0.5){
    servo_x.setEaseTo(120, 10);
  }else{
    servo_x.setEaseTo(90, 10);
  }

  Serial.println(gazeV);
  synchronizeAllServosStartAndWaitForAllServosToStop();

}