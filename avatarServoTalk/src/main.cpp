#include <M5Unified.h>
#include <ServoEasing.hpp>
#include <Avatar.h>
#include <AquesTalkTTS.h>
#include <tasks/LipSync.h>    // セリフに合わせてアバターの口を動かすプログラムを読み込みます。
#include <BeraFace.h>

#define S1 2  //上下に動くサーボモーター
#define S2 5  //左右に動くサーボモーター
ServoEasing servo1;
ServoEasing servo2;

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
  servo1.attach(S1, 60, 500, 2400);
  servo2.attach(S2, 90, 500, 2400);
  //サーボモーターの動きがすべて終わるまで処理を停止します。
  synchronizeAllServosStartAndWaitForAllServosToStop();
  //両方のサーボモーターにイージングを設定
  servo1.setEasingType(EASE_CIRCULAR_OUT);
  servo2.setEasingType(EASE_CIRCULAR_OUT);
  //モーターの動作速度設定
  setSpeedForAllServos(60);

  TTS.create(AQUESTALK_KEY);          // アバターを喋らせるためのクラスを定義します。
  avatar.addTask(lipSync, "lipSync"); // セリフに合わせて口を動かすよう設定します。
  avatar.setFace(new BeraFace());
  avatar.init(); // start drawing
    avatar.setSpeechFont(&fonts::lgfxJapanGothic_12); //表示するフォントを指定(これをしないと日本語が表示されない)
    //TTS.play("yahho-", 90);                           // 「こんにちは」と速度80でいいます。
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
  if(gazeV < 0.0){
    servo1.setEaseTo(60, 500);
    Serial.println("MOVE!");
  }else{
    servo1.setEaseTo(90, 500);
  }
  Serial.println(gazeV);
  synchronizeAllServosStartAndWaitForAllServosToStop();

}