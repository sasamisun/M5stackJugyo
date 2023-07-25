#include <M5Unified.h>
#include <Avatar.h>
#include <BeraFace.h>

using namespace m5avatar;

Avatar avatar;

void setup()
{
  M5.begin();
  avatar.setFace(new BeraFace());
  avatar.init(); // start drawing
    avatar.setSpeechFont(&fonts::lgfxJapanGothic_12); //表示するフォントを指定(これをしないと日本語が表示されない)
    avatar.setSpeechText("やっぽー");                 //吹き出しを表示
    avatar.setMouthOpenRatio(0.7);                    //口をあける
    delay(2000);                                      //2秒まつ
    avatar.setMouthOpenRatio(0);                      //口を閉じる
    avatar.setSpeechText("");                         //吹き出しを消す
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
}