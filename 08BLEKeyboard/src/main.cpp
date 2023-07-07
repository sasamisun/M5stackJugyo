#include <M5Unified.h>
#include <BleKeyboard.h>

//BleKeyboard bleKeyboard("device name");//でデバイス名を設定可能
BleKeyboard bleKeyboard;

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.clear();//ディスプレイクリア
  M5.Display.print("Keyboard Mode");
  bleKeyboard.begin();
}

void loop()
{
  M5.update();  //ボタンの状態を更新

  if(bleKeyboard.isConnected()) {//bluetooth接続がOKの時だけ動く
    if(M5.BtnA.wasPressed()){
      M5.Display.fillScreen(WHITE);
      //指定文字のキーストロークを送信
      bleKeyboard.print("Hello world");
      delay(50);
    }

    if(M5.BtnB.wasPressed()){
      M5.Display.fillScreen(BLUE);
      //リターンキーを送信
      bleKeyboard.write(KEY_RETURN);
      delay(50);
    }

    if(M5.BtnC.wasPressed()){
      M5.Display.fillScreen(BLACK);
      //Windows+shift+[s]　の同時押し
      bleKeyboard.press(KEY_LEFT_GUI);  //windowsキー押しっぱなし
      bleKeyboard.press(KEY_LEFT_SHIFT);//Shiftキー押しっぱなし
      bleKeyboard.press('s');           //sキー押しっぱなし(Stringじゃなくてcharです。気を付けて！)
      delay(100);                       //0.1秒押しっぱなし信号を送る
      bleKeyboard.releaseAll();         //押しっぱなし解除
      delay(50);
    }
  }
}
