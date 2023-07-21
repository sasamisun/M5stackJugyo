#include <M5Unified.h>
#include <BleKeyboard.h>

//BleKeyboard bleKeyboard("device name");//でデバイス名を設定可能
BleKeyboard bleKeyboard("M5stic-C presenter");

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
      bleKeyboard.write(KEY_RETURN);
      delay(50);
    }

    if(M5.BtnB.wasPressed()){
      bleKeyboard.write(KEY_BACKSPACE);
      delay(50);
    }

  }
}
