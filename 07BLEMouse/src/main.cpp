#include <M5Unified.h>
#include <BleMouse.h>

//BleMouse bleMouse("Mymouse");//これでデバイス名を指定することも可能。
BleMouse bleMouse;

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.clear();//ディスプレイクリア
  M5.Display.print("Mouse Mode");
  bleMouse.begin();//マウス動作をスタート
}

void loop()
{
  M5.update();  //ボタンの状態を更新

  if(M5.BtnA.wasPressed()){
    bleMouse.click(MOUSE_LEFT); //マウス左クリック
    delay(500);
  }

  if(M5.BtnB.wasPressed()){
    bleMouse.click(MOUSE_MIDDLE); //マウス中央クリック
    delay(500);
  }

  if(M5.BtnC.wasPressed()){
    bleMouse.click(MOUSE_RIGHT); //マウス右クリック
    delay(500);
  }
}
