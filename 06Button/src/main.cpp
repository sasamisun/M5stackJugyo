#include <M5Unified.h>

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.clear();//ディスプレイクリア
}

void loop()
{
  M5.update();  //ボタンの状態を更新

  //M5.BtnA.wasPressed() は、ボタンAが押されたら1になる。（押し続けると次は0になる)
  if(M5.BtnA.wasPressed()){
    M5.Display.fillScreen(WHITE);
    M5.Display.print(M5.BtnA.lastChange());//ボタンの状態が変わるまでの時間(最後に押してからの時間)
  }

  if(M5.BtnB.wasPressed()){
    M5.Display.fillScreen(GREEN);
  }

}