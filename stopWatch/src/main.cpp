#include <M5Unified.h>

//ストップウォッチの開始時間を記憶する変数
uint32_t start=0;

//ストップウォッチの状態を保存する変数
//0 停止、1 動作中
uint32_t mode=0;

//画面に表示する時間
uint32_t timevalue=0;

void setup() {

  auto cfg = M5.config();
  M5.begin(cfg);                        // initialize M5 device

  M5.Display.clear();
  M5.Display.setTextSize(3);            // change text size

  M5.Display.setCursor(0, 0);
  M5.Display.print(timevalue);  //時間を表示
}

void loop() {
  M5.update();

  //Aボタンでストップウォッチスタート
  if(M5.BtnA.wasPressed()){
    mode = 1;
    start = millis();//millis()はM5stackが起動してからの経過時間をミリ秒で返します
  }

  //Bボタンでストップ
  if(M5.BtnB.wasPressed()){
    mode = 0;
    timevalue = 0;
  }

  //modeが1(ストップウォッチ動作中)なら、
  if(mode == 1){
    timevalue = millis() - start; //開始ボタンが押されてからの経過時間を計算
    M5.Display.setCursor(0, 0);
    M5.Display.print(timevalue);  //時間を表示
  }


}