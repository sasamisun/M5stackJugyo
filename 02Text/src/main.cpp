#include <M5Unified.h>


void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.fillScreen(WHITE);//画面を白で塗りつぶし

  M5.Display.setTextSize(7);  //文字の大きさを指定
  M5.Display.setTextColor(ORANGE,NAVY);  //文字の色をオレンジ、背景色をネイビーに指定

  M5.Display.print("Hello!");

  M5.Display.setCursor(50,60); //次の文字表示位置を指定
  M5.Display.setTextColor(BLACK); //文字の色だけ黒に指定。背景は指定していないのでリセットされる

  M5.Display.print("@ @");

  //日本語表示のしかた

  M5.Display.setFont(&fonts::lgfxJapanMincho_12); //// ゴシック体（8,12,16,20,24,28,32,36,40）
  M5.Display.setCursor(50,120);
  M5.Display.setTextSize(3);  //文字の大きさを指定
  M5.Display.print("こんにちは日本語！");

}

void loop() {
  // put your main code here, to run repeatedly:
}