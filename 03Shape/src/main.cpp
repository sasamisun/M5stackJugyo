#include <M5Unified.h>


void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.fillScreen(WHITE);//画面を白で塗りつぶし

  M5.Display.drawPixel(5, 5, BLACK);  //点を描画
  M5.Display.drawLine(10, 20, 310, 50, RED);  //直線を描画
  
  M5.Display.drawRect(10, 50, 60, 70, BLUE);  //四角形を描画
  M5.Display.drawRoundRect(200, 55, 100, 70, 10, ORANGE); //角丸四角形を描画
  M5.Display.drawCircle(160, 150, 30, BLACK);  //正円を描画
  M5.Display.drawEllipse(100,100,20,30, PURPLE);  //楕円を描画
  M5.Display.drawTriangle(60, 80, 40, 160, 150, 90, DARKCYAN);  //三角形を描画

}

void loop() {
  // put your main code here, to run repeatedly:
}