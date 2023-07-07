#include <M5Unified.h>
#include <SPIFFS.h>

constexpr auto IMG_PATH = "/10.png";
M5Canvas canvas(&M5.Display);
uint8_t gIMGBuf[8192];

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  SPIFFS.begin();

  M5.Display.fillRect(5, 5, 309, 115, NAVY);
  M5.Display.fillCircle(15, 15, 5, CYAN);
  M5.Display.fillCircle(15, 109, 5, CYAN);
  M5.Display.fillCircle(302, 15, 5, CYAN);
  M5.Display.fillCircle(302, 109, 5, CYAN);
  M5.Display.drawLine(5, 128, 315, 128, ORANGE);

  M5.Display.setFont(&fonts::lgfxJapanMincho_40);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(WHITE);

  M5.Display.setCursor(50,28);
  M5.Display.print("26°");

  M5.Display.setTextSize(1);
  M5.Display.setCursor(55,166);
  M5.Display.print("2023/07/07");

  File file = SPIFFS.open(IMG_PATH, "r");
  file.read(gIMGBuf, 8192);
  file.close();

  canvas.createSprite(100, 100);
  canvas.fillScreen(NAVY);
  canvas.drawPng(gIMGBuf, 8192);
  canvas.pushSprite(170, 12);
}

void loop() {
  // put your main code here, to run repeatedly:
}