
#include <M5Unified.h>
#include <SPIFFS.h>

constexpr auto IMG_PATH1 = "/10.png";
constexpr auto IMG_PATH2 = "/20.png";
constexpr auto IMG_PATH3 = "/30.png";

//画像はcanvasという仕組みを使って表示する。
M5Canvas canvas(&M5.Display);
// 画像データを一時保存する変数
uint8_t gIMGBuf[8192];


void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  SPIFFS.begin(); //SPIFFSに保存した画像を読みたいので、SPIFFSを使えるようにする。

  //画像１を表示
  auto file = SPIFFS.open(IMG_PATH1, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 8192);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる

  canvas.createSprite(100, 100);            //100x100サイズのスプライト（表示領域）を作る
  canvas.drawPng(gIMGBuf, 8192);            //表示領域に対してgIMGBufの内容を書き込む
  canvas.pushSprite(0, 0);                  //スプライトを指定座標に描画する

  //画像２を表示
  file = SPIFFS.open(IMG_PATH2, "r");
  file.read(gIMGBuf, 8192);
  file.close();

  canvas.createSprite(100, 100);
  canvas.drawPng(gIMGBuf, 8192);
  canvas.pushSprite(100, 0);

  //画像３を表示
  file = SPIFFS.open(IMG_PATH3, "r");
  file.read(gIMGBuf, 8192);
  file.close();

  canvas.createSprite(100, 100);
  canvas.drawPng(gIMGBuf, 8192);
  canvas.pushSprite(200, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
}