#include <M5Unified.h>

#include <Avatar.h>
#include <AquesTalkTTS.h>
#include <tasks/LipSync.h>    // セリフに合わせてアバターの口を動かすプログラムを読み込みます。

//avatarのための初期設定
using namespace m5avatar;
Avatar avatar;

// AquesTalkのライセンスキーを入力します。ライセンスがある場合はライセンスキーを入力します。
// ライセンスキーがない場合は適当な文字列を入力します。
const char* AQUESTALK_KEY = "XXXX-XXXX-XXXX-XXXX";

void setup() {

  auto cfg = M5.config();
  M5.begin(cfg);

  avatar.init(); // start drawing

  TTS.create(AQUESTALK_KEY);          // アバターを喋らせるためのクラスを定義します。
  avatar.addTask(lipSync, "lipSync"); // セリフに合わせて口を動かすよう設定します。
}

void loop() {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    TTS.play("konnichiwa", 80);       // 「こんにちは」と速度80でいいます。
    avatar.setSpeechText("Hello");    // セリフの吹き出しを表示します。
    delay(1000);
    avatar.setSpeechText("");         // セリフの吹き出しを消します。
  }
}

