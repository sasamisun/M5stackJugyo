#include <M5Unified.h>
#include <SD.h>           //追加
#include <AquesTalkTTS.h> //追加

// AquesTalkのライセンスキーを入力します。ライセンスがある場合はライセンスキーを入力します。
// ライセンスキーがない場合は適当な文字列を入力します。
const char* AQUESTALK_KEY = "XXXX-XXXX-XXXX-XXXX";//追加


void setup() {
  M5.begin();
  //ここから下が必要
  SD.begin(GPIO_NUM_4);
  TTS.createK(AQUESTALK_KEY);          // アバターを喋らせるためのクラスを定義します。
  TTS.playK("こんにちは", 90);
}

void loop() {
}

