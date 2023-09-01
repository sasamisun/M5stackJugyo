#include <M5Unified.h>
#include <SPIFFS.h>
constexpr auto AUDIO_PATH = "/puyo32kbit.wav";
uint8_t sound_buff[27502];


void setup() {
  auto cfg = M5.config();
  cfg.external_speaker.module_display = true;
  M5.begin(cfg);
  M5.Speaker.begin();

  //スピーカーが使えなければ、プログラムを停止する。
  if (!M5.Speaker.isEnabled())
  {
    M5.Display.print("Speaker not found...");
    for (;;) { M5.delay(1); }
  }

  //SPIFFSからファイルを読み込む
  auto file = SPIFFS.open(AUDIO_PATH, "r");
  file.read(sound_buff,27502);
  file.close();

  //音量調整(0～255 default:64)
  M5.Speaker.setVolume(64);

  M5.Speaker.playWav(sound_buff, sizeof(sound_buff), ~0u, 0, true);
}

void loop() {
  // put your main code here, to run repeatedly:
}

