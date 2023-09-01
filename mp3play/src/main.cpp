#include <M5Unified.h>
#include <WiFi.h>
#include "SPIFFS.h"

#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2SNoDAC *out;
AudioFileSourceID3 *id3;

void setup() {
  auto cfg = M5.config();
  // If you want to play sound from ATOMIC Speaker, write this
  //cfg.external_speaker.atomic_spk     = true;

  // If you want to play sound from ModuleDisplay, write this
  cfg.external_speaker.module_display = true;

  // If you want to play sound from ModuleRCA, write this
  //  cfg.external_speaker.module_rca     = true;

  // If you want to play sound from HAT Speaker, write this
  //  cfg.external_speaker.hat_spk        = true;

  // If you want to play sound from HAT Speaker2, write this
  //  cfg.external_speaker.hat_spk2       = true;

  M5.begin(cfg);

  { /// I2S Custom configurations are available if you desire.
    auto spk_cfg = M5.Speaker.config();

    if (spk_cfg.use_dac || spk_cfg.buzzer)
    {
    /// Increasing the sample_rate will improve the sound quality instead of increasing the CPU load.
      spk_cfg.sample_rate = 192000; // default:64000 (64kHz)  e.g. 48000 , 50000 , 80000 , 96000 , 100000 , 128000 , 144000 , 192000 , 200000
    }
    /*
    spk_cfg.pin_data_out=8;
    spk_cfg.pin_bck=7;
    spk_cfg.pin_ws=10;     // LRCK

    /// use single gpio buzzer, ( need only pin_data_out )
    spk_cfg.buzzer = false;

    /// use DAC speaker, ( need only pin_data_out ) ( only GPIO_NUM_25 or GPIO_NUM_26 )
    spk_cfg.use_dac = false;
    // spk_cfg.dac_zero_level = 64; // for Unit buzzer with DAC.

    /// Volume Multiplier
    spk_cfg.magnification = 16;
    //*/
    M5.Speaker.config(spk_cfg);


  }
  M5.Speaker.begin();
  WiFi.mode(WIFI_OFF); 

  audioLogger = &Serial;
  file = new AudioFileSourceSPIFFS("/puyo.mp3");
  id3 = new AudioFileSourceID3(file);
  out = new AudioOutputI2SNoDAC();
  out->SetOutputModeMono(true);
  out->SetGain(0.7);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
}

void loop() {
  M5.Display.setCursor(172,112);
  if(mp3->isRunning()){
    M5.Display.print("->");
  }else{
    M5.Display.print("[]");
  }
  
}
