#include <M5Unified.h>
#include <WiFi.h>

//wifi接続設定
const char* ssid = "ssid";
const char* password = "password";

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.clear();
  
  M5.Display.setCursor(0,0);
  M5.Display.print("Wifi connecting");

  WiFi.begin(ssid, password); //Wifiに接続を開始

  //接続が完了するまで無限ループ(SSIDやパスフレーズが間違っているとここで動かなくなる)
  while (WiFi.status() != WL_CONNECTED){  //接続が完了すると、WiFi.status()はWL_CONNECTEDになる
      delay(500);
      M5.Lcd.print('.');
  }

  M5.Display.clear();
  M5.Display.setCursor(0,0);
  M5.Lcd.print("\r\nWiFi connected\r\nIP address: ");
  M5.Lcd.println(WiFi.localIP());//接続が完了するとWiFi.localIP()はローカルIPアドレスを返すようになる
}

void loop()
{


}