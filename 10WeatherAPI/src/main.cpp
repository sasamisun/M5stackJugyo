#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

constexpr auto IMG_PATH1 = "/10.png";
constexpr auto IMG_PATH2 = "/20.png";
constexpr auto IMG_PATH3 = "/30.png";

//画像はcanvasという仕組みを使って表示する。
M5Canvas canvas(&M5.Display);
// 画像データを一時保存する変数
uint8_t gIMGBuf[8192];

//前項で計算した、JSONデータのサイズを<>内に書きます。
StaticJsonDocument<768> jsondata;

//HTTP接続用の変数
HTTPClient http;
//API実行結果を入れるための変数
const char* payload;
//接続先URL
const char* url = "https://api.open-meteo.com/v1/forecast?latitude=35.6895&longitude=139.6917&daily=weathercode,temperature_2m_max,temperature_2m_min&timezone=Asia%2FTokyo&forecast_days=1";

//wifi接続設定
const char* ssid = "BEHEMOTH1307";
const char* password = "wlan1024";


void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  SPIFFS.begin(); //SPIFFSに保存した画像を読みたいので、SPIFFSを使えるようにする。

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

  
  http.begin(url);          //指定のURLに接続する準備
  int httpCode = http.GET();//HTMLでいうところの、form medhot="GET" でsubmitする

  if (httpCode > 0){//httpCodeが0より大きいなら、サーバとの通信は正常に行われた。(サーバから返事が返ってきた)
    if (httpCode == HTTP_CODE_OK){//HTTP_CODE_OKはコード200
      payload = http.getString().c_str();
    }//サーバに接続できたけど、404や500などのエラーをキャッチする場合はここにelseで書く

  }//サーバに接続できなかった場合のエラーはここにelseで書く
  

  //M5.Display.print(payload);
  // JSON形式データの読み込み
  DeserializationError error = deserializeJson(jsondata, payload);

  //取得したデータを入れる変数
  const char* iTime;
  int iWeathercode;
  int iMaxtemp;
  int iMintemp;

  if (error) {                                        // JSON形式データ読み込みエラーの場合
    M5.Lcd.print(F("deserializeJson() failed: "));
    M5.Lcd.println(error.f_str());                    // エラーメッセージのディスプレイ表示
  } else {                                            // 正常な場合
    //個別のデータ読み込み部分
    iTime = jsondata["daily"]["time"][0].as<const char*>();
    iWeathercode = jsondata["daily"]["weathercode"][0].as<int>();
    iMaxtemp = jsondata["daily"]["temperature_2m_max"][0].as<int>();
    iMintemp = jsondata["daily"]["temperature_2m_min"][0].as<int>();

    M5.Lcd.println("Can read from JSON Data!");
    M5.Lcd.setTextColor(RED);
    M5.Lcd.println(iTime);
    M5.Lcd.println(iWeathercode);
    M5.Lcd.println(iMaxtemp);
    M5.Lcd.println(iMintemp);
  }

  //
  M5.Lcd.clear();
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(10,20);
  M5.Lcd.setTextSize(5);
  M5.Lcd.print(iTime);


  M5.Lcd.setTextSize(7);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setCursor(50,80);
  M5.Lcd.print(iMaxtemp);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setCursor(120,170);
  M5.Lcd.print(iMintemp);

  //画像を表示
  const char* imgPath;
  if(iWeathercode < 2) imgPath = IMG_PATH1;
  else if(iWeathercode < 4)imgPath = IMG_PATH2;
  else imgPath = IMG_PATH3;

  auto file = SPIFFS.open(imgPath, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 8192);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる

  canvas.createSprite(100, 100);            //100x100サイズのスプライト（表示領域）を作る
  canvas.fillScreen(WHITE);
  canvas.drawPng(gIMGBuf, 8192);            //表示領域に対してgIMGBufの内容を書き込む
  canvas.pushSprite(200, 80);                  //スプライトを指定座標に描画する

  //線を描画3本引いて太くする
  M5.Display.drawLine(159,120,49,200,BLACK);
  M5.Display.drawLine(160,120,50,200,BLACK);
  M5.Display.drawLine(161,120,51,200,BLACK);
}

void loop()
{


}