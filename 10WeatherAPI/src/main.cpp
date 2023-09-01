#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

//前項で計算した、JSONデータのサイズを<>内に書きます。
StaticJsonDocument<768> jsondata;

//HTTP接続用の変数
HTTPClient http;
//API実行結果を入れるための変数
String payload;
//接続先URL
const char* url = "https://api.open-meteo.com/v1/forecast?latitude=35.6895&longitude=139.6917&daily=weathercode,temperature_2m_max,temperature_2m_min&timezone=Asia%2FTokyo&forecast_days=1"; 

//wifi接続設定
const char* ssid = "APSARA1307";
const char* password = "wlan1024";

//画像表示用
constexpr auto IMG_PATH1 = "/10.png";
constexpr auto IMG_PATH2 = "/20.png";
constexpr auto IMG_PATH3 = "/30.png";

//画像はcanvasという仕組みを使って表示する。
M5Canvas canvas(&M5.Display);
// 画像データを一時保存する配列
//uint8_tは1byteなので、8192byteまで入れられる。ここは画像のデータ量に合わせる。
uint8_t gIMGBuf[8192];


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
  M5.Display.print("\r\nWiFi connected\r\nIP address: ");
  M5.Display.println(WiFi.localIP());//接続が完了するとWiFi.localIP()はローカルIPアドレスを返すようになる

  SPIFFS.begin(); //SPIFFSに保存した画像を読みたいので、SPIFFSを使えるようにする。
  
  http.begin(url);          //指定のURLに接続する準備
  int httpCode = http.GET();//HTMLでいうところの、form medhot="GET" でsubmitする

  if (httpCode > 0){//httpCodeが0より大きいなら、サーバとの通信は正常に行われた。(サーバから返事が返ってきた)
    if (httpCode == HTTP_CODE_OK){//HTTP_CODE_OKはコード200
      payload = http.getString();
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

  File file;

  if (error) {                                        // JSON形式データ読み込みエラーの場合
    M5.Lcd.print("deserializeJson() failed: ");
    M5.Lcd.println(error.f_str());                    // エラーメッセージのディスプレイ表示
  } else {                                            // 正常な場合
    //個別のデータ読み込み部分
    //char* timezone = jsondata["timezone"]; timezoneが取得したいときの書き方
    iTime = jsondata["daily"]["time"][0].as<const char*>();//[0]で配列の0番目の日付を指定している
    iWeathercode = jsondata["daily"]["weathercode"][0].as<int>();
    iMaxtemp = jsondata["daily"]["temperature_2m_max"][0].as<int>();
    iMintemp = jsondata["daily"]["temperature_2m_min"][0].as<int>();

    M5.Lcd.println("Can read from JSON Data!");
    M5.Display.fillScreen(DARKCYAN);
    M5.Display.setTextSize(4);
    M5.Display.setCursor(50,30);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println(iTime);
    M5.Display.setTextSize(4);
    M5.Display.setCursor(50,90);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.println(iMaxtemp);
    M5.Display.setTextSize(4);
    M5.Display.setCursor(50,130);
    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.println(iMintemp);
  }
  
  iWeathercode=0;
  switch(iWeathercode) {
    case 0:
    //画像１を表示
  file = SPIFFS.open(IMG_PATH1, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 8192);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる

  canvas.createSprite(100, 100);            //100x100サイズのスプライト（表示領域）を作る
  canvas.fillScreen(DARKCYAN);
  canvas.drawPng(gIMGBuf, 8192);            //表示領域に対してgIMGBufの内容を書き込む
  //canvas.pushSprite(100, 80);                  //スプライトを指定座標に描画する
  canvas.setPivot(0,0);
  canvas.pushRotateZoom(100, 80, 0, 1.5, 1.5);
  break;

  case 1 | 2 | 3:
  //画像２を表示
  file = SPIFFS.open(IMG_PATH2, "r");
  file.read(gIMGBuf, 8192);
  file.close();

  canvas.createSprite(100, 100);
  canvas.fillScreen(DARKCYAN);
  canvas.drawPng(gIMGBuf, 8192);
  canvas.pushSprite(100, 80);

  break;
  case 61 | 63 | 65:
  //画像３を表示
  file = SPIFFS.open(IMG_PATH3, "r");
  file.read(gIMGBuf, 8192);
  file.close();

  canvas.createSprite(100, 100);
  canvas.drawPng(gIMGBuf, 8192);
  canvas.pushSprite(100, 80);
  break;
  default:
  error;
  }
}

void loop()
{

}