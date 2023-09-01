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
const char* url = "https://api.a3rt.recruit.co.jp/talk/v1/smalltalk"; 

//wifi接続設定
const char* ssid = "APSARA1307";
const char* password = "wlan1024";

//画像表示用
constexpr auto IMG_PATH1 = "/10.png";
constexpr auto IMG_PATH2 = "/20.png";
constexpr auto IMG_PATH3 = "/30.png";

//画像はcanvasという仕組みを使って表示する。
M5Canvas canvas1(&M5.Display);
M5Canvas canvas2(&M5.Display);
M5Canvas canvas3(&M5.Display);

// 画像データを一時保存する配列
//uint8_tは1byteなので、8192byteまで入れられる。ここは画像のデータ量に合わせる。
uint8_t gIMGBuf[4092];


void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  SPIFFS.begin();

  auto file = SPIFFS.open(IMG_PATH1, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 4092);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる
  canvas1.createSprite(100, 100);            //200x200サイズのスプライト（表示領域）を作る
  canvas1.drawPng(gIMGBuf, 4092);            //表示領域に対してgIMGBufの内容を書き込む

  file = SPIFFS.open(IMG_PATH2, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 4092);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる
  canvas2.createSprite(100, 100);            //200x200サイズのスプライト（表示領域）を作る
  canvas2.drawPng(gIMGBuf, 4092);            //表示領域に対してgIMGBufの内容を書き込む


  file = SPIFFS.open(IMG_PATH3, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 4092);                 //ファイルをgIMGBufに展開
  file.close();                            //ファイルを閉じる
  canvas3.createSprite(100, 100);            //200x200サイズのスプライト（表示領域）を作る
  canvas3.drawPng(gIMGBuf, 4092);            //表示領域に対してgIMGBufの内容を書き込む


  http.begin(url);
  http.POST("{\"apikey\": \"DZZjLcL13MsHZHJ58CceofWXaFgiZRyp\",\"query\": \"\"}");
  //実際にデータを送信して、返事をもらうまで待機
  
  WiFi.begin(ssid, password); //Wifiに接続を開始

  //接続が完了するまで無限ループ(SSIDやパスフレーズが間違っているとここで動かなくなる)
  while (WiFi.status() != WL_CONNECTED){  //接続が完了すると、WiFi.status()はWL_CONNECTEDになる
      delay(500);
  }

  
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

  Serial.println(http.getString());			//返事はhttp.getString()で取得する


  }


void loop()
{
  // avatar's face updates in another thread
  // so no need to loop-by-loop rendering//画像を表示

  M5.Display.clear();
  canvas1.pushSprite(0, 0);                  //スプライトを指定座標に描画する
  delay(500);

  M5.Display.clear();
  canvas2.pushSprite(0, 0);                  //スプライトを指定座標に描画する
  delay(500);

  M5.Display.clear();
  canvas3.pushSprite(0, 0);                  //スプライトを指定座標に描画する
  delay(500);

  /*吹き出し表示
   avatar.init(); // start drawing
   avatar.setSpeechFont(&fonts::lgfxJapanGothic_12); //表示するフォントを指定(これをしないと日本語が表示されない)
   avatar.setSpeechText("ねむい…");                 //吹き出しを表示
   avatar.setSpeechText("");                         //吹き出しを消す
	*/

}