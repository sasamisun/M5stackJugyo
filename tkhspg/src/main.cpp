#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <SD.h>           
#include <AquesTalkTTS.h> 


//前項で計算した、JSONデータのサイズを<>内に書きます。
StaticJsonDocument<300> jsondata;

//HTTP接続用の変数
HTTPClient http;
//API実行結果を入れるための変数
String payload;
//接続先URL
const char* url = "https://api.a3rt.recruit.co.jp/talk/v1/smalltalk"; 

//wifi接続設定
const char* ssid = "APSARA1307";
const char* password = "wlan1024";

//POSTするデータ
String data = "apikey=DZZTIfsC88xfXVZXAJdjuIiv19V5CgPq&query=";

//AquesTalkライセンスキー
const char* AQUESTALK_KEY = "XXXX-XXXX-XXXX-XXXX";

static unsigned long millis_buf = 0;//追加分

//画像表示用
constexpr auto IMG_PATH1 = "/01.png";
constexpr auto IMG_PATH2 = "/02.png";
constexpr auto IMG_PATH3 = "/03.png";
constexpr auto IMG_PATH4 = "/01.png";
constexpr auto IMG_PATH5 = "/02.png";
constexpr auto IMG_PATH6 = "/03.png";
constexpr auto IMG_PATH7 = "/01.png";

//画像はcanvasという仕組みを使って表示する。
M5Canvas canvas1(&M5.Display);
M5Canvas canvas2(&M5.Display);
M5Canvas canvas3(&M5.Display);
M5Canvas canvas4(&M5.Display);
M5Canvas canvas5(&M5.Display);
M5Canvas canvas6(&M5.Display);
M5Canvas canvas7(&M5.Display);

// 画像データを一時保存する配列
//uint8_tは1byteなので、1300byteまで入れられる。ここは画像のデータ量に合わせる。
uint8_t gIMGBuf[6000];

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.clear();


/*以下Wi-Fi接続*/
  WiFi.begin(ssid, password); //Wifiに接続を開始

  //接続が完了するまで無限ループ(SSIDやパスフレーズが間違っているとここで動かなくなる)
  while (WiFi.status() != WL_CONNECTED){  //接続が完了すると、WiFi.status()はWL_CONNECTEDになる
      delay(500);
      M5.Lcd.print('.');
  }

  Serial.print("\r\nWiFi connected\r\nIP address: ");
  Serial.println(WiFi.localIP());//接続が完了するとWiFi.localIP()はローカルIPアドレスを返すようになる


/*以下画像表示*/

  SPIFFS.begin(); //SPIFFSに保存した画像を読みたいので、SPIFFSを使えるようにする。
  auto file = SPIFFS.open(IMG_PATH1, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 1000);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる
  canvas1.createSprite(100, 100);            //200x200サイズのスプライト（表示領域）を作る
  canvas1.drawPng(gIMGBuf, 1000);            //表示領域に対してgIMGBufの内容を書き込む

  file = SPIFFS.open(IMG_PATH2, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 1000);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる
  canvas2.createSprite(100, 100);            //200x200サイズのスプライト（表示領域）を作る
  canvas2.drawPng(gIMGBuf, 1000);            //表示領域に対してgIMGBufの内容を書き込む

  file = SPIFFS.open(IMG_PATH3, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 1000);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる
    canvas3.createSprite(100, 100);            //200x200サイズのスプライト（表示領域）を作る
  canvas3.drawPng(gIMGBuf, 1000);            //表示領域に対してgIMGBufの内容を書き込む
/*
  file = SPIFFS.open(IMG_PATH4, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 1000);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる
  canvas4.createSprite(60, 30);            //200x200サイズのスプライト（表示領域）を作る
  canvas4.drawPng(gIMGBuf, 1000);            //表示領域に対してgIMGBufの内容を書き込む

file = SPIFFS.open(IMG_PATH5, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 1000);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる
  canvas5.createSprite(60, 60);            //200x200サイズのスプライト（表示領域）を作る
  canvas5.drawPng(gIMGBuf, 1000);            //表示領域に対してgIMGBufの内容を書き込む

file = SPIFFS.open(IMG_PATH6, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 20000);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる
  canvas6.createSprite(60, 60);            //200x200サイズのスプライト（表示領域）を作る
  canvas6.drawPng(gIMGBuf, 2000);            //表示領域に対してgIMGBufの内容を書き込む

file = SPIFFS.open(IMG_PATH7, "r");  //ファイル名から画像ファイルを開く
  file.read(gIMGBuf, 2000);                 //ファイルをgIMGBufに展開
  file.close();                             //ファイルを閉じる
  canvas7.createSprite(100, 100);            //200x200サイズのスプライト（表示領域）を作る
  canvas7.drawPng(gIMGBuf, 2000);            //表示領域に対してgIMGBufの内容を書き込む
*/

/*しゃべるやつ*/

  SD.begin(GPIO_NUM_4);
  TTS.createK(AQUESTALK_KEY);          // アバターを喋らせるためのクラスを定義します。
  //TTS.playK("開始するよ", 100);

}


void loop()
{
  // avatar's face updates in another thread
  // so no need to loop-by-loop rendering//画像を表示
  const char* rep;//取得したデータを入れる変数
  


  canvas1.pushSprite(105, 60);                  //スプライトを指定座標に描画する
  delay(500);

  canvas1.pushSprite(105, 65);                  //スプライトを指定座標に描画する
  delay(500);

  canvas1.pushSprite(105, 70);                  //スプライトを指定座標に描画する
  delay(500);

  canvas1.pushSprite(105, 65);                  //スプライトを指定座標に描画する
  delay(500);
/*
M5.update();
if(M5.BtnA.wasPressed()){
 canvas2.pushSprite(50, 50);  
 delay(350);

 canvas2.pushSprite(210, 100);
 delay(800);

 M5.Display.fillRect(50,50,50,80,BLACK);
 M5.Display.fillRect(210,100,50,80,BLACK);

 canvas3.pushSprite(110, 60);
 delay(2000);

 M5.Display.fillRect(110,60,100,100,BLACK);
 delay(2000);
}

if(M5.BtnB.wasPressed()){
//キャンディー
 canvas4.pushSprite(40, 90);
 delay(2000);
 M5.Display.fillRect(40,90,60,30,BLACK);

 M5.Display.setFont(&fonts::lgfxJapanMincho_12); 
 M5.Display.setCursor(40,30);
 M5.Display.setTextSize(3); 
 M5.Display.setTextColor(WHITE); 
 M5.Display.print("わーい");
 delay(2000);
 M5.Display.fillRect(40,30,110,40,BLACK);

//キラキラ1
 canvas5.pushSprite(40, 70);
 canvas5.pushSprite(180, 35);
 canvas5.pushSprite(210, 100);
 delay(500);
 M5.Display.fillRect(40,70,60,60,BLACK);
 M5.Display.fillRect(180,35,60,60,BLACK);
 M5.Display.fillRect(210,100,60,60,BLACK);

 //キラキラ2
 canvas6.pushSprite(40, 70);
 canvas6.pushSprite(180, 35);
 canvas6.pushSprite(210, 100);
 delay(500);
 M5.Display.fillRect(40,70,60,60,BLACK);
 M5.Display.fillRect(180,35,60,60,BLACK);
 M5.Display.fillRect(210,100,60,60,BLACK);
}


if(M5.BtnC.wasPressed()){
 canvas7.pushSprite(100, 65);  
 delay(1000);
 M5.Display.fillRect(100,65,100,100,BLACK);
 delay(3000);
}
  */
  while (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    String senddata = data + command;

    http.begin(url);          //指定のURLに接続する準備
    //int httpCode = http.GET();//HTMLでいうところの、form medhot="GET" でsubmitする
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");     //M5stack から送信するデータの形式

    int httpCode = http.POST(senddata);
    //Serial.println(httpCode);

    if (httpCode > 0){//httpCodeが0より大きいなら、サーバとの通信は正常に行われた。(サーバから返事が返ってきた)
      if (httpCode == HTTP_CODE_OK){//HTTP_CODE_OKはコード200
        payload = http.getString();
        Serial.println(payload);
      }else{
        M5.Display.print("Error code:");
        M5.Display.println(httpCode);
      }

    }else{
      M5.Display.println("Can't connect");
    }

    //M5.Display.print(payload);
    // JSON形式データの読み込み
    DeserializationError error = deserializeJson(jsondata, payload);

    if (error) {                                        // JSON形式データ読み込みエラーの場合
      M5.Display.print("deserializeJson() failed: ");
      M5.Display.println(error.f_str());                    // エラーメッセージのディスプレイ表示
    } else {                                            // 正常な場合
      //個別のデータ読み込み部分
      //char* timezone = jsondata["timezone"]; timezoneが取得したいときの書き方
      rep = jsondata["results"][0]["reply"].as<const char*>();
      M5.Display.setFont(&fonts::lgfxJapanMincho_12);
      M5.Display.setCursor(10,20);
      M5.Display.setTextSize(3);  //文字の大きさを指定
      M5.Display.setTextColor(WHITE);  //文字の色
      M5.Display.println(rep);
      Serial.println(rep);
      //TTS.playK(rep, 100);
      // 上の待機から抜けた時間を格納
      millis_buf = millis();
      //delay(2000);
    }
  }
  if ((millis() - millis_buf) >= 2000)//追加分
  {
    M5.Display.clear();
  }

}