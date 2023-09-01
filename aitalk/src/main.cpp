#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//前項で計算した、JSONデータのサイズを<>内に書きます。
StaticJsonDocument<200> jsondata;

//HTTP接続用の変数
HTTPClient http;
//API実行結果を入れるための変数
String payload;
//接続先URL
const char* url = "https://api.a3rt.recruit.co.jp/talk/v1/smalltalk"; 
//POSTするデータ

//wifi接続設定
const char* ssid = "APSARA1307";
const char* password = "wlan1024";

//質問文
String question="目玉焼きには何をかける？";
//POSTするデータ
String data = "apikey=DZZTIfsC88xfXVZXAJdjuIiv19V5CgPq&query=目玉焼きには何をかける？";

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.clear();
  
  M5.Display.setFont(&fonts::lgfxJapanMincho_12); //日本語フォント
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
  
  http.begin(url);          //指定のURLに接続する準備
  //int httpCode = http.GET();//HTMLでいうところの、form medhot="GET" でsubmitする
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");     //M5stack から送信するデータの形式
  int httpCode = http.POST(data);			//実際にデータを送信して、返事をもらうまで待機

  if (httpCode > 0){//httpCodeが0より大きいなら、サーバとの通信は正常に行われた。(サーバから返事が返ってきた)
    if (httpCode == HTTP_CODE_OK){//HTTP_CODE_OKはコード200
      payload = http.getString();
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

	//取得したデータを入れる変数
  const char* rep;

  if (error) {                                        // JSON形式データ読み込みエラーの場合
    M5.Display.print("deserializeJson() failed: ");
    M5.Display.println(error.f_str());                    // エラーメッセージのディスプレイ表示
  } else {                                            // 正常な場合
    //個別のデータ読み込み部分
    //char* timezone = jsondata["timezone"]; timezoneが取得したいときの書き方
    rep = jsondata["results"][0]["reply"].as<const char*>();

    M5.Lcd.println("Can read from JSON Data!");
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(WHITE);

    M5.Display.print("You:");
    M5.Display.println(question);
    
    M5.Display.print("Ai:");
    M5.Display.println(rep);
  }
  
}

void loop()
{

}