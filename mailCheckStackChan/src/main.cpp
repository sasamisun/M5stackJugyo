#include <M5Unified.h>
#include <ServoEasing.hpp>
#include <Avatar.h>
#include <SD.h>     
#include <AquesTalkTTS.h>
#include <tasks/LipSync.h>    // セリフに合わせてアバターの口を動かすプログラムを読み込みます。

#include <HTTPClient.h>
#include <ArduinoJson.h>

//二つのサーボモーターのピン番号をdefineしておきます。
#define SERVO_PIN_X 5
#define SERVO_PIN_Y 2
#define START_DEGREE_VALUE_X 90
#define START_DEGREE_VALUE_Y 90
ServoEasing servo_x;
ServoEasing servo_y;

using namespace m5avatar;
Avatar avatar;

float *gazeVp,*gazeHp;
float gazeV,gazeH;

// AquesTalkのライセンスキーを入力します。ライセンスがある場合はライセンスキーを入力します。
// ライセンスキーがない場合は適当な文字列を入力します。
const char* AQUESTALK_KEY = "XXXX-XXXX-XXXX-XXXX";

uint32_t lastMailCheck = 0;
uint32_t mailCheckInterval = 1000*120;

//json
StaticJsonDocument<192> jsondata;
//HTTP接続用の変数
HTTPClient http;
//API実行結果を入れるための変数
String payload;
//接続先URL
const char* url = "https://script.google.com/macros/s/AKfycbxnI6nfbLodAUOyaYof1jFdH5Uflusby_q7IL8JMHL3ul1G2J_NKdwAO1c52CkV4GY/exec";

//wifi接続設定
const char* ssid = "APSARA1307";
const char* password = "wlan1024";

void mailCheck(){
  if(millis() - lastMailCheck > mailCheckInterval){
    avatar.setSpeechText("メールチェック！");
    int httpCode = http.GET();//HTMLでいうところの、form medhot="GET" でsubmitする

    if (httpCode > 0){//httpCodeが0より大きいなら、サーバとの通信は正常に行われた。(サーバから返事が返ってきた)
      if (httpCode == HTTP_CODE_OK){//HTTP_CODE_OKはコード200
        payload = http.getString();
      }//サーバに接続できたけど、404や500などのエラーをキャッチする場合はここにelseで書く

    }//サーバに接続できなかった場合のエラーはここにelseで書く
    
    //M5.Display.print(payload);
    // JSON形式データの読み込み
    Serial.print(payload);
    M5.Display.print(payload);
    DeserializationError error = deserializeJson(jsondata, payload);

    //取得したデータを入れる変数
    const char* id;
    const char* subject;
    const char* content;

    if (error) {                                        // JSON形式データ読み込みエラーの場合
      M5.Lcd.print("deserializeJson() failed: ");
      M5.Lcd.println(error.f_str());                    // エラーメッセージのディスプレイ表示
      avatar.setSpeechText("デシリアライズエラー");
    } else {                                            // 正常な場合
      //個別のデータ読み込み部分
      //char* timezone = jsondata["timezone"]; timezoneが取得したいときの書き方
      id = jsondata["id"].as<const char*>();
      subject = jsondata["sub"].as<const char*>();
      content = jsondata["body"].as<const char*>();
      avatar.setSpeechText(subject);
    }
    delay(5000);
    avatar.setSpeechText("");
  }
}

void setup()
{
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  M5.begin(cfg);

  SD.begin(GPIO_NUM_4);
  TTS.createK(AQUESTALK_KEY);          // アバターを喋らせるためのクラスを定義します。
  
  //wifi setting
  M5.Display.clear();
  
  M5.Display.setCursor(0,0);
  M5.Display.print("Wifi connecting");

  WiFi.begin(ssid, password); //Wifiに接続を開始

  //接続が完了するまで無限ループ(SSIDやパスフレーズが間違っているとここで動かなくなる)
  while (WiFi.status() != WL_CONNECTED){  //接続が完了すると、WiFi.status()はWL_CONNECTEDになる
      delay(500);
      M5.Lcd.print('.');
  }
  http.begin(url);          //指定のURLに接続する準備

  M5.Display.clear();
  M5.Display.setCursor(0,0);
  M5.Display.print("\r\nWiFi connected\r\nIP address: ");
  M5.Display.println(WiFi.localIP());//接続が完了するとWiFi.localIP()はローカルIPアドレスを返すようになる
  delay(2000);
  M5.Display.clear();

  // attach(int aPin, int aInitialDegreeOrMicrosecond, int aMicrosecondsForServo0Degree, int aMicrosecondsForServo180Degree)
  //ピン番号、初期角度、最低ON時間(マイクロ秒)、最長ON時間(マイクロ秒)を設定します。
  servo_x.attach(SERVO_PIN_X,
                 START_DEGREE_VALUE_X,
                 DEFAULT_MICROSECONDS_FOR_0_DEGREE,
                 DEFAULT_MICROSECONDS_FOR_180_DEGREE);
  servo_y.attach(SERVO_PIN_Y,
                 START_DEGREE_VALUE_Y-20,
                 DEFAULT_MICROSECONDS_FOR_0_DEGREE,
                 DEFAULT_MICROSECONDS_FOR_180_DEGREE);
  //両方のサーボモーターにイージングを設定
  servo_x.setEasingType(EASE_QUADRATIC_IN_OUT);
  servo_y.setEasingType(EASE_QUADRATIC_IN_OUT);
  //モーターの動作速度設定
  setSpeedForAllServos(5);



  avatar.init(); // start drawing
  avatar.addTask(lipSync, "lipSync"); // セリフに合わせて口を動かすよう設定します。
  
  
    avatar.setSpeechFont(&fonts::lgfxJapanGothicP_8); //表示するフォントを指定(これをしないと日本語が表示されない)
    //TTS.playK("起動したよ", 100);
    //avatar.setSpeechText("やっぽー");                 //吹き出しを表示
    gazeV=0;
    gazeVp = &gazeV;
    gazeH=0;
    gazeHp = &gazeH;
    avatar.getGaze(gazeVp,gazeHp);
    
    avatar.setSpeechText("スタート");

    TTS.playK("スタート",100);
    delay(1000);
    avatar.setSpeechText("");
}

void loop()
{
  
  M5.update();

  //mailCheck();
  //目線に応じて動く
  avatar.getGaze(gazeVp,gazeHp);
  if(gazeV < -0.5){
    servo_y.setEaseTo(45, 10);
  }else{
    servo_y.setEaseTo(60, 10);
  }
  if(gazeH < -0.5){
    servo_x.setEaseTo(60, 10);
  }else if(gazeH > 0.5){
    servo_x.setEaseTo(120, 10);
  }else{
    servo_x.setEaseTo(90, 10);
  }
  synchronizeAllServosStartAndWaitForAllServosToStop();

  if(M5.BtnA.wasPressed()){
    avatar.setSpeechText("お疲れ様でした！");
    TTS.playK("お疲れ様でした",100);
  }

}