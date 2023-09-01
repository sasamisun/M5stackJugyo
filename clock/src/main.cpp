#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h> //時間を処理できるライブラリ


//HTTP接続用の変数
//HTTPClient http;


//接続先URL
const char* url = "ntp.nict.jp";
const long  gmtOffset_sec = 3600 * 9; //世界標準時刻との時間差(９時間)
const int   daylightOffset_sec = 0; //サマータイムはないので0


//wifi接続設定
const char* ssid = "APSARA1307";
const char* password = "wlan1024";

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

  /*
  http.begin(url);          //指定のURLに接続する準備
  int httpCode = http.GET();//HTMLでいうところの、form medhot="GET" でsubmitする
  //サーバのレスポンスが車で処理が止まる
  if (httpCode > 0){//httpCodeが0より大きいなら、サーバとの通信は正常に行われた。(サーバから返事が返ってきた)
    if (httpCode == HTTP_CODE_OK){//HTTP_CODE_OKはコード200
      payload = http.getString();
    }//サーバに接続できたけど、404や500などのエラーをキャッチする場合はここにelseで書く

  }//サーバに接続できなかった場合のエラーはここにelseで書く
  */
 //
  //URLから時間を取得する動きはconfigTimeメソッド内でやってくれます。
  //取得したデータは、M5stack内のシステム時間に保存されます。
  configTime(gmtOffset_sec, daylightOffset_sec, url);

  /*  struct について
    structは構造体を表します。構造体とは、変数の中に変数が入っているものだと思ってください。
    この場合、tm型で宣言した変数timeinfoのなかに、tm_sec、tm_min、tm_hourなどの時間を表す変数が入っています。
  */
  struct tm timeinfo;
  //変数timeinfo内の各変数に、システム時間の内容を代入します。
  getLocalTime(&timeinfo);

  // テキストサイズ指定
  M5.Lcd.setTextSize(2);
  // カーソル位置を設定
  M5.Lcd.setCursor(40,100);
  M5.Lcd.printf("%04d-%02d-%02d %02d:%02d:%02d" 
                ,timeinfo.tm_year + 1900
                ,timeinfo.tm_mon + 1
                ,timeinfo.tm_mday
                ,timeinfo.tm_hour
                ,timeinfo.tm_min
                ,timeinfo.tm_sec
                );

}

void loop()
{

/*
システム時間に時間を設定すると、常に現在時刻を更新してくれるので、
新しい時間を表示させるときにはサーバに接続する必要はありません。
getLocalTime(&timeinfo);で新しい時間を取得できます。
*/
}