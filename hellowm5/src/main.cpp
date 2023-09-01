#include <M5Unified.h>

//ここで宣言した変数はファイル内のどのメソッドからでも参照できます。
uint32_t count;


// setup メソッドはプログラム実行時に最初の１回だけ自動的に実行されます。
void setup() {

  auto cfg = M5.config(); // Assign a structure for initializing M5Stack
  // If config is to be set, set it here
  // Example.
  // cfg.external_spk = true;

  M5.begin(cfg);                        // initialize M5 device

  M5.Display.setTextSize(3);            // change text size
  M5.Display.print("Hello World!!!") ;  // display Hello World! and one line is displayed on the screen
  Serial.println("Hello World!!!") ;    // display Hello World! and one line on the serial monitor
  count = 0;                            // initialize count

}

// loop メソッドはプログラムが終了するまで無限に繰り返します。
void loop() {
  int a=0;
  M5.Display.setCursor(0, 30);             // set character drawing coordinates (cursor position)
  M5.Display.printf("COUNT: %d\n", count); // display count on screen
  Serial.printf("COUNT: %d\n", count);  // display count serially
  count++;                              // increase count by 1
  delay(1000);                          // wait 1 second(1,000msec)

}