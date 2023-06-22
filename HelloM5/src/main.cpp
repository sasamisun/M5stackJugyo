#include <M5Unified.h>

// global variables (define variables to be used throughout the program)
uint32_t count;


// setup function is executed only once at startup.
// This function mainly describes the initialization process.
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

// loop function is executed repeatedly for as long as it is running.
// loop function acquires values from sensors, rewrites the screen, etc.
void loop() {

  M5.Display.setCursor(0, 20);             // set character drawing coordinates (cursor position)
  M5.Display.printf("COUNT: %d\n", count); // display count on screen
  Serial.printf("COUNT: %d\n", count);  // display count serially
  count++;                              // increase count by 1
  delay(1000);                          // wait 1 second(1,000msec)

}