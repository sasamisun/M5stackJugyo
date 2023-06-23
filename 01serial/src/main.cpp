#include <M5Unified.h>
uint32_t count;

void setup() {

  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;   // default=115200. if "Serial" is not needed, set it to 0.
  M5.begin(cfg);

  Serial.println("serial start.");
  count = 0;
}

void loop() {

  M5.Display.setCursor(0, 20);
  M5.Display.printf("COUNT: %d\n", count); // display count on screen
  while (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    count++;
    Serial.println(count);
    M5.Display.println(command);
  }

}