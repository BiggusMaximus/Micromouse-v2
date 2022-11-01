#include <Arduino.h>
#include "display.h"

void setup()
{
  Serial.begin(115200);
  start_display();
}

void loop()
{
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0, 0);
  display.print("TEST ");
  Serial.println("Test");
  display.display();
}
