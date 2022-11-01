#include <Arduino.h>
#include "ultrasonic.h"
#include "oled.h"
// #include "gyroscope.h"
#include "button.h"
const uint8_t trig1 = 2;
const uint8_t trig2 = 10;
const uint8_t trig3 = 12;
const uint8_t echo1 = 3;
const uint8_t echo2 = 11;
const uint8_t echo3 = 13;
uint8_t i;
uint32_t kd = 0;
uint32_t ki = 0;
uint32_t kp = 0;
float front, left, right;

void setup()
{
  Serial.begin(115200);
  start_display();
  // set_mpu();
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
  for (i = 0; i < noOfButtons; ++i)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop()
{
  display.clearDisplay();
  select_channel();
  if (counter[0] == 1)
  {
    kp = control_value(kp, 1);
  }
  else if (counter[0] == 2)
  {
    ki = control_value(ki, 1);
  }
  else
  {
    kd = control_value(kd, 1);
  }
  display_selector(counter[0], kp, ki, kd);

  front = read_ultrasonic(trig1, echo1);
  right = read_ultrasonic(trig2, echo2);
  left = read_ultrasonic(trig3, echo3);
  display_ultrasonic(front, right, left);
  display.display();
}
