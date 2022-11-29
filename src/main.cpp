#include "bluetooth.h"
#include "pid.h"
#include "ultrasonic.h"
#include "oled.h"
#include "button.h"

const uint8_t trig1 = 3;
const uint8_t trig2 = 5;
// const uint8_t trig3 = 13;
const uint8_t echo1 = 2;
const uint8_t echo2 = 6;
// const uint8_t echo3 = 12;
uint8_t i;
int val;
int val1;
int val2;
float kd;
float ki;
float kp;
float size_number;
float front, left, right;
unsigned long timerDelay = 3000;
unsigned long lastTime = 0;
bool pid_status = true;

void debugging(String val)
{
  if ((millis() - lastTime) > timerDelay)
  {
    if (val == "all")
    {
      val = digitalRead(A0);
      val1 = digitalRead(A1);
      val2 = digitalRead(A2);
      Serial.println(String(val) + String(val1) + String(val2));
      Serial.println(
          "Front : " + String(front) +
          "Right : " + String(right) +
          "Left : " + String(left));
    }
    else if (val == "ultrasonic")
    {
      Serial.println(
          "Front : " + String(front) +
          "Right : " + String(right) +
          "Left : " + String(left));
    }
    else if (val == "pushbutton")
    {
      val = digitalRead(A0);
      val1 = digitalRead(A1);
      val2 = digitalRead(A2);
      Serial.println(String(val) + String(val1) + String(val2));
    }
    else if (val == "pid")
    {
      Serial.println("size : " + String(size_number) + ", Kp : " + String(kp) + ", Ki : " + String(ki) + ", Kd : " + String(kd));
    }
    else if (val == "send")
    {
      Serial.println("Kp:" + String(kp) + ",Ki:" + String(ki) + ",Kd:" + String(kd) + ",F:" + String(front) + ",R:" + String(right));
    }
    lastTime = millis();
  }
}

void setup()
{
  Serial.begin(9600);
  start_display();
  start_dcMotor();
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  // pinMode(trig3, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  // pinMode(echo3, INPUT);
  for (i = 0; i < noOfButtons; ++i)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  size_number = change_size();
}

void loop()
{
  // counter[0] => select channel

  display.clearDisplay();
  select_channel();
  if (counter[0] == 1)
  {
    kp = control_value(kp, size_number);
  }
  else if (counter[0] == 2)
  {
    ki = control_value(ki, size_number);
  }
  else if (counter[0] == 3)
  {
    kd = control_value(kd, size_number);
  }
  else
  {
    size_number = change_size();
  }
  front = read_ultrasonic(trig2, echo2);
  right = read_ultrasonic(trig1, echo1);
  
  byte pid_button = set_pid();
  if(pid_button == 1){
    pid(kp, ki, kd, front, right);
    Serial.println(right);
  }else{
    stop_motor();
  }

  display_selector(counter[0], kp, ki, kd, size_number);
  display_ultrasonic(front, right, 0);
  display.display();
  read_bluetooth();
}
