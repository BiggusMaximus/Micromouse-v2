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
#define noOfButtons 3
#define bounceDelay 20
#define minButtonPress 3

const int buttonPins[] = {A0, A1, A2};
uint32_t previousMillis[noOfButtons];
uint8_t pressCount[noOfButtons];
uint8_t counter[noOfButtons];
float size[6] = {0.01, 0.1, 1, 5, 10, 100};
int select_size = 0;
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void start_display()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    display.display();
    delay(2000);

    display.clearDisplay();
}

void display_selector(uint16_t select, float kp, float ki, float kd, float size_number)
{

    display.setTextSize(1);

    if (select == 1)
    {
        display.setTextColor(BLACK, WHITE);
        display.setCursor(0, 0);
        display.print("kp : " + String(kp));
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.print("ki : " + String(ki));
        display.setCursor(0, 20);
        display.print("kd : " + String(kd));
        display.setCursor(70, 20);
        display.print("S:" + String(size_number));
        select++;
    }
    else if (select == 2)
    {
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("kp : " + String(kp));
        display.setTextColor(BLACK, WHITE);
        display.setCursor(0, 10);
        display.print("ki : " + String(ki));
        display.setTextColor(WHITE);
        display.setCursor(0, 20);
        display.print("kd : " + String(kd));
        display.setCursor(70, 20);
        display.print("S:" + String(size_number));
        select++;
    }
    else if (select == 3)
    {
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("kp : " + String(kp));
        display.setCursor(0, 10);
        display.print("ki : " + String(ki));
        display.setCursor(70, 20);
        display.print("S:" + String(size_number));
        display.setTextColor(BLACK, WHITE);
        display.setCursor(0, 20);
        display.print("kd : " + String(kd));
    }
    else if (select == 4)
    {
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("kp : " + String(kp));
        display.setCursor(0, 10);
        display.print("ki : " + String(ki));
        display.setCursor(0, 20);
        display.print("kd : " + String(kd));
        display.setTextColor(BLACK, WHITE);
        display.setCursor(70, 20);
        display.print("S:" + String(size_number));
    }
}
#include <Arduino.h>
#define dir1 12
#define pwm1 10
#define pwm2 11
#define dir2 13

int pwmA;
int pwmB;
int pwmKa;
int pwmKi;
int Upper = 255;
int Lower = 15;
int basePWM = 50;

float sp = 5.00;
float dt = 0.001;
float error, integralE, derivativeE, lastError;
float PID;

void start_dcMotor()
{
    pinMode(dir1, OUTPUT);
    pinMode(pwm1, OUTPUT);
    pinMode(dir2, OUTPUT);
    pinMode(pwm2, OUTPUT);
}

void pid(float Kp, float Ki, float Kd, float depan, float kanan)
{
    if (kanan > 20)
    {
        kanan = 20;
    }
    else
    {
        kanan = kanan;
    }

    if (depan <= 4)
    {
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, LOW);
        analogWrite(pwm1, 155);
        analogWrite(pwm2, 100);
    }

    else
    {
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, HIGH);

        error = sp - kanan;
        integralE = integralE + error;
        derivativeE = error - lastError;

        float P = Kp * error;
        float I = (Ki * integralE) * dt;
        float D = (Kd / dt) * derivativeE;

        lastError = error;

        PID = P + I + D;

        pwmA = basePWM + PID;
        if (pwmA > Upper)
        {
            pwmA = Upper;
        }
        if (pwmA < Lower)
        {
            pwmA = Lower;
        }
        pwmKa = pwmA;

        pwmB = basePWM - PID;
        if (pwmB > Upper)
        {
            pwmB = Upper;
        }
        if (pwmB < Lower)
        {
            pwmB = Lower;
        }
        pwmKi = pwmB;

        analogWrite(pwm1, pwmKa);
        analogWrite(pwm2, pwmKi);
    }
}
#include <Arduino.h>

float read_ultrasonic(int trigPin, int echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(echoPin, INPUT);
    long duration = pulseIn(echoPin, HIGH);

    // Convert the time into a distance
    float cm = (duration / 2.0) / 29.1;
    return cm;
}

void display_ultrasonic(float front, float right, float left)
{
    display.setTextColor(WHITE);
    display.setCursor(70, 0);
    display.print("F:" + String(front));
    display.setCursor(70, 10);
    display.print("R:" + String(right));
}

void select_channel()
{
    long currentMillis = millis();

    // If select channel not pressed
    if (digitalRead(buttonPins[0]))
    {
        previousMillis[0] = currentMillis;
        pressCount[0] = 0;
    }
    // If select channel pressed
    else
    {
        if (currentMillis - previousMillis[0] > bounceDelay)
        {
            previousMillis[0] = currentMillis;
            ++pressCount[0];
            if (pressCount[0] == minButtonPress)
            {
                counter[0]++;
            }
        }
    }
    if (counter[0] > 4)
    {
        counter[0] = 1;
    }
    else if (counter[0] == 0)
    {
        counter[0] = 1;
    }
}

float control_value(float val, float size)
{
    // If plus button not pressed
    long currentMillis = millis();
    if (digitalRead(buttonPins[1]))
    {
        previousMillis[1] = currentMillis;
        pressCount[1] = 0;
    }
    // If plus button pressed
    else
    {
        if (currentMillis - previousMillis[1] > bounceDelay)
        {
            previousMillis[1] = currentMillis; // Set previousMillis to millis to reset timeout
            ++pressCount[1];
            if (pressCount[1] == minButtonPress)
            // add size
            {
                val = val + size;
            }
        }
    }
    if (digitalRead(buttonPins[2]))
    {
        previousMillis[2] = currentMillis;
        pressCount[2] = 0;
    }
    else
    {
        if (currentMillis - previousMillis[2] > bounceDelay)
        {
            previousMillis[2] = currentMillis; // Set previousMillis to millis to reset timeout
            ++pressCount[2];
            if (pressCount[2] == minButtonPress)
            {
                val = val - size;
            }
        }
    }
    return val;
}

float change_size()
{
    // If plus button not pressed
    long currentMillis = millis();
    if (digitalRead(buttonPins[1]))
    {
        previousMillis[1] = currentMillis;
        pressCount[1] = 0;
    }
    // If plus button pressed
    else
    {
        if (currentMillis - previousMillis[1] > bounceDelay)
        {
            previousMillis[1] = currentMillis; // Set previousMillis to millis to reset timeout
            ++pressCount[1];
            if (pressCount[1] == minButtonPress)
            // add size
            {
                select_size = select_size + 1;
            }
        }
    }
    if (digitalRead(buttonPins[2]))
    {
        previousMillis[2] = currentMillis;
        pressCount[2] = 0;
    }
    else
    {
        if (currentMillis - previousMillis[2] > bounceDelay)
        {
            previousMillis[2] = currentMillis; // Set previousMillis to millis to reset timeout
            ++pressCount[2];
            if (pressCount[2] == minButtonPress)
            {
                select_size = select_size - 1;
            }
        }
    }
    if (select_size > 5)
    {
        select_size = 0;
    }
    else if (select_size < 0)
    {
        select_size = 0;
    }
    return size[select_size];
}
void read_bluetooth()
{
    if (Serial.available() > 0)
    {
        int info = Serial.read();
        int state = 0;
    }
}

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
    if ((millis() - lastTime) > 500)
    {
        front = read_ultrasonic(trig2, echo2);
        right = read_ultrasonic(trig1, echo1);
        lastTime = millis();
    }
    display_selector(counter[0], kp, ki, kd, size_number);
    pid(kp, ki, kd, front, right);
    display_ultrasonic(front, right, 0);
    display.display();
    Serial.println("Kp:" + String(kp) + ",Ki:" + String(ki) + ",Kd:" + String(kd) + ",F:" + String(front) + ",R:" + String(right));
    read_bluetooth();
}
