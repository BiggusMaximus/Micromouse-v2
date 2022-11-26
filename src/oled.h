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

void display_ultrasonic(float front, float right, float left)
{
    display.setTextColor(WHITE);
    display.setCursor(70, 0);
    display.print("F:" + String(front));
    display.setCursor(70, 10);
    display.print("R:" + String(right));
}