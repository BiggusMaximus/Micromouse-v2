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

void display_selector(uint8_t select, uint8_t kp, uint8_t ki, uint8_t kd)
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
        select++;
    }
    else if (select == 3)
    {
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("kp : " + String(kp));
        display.setCursor(0, 10);
        display.print("ki : " + String(ki));
        display.setTextColor(BLACK, WHITE);
        display.setCursor(0, 20);
        display.print("kd : " + String(kd));
    }
}

void display_ultrasonic(uint8_t front, uint8_t right, uint8_t left)
{
    display.setTextColor(WHITE);
    display.setCursor(50, 0);
    display.print("front : " + String(front));
    display.setCursor(50, 10);
    display.print("right : " + String(right));
    display.setCursor(50, 20);
    display.print("left : " + String(left));
}