#include <Arduino.h>

void read_bluetooth()
{
    if (Serial.available() > 0)
    {
        int info = Serial.read();
        int state = 0;
    }
}