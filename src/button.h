#define noOfButtons 4
#define bounceDelay 20
#define minButtonPress 3

const int buttonPins[] = {A0, A1, A2, A3};
uint32_t previousMillis[noOfButtons];
uint8_t pressCount[noOfButtons];
uint8_t counter[noOfButtons];

void select_channel()
{
    long currentMillis = millis();
    if (digitalRead(buttonPins[0]))
    {
        previousMillis[0] = currentMillis;
        pressCount[0] = 0;
    }
    else
    {
        if (currentMillis - previousMillis[0] > bounceDelay)
        {
            previousMillis[0] = currentMillis; // Set previousMillis to millis to reset timeout
            ++pressCount[0];
            if (pressCount[0] == minButtonPress)
            {
                counter[0]++;
            }
        }
    }
    if (counter[0] > 3)
    {
        counter[0] = 1;
    }
    else if (counter[0] == 0)
    {
        counter[0] = 1;
    }
}

int control_value(uint16_t val, uint16_t size)
{
    long currentMillis = millis();
    if (digitalRead(buttonPins[1]))
    {
        previousMillis[1] = currentMillis;
        pressCount[1] = 0;
    }
    else
    {
        if (currentMillis - previousMillis[1] > bounceDelay)
        {
            previousMillis[1] = currentMillis; // Set previousMillis to millis to reset timeout
            ++pressCount[1];
            if (pressCount[1] == minButtonPress)
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
