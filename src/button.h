#define noOfButtons 3
#define bounceDelay 20
#define minButtonPress 3

const int buttonPins[] = {A0, A1, A2};
uint32_t previousMillis[noOfButtons];
uint8_t pressCount[noOfButtons];
uint8_t counter[noOfButtons];
float size[6] = {0.01, 0.1, 1, 5, 10, 100};
int select_size = 0;

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