#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "functions.h"

#define sensorHeartCapt A0

class oled
{
public:
    vec2 screenDimension;
    Adafruit_SSD1306* pDisp;

    int size;
    vec2 cursorPos;
    uint16_t color;

    oled();
    
private:

};
class heartSensor
{
public:
    oled* screen;
    unsigned long startMillis;
    unsigned long currentMillis;
    unsigned long oldTime;
    bool start;
    bool stop;

    heartSensor();
    void heartBeat();
private:
};

extern heartSensor* heart;

