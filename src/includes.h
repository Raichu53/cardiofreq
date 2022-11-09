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

    const char* txt;
    int size;
    vec2 cursorPos;
    uint16_t color;

    oled();
    void printText();
    
private:

};

extern oled* screen;
extern unsigned long currentMillis;
extern unsigned long oldTime;
extern bool start;
extern bool stop;

