#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "functions.h"

#define sensorHeartCapt A0
#define clockPin 6
#define dataPin 7
#define resetPin 8

class clock
{
public:
    unsigned char buffer;  

    clock(bool init);
    bool initClock(); //call once
    unsigned char readRegister(unsigned char addr);
    void writeRegister(unsigned char addr,unsigned char data);
    void afficheHeure();
private:
};
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
extern clock* horloge;
