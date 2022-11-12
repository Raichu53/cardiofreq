#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "functions.h"

#define sensorHeartCapt A0
#define clockPin 6
#define dataPin 7
#define resetPin 8
#define buttonPin 3
#define redLed 12
#define yellowLed 11
#define greenLed 10

class clock
{
public:
    unsigned char buffer;  

    clock();
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

    bool bButton;
    bool buffer;
    oled();
    bool isButtonPressed();
    void drawBlackScreen();
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
    int bpm;

    heartSensor();
    void heartBeat();
    void healthLeds();
private:
};

extern heartSensor* heart;
extern clock* horloge;
