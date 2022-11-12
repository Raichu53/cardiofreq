#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Tone.h>
#include <Adafruit_ILI9341.h> //pour les couleurs

#define sensorHeartCapt A0
#define clockPin 6
#define dataPin 7
#define resetPin 8
#define buttonPin 3
#define redLed 12
#define yellowLed 11
#define greenLed 10
#define buzzerPin 5

typedef struct vector2{
    int x,y;
}vec2;

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
    Adafruit_SSD1306* pDisp;

    vec2 screenDimension,cursorPos;

    bool bButton,pressed,press,doublePressed,toggleScreen;

    unsigned long delayMax,doublePush;

    oled();
    int isButtonPressed();
    void drawBlackScreen();
private:

};
class heartSensor
{
public:
    oled* screen;
    clock* horloge;
    unsigned long startMillis,currentMillis,oldTime,lastBeep;

    bool start;

    int bpm,toneValue;
 
    float interval;
    
    heartSensor();
    void heartBeat();
    void healthLeds(bool active);
    void beebBpm();
    int periodToBPM(unsigned long t);
private:
};

extern heartSensor* heart;
