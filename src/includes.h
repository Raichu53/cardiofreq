#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Tone.h>
#include <EEPROM.h>

#define sensorHeartCapt A0
#define clockPin 6
#define dataPin 7
#define resetPin 8
#define buttonPin 3
#define redLed 12
#define yellowLed 11
#define greenLed 10
#define buzzerPin 5
#define POTpin A2
#define pinEEPROM 4

#define MAX_HEIGHT_PPG 50 //en px

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

    //graph
    vec2 startingPoint = {5,50};
    vec2 len = {120,50};

    int POTvalue,echelleABS,secondes,minutes,nbDeDecoupes,counter;
    float tailleDesDecoupes;
    oled();
    int isButtonPressed();
    void drawBlackScreen();
    void drawGraph();
private:

};
class heartSensor
{
public:
    oled* screen;
    clock* horloge;
    unsigned long startMillis,currentMillis,oldTime,lastBeep,EEPROMtiming,bufferTiming;

    bool start,buttonEEPROM;

    int bpm,toneValue,addr,lastAddr,bufferCount,k1,inverseI = 0;
 
    float interval;

    int buffer10[10];
    int buffer60[12];
    //int buffer600[10];

    heartSensor();
    void heartBeat(bool draw);
    void healthLeds(bool active);
    void beebBpm();
    int periodToBPM(unsigned long t);
    void sendDataToEEPROM();

    void addTobuffer();
    void fillBuffer1min();
    int moyenneBuffer10();
private:
};

extern heartSensor* heart;
