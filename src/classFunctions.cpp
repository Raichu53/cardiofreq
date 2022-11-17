#include "includes.h"

oled::oled()
{
    //default settings
    screenDimension.x = 128;
    screenDimension.y = 64;

    pDisp = new Adafruit_SSD1306(screenDimension.x,screenDimension.y,&Wire,-1);

    cursorPos.x = 0;
    cursorPos.y = 32;
    
    bButton = false;
    
    pressed = false;
    press = false;
    delayMax = 0;
    doublePush = 0;

    toggleScreen = false;
}

heartSensor::heartSensor()
{
    screen = new oled();
    horloge = new clock();
    startMillis = millis();
    currentMillis = 0;
    oldTime = 0;
    start = false;
    bpm = 0;
    interval = 0.f;
    toneValue = 0;
    lastBeep = 0;
    inverseI = 10;
    k1 = 0;
    pinMode(sensorHeartCapt,INPUT);

    pinMode(redLed,OUTPUT);
    pinMode(yellowLed,OUTPUT);
    pinMode(greenLed,OUTPUT);

    digitalWrite(redLed,LOW);
    digitalWrite(yellowLed,LOW);
    digitalWrite(greenLed,LOW);

    pinMode(buzzerPin,OUTPUT);

    pinMode(pinEEPROM,INPUT);
}
//init param utilisé que lors de la creation
clock::clock()
{
  pinMode(clockPin, OUTPUT); 
  digitalWrite(clockPin, LOW);   

  pinMode(resetPin, OUTPUT);   
  digitalWrite(resetPin, LOW); 

  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, LOW);

}