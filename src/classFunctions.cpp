#include "includes.h"

oled::oled()
{
    //default settings
    screenDimension.x = 128;
    screenDimension.y = 64;

    pDisp = new Adafruit_SSD1306(screenDimension.x,screenDimension.y,&Wire,-1);

    size = 1;
    cursorPos.x = 0;
    cursorPos.y = 32;
    color = WHITE;
}

heartSensor::heartSensor()
{
    screen = new oled();
    startMillis = millis();
    currentMillis = 0;
    oldTime = 0;
    start = false;
    stop = false;
}
//init param utilisé que lors de la creation
clock::clock(bool init)
{
  pinMode(clockPin, OUTPUT); 
  digitalWrite(clockPin, LOW);   

  pinMode(resetPin, OUTPUT);   
  digitalWrite(resetPin, LOW); 

  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, LOW);

  if(init){
    if(!initClock()){
        Serial.println("Erreur dans l'initialisation de la clock");
    }
  }
}