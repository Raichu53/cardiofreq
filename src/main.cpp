#include "includes.h"

oled* screen = new oled();
unsigned long currentMillis;
unsigned long oldTime;
bool start;
bool stop;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensorHeartCapt,INPUT);
  if(!screen->pDisp->begin(SSD1306_SWITCHCAPVCC,0X3C)){
    Serial.print("Display initialization failed, exiting....\n");
    exit(1);
  }
  screen->pDisp->clearDisplay();

  unsigned long startMillis = millis();
  bool start = false;
  bool stop = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();

  heartBeat();
  
  screen->pDisp->display();
}
