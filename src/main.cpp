#include "includes.h"

heartSensor* heart = new heartSensor();
clock* horloge = new clock(false);

void setup() {
 
  Serial.begin(9600);
  pinMode(sensorHeartCapt,INPUT);
  if(!heart->screen->pDisp->begin(SSD1306_SWITCHCAPVCC,0X3C)){
    Serial.print("Display1 initialization failed, exiting....\n");
    exit(1);
  }
  /*
  if(!heart->screen->pDisp->begin(SSD1306_SWITCHCAPVCC,0X3D)){
    Serial.print("Display2 initialization failed, exiting....\n");
    exit(1);
  }
  */
  heart->screen->pDisp->clearDisplay();
}

void loop() {
  
  heart->currentMillis = millis();

  heart->heartBeat();
  horloge->afficheHeure();

  heart->screen->pDisp->display();
}
