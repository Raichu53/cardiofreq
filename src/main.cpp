#include "includes.h"

heartSensor* heart = new heartSensor();
clock* horloge = new clock();

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
  //horloge->initClock();
  heart->screen->pDisp->clearDisplay();
}

void loop() {
  heart->currentMillis = millis();

  if(heart->screen->isButtonPressed() == 2){
    heart->screen->toggleScreen = !heart->screen->toggleScreen;
  }
  if(heart->screen->toggleScreen){
    horloge->afficheHeure();
  }else{
    heart->heartBeat();
    if(heart->screen->pressed){
      heart->beebBpm();
    }
  }
  heart->screen->pDisp->display();
}
