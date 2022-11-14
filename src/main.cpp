#include "includes.h"

//main class qui contient les ptr 
heartSensor* heart = new heartSensor();

void setup() {
 
  Serial.begin(9600);
  
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
  //heart->horloge->initClock();
  heart->screen->pDisp->clearDisplay();
}

void loop() {
  heart->currentMillis = millis();

  if(heart->screen->isButtonPressed() == 2){
    heart->screen->toggleScreen = !heart->screen->toggleScreen;
    if(!heart->screen->toggleScreen){
      heart->screen->pDisp->clearDisplay();
      heart->screen->cursorPos.x = 0;
    }
  }
  if(heart->screen->toggleScreen){
    heart->horloge->afficheHeure();
    heart->screen->drawGraph();
  }else{
    heart->heartBeat(false);
    heart->sendDataToEEPROM();
    if(heart->screen->pressed){
      heart->beebBpm();
    }
    heart->healthLeds(false);
  }
  heart->screen->pDisp->display(

  );
}
