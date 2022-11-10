#include "includes.h"

heartSensor* heart = new heartSensor();

void setup() {
 
  Serial.begin(9600);
  pinMode(sensorHeartCapt,INPUT);
  if(!heart->screen->pDisp->begin(SSD1306_SWITCHCAPVCC,0X3C)){
    Serial.print("Display initialization failed, exiting....\n");
    exit(1);
  }
  heart->screen->pDisp->clearDisplay();

}

void loop() {
  
  heart->currentMillis = millis();

  heart->heartBeat();
  
  heart->screen->pDisp->display();
}
