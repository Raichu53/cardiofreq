#include "includes.h"

void heartSensor::heartBeat()
{   
    unsigned long startTime,tempsEnHaut,period = 0;
    int moyenne,bpm;
    int sensorPin = A0;
    int buffer[20] = {};

    //on rempli le tableau
    for(int i = 0; i < 20;i++)
    {
        buffer[i] = analogRead(sensorPin);
    }
    //on fait la moyenne des 20 valeurs
    for(int j = 0; j < 20;j++)
    {
        moyenne += buffer[j];
        if(j == 19){
            moyenne = moyenne /20;
        }
    }
    
    //drawing sinusoide on oled heart->screen
    heart->screen->cursorPos.x = heart->screen->cursorPos.x + 1;
    if(heart->screen->cursorPos.x > 128)
    {
        heart->screen->cursorPos.x = heart->screen->cursorPos.x - 128;
        heart->screen->pDisp->clearDisplay();
    }
    for(int k = 0; k < 20;k++)
    {
        if(buffer[k] == (int)moyenne)
            Serial.println("egal");
        if(buffer[k] > moyenne){
            //vers le haut
            heart->screen->cursorPos.y = heart->screen->cursorPos.y - (buffer[k] - moyenne);
        }
        else{
            //vers le bas
            heart->screen->cursorPos.y = heart->screen->cursorPos.y + (moyenne - buffer[k]);
        }
        heart->screen->pDisp->drawLine(heart->screen->cursorPos.x,32,heart->screen->cursorPos.x,heart->screen->cursorPos.y,WHITE);
    }
    /*
        avoir les changement des fronts 
        il faut que a chaque changement de front on compte le temps jusqu'au prochain changement de front
        et a partir de la, calculer la frequence
    */
    if(heart->screen->cursorPos.y > 32){
        //etat bas
        if(heart->start){
            tempsEnHaut = (heart->currentMillis - startTime); //car le calcul est sur le front descendant
            period = (tempsEnHaut - heart->oldTime)/4;

            bpm = periodToBPM(period);
            if(bpm>30){
                heart->screen->pDisp->setCursor(5,5);
                heart->screen->pDisp->setTextColor(WHITE,BLACK);
                heart->screen->pDisp->setTextSize(2);
                heart->screen->pDisp->println(bpm);
            }
            heart->oldTime = tempsEnHaut; 
            heart->start = !heart->start;
        }
    }
    else{
        //etat haut
        if(!heart->start){
            heart->start = !heart->start;
            startTime = heart->currentMillis;
        }
    }
}
// period * x = 60 <=> x = 60/ period
int periodToBPM(unsigned long t){
    return (60000/t);
}