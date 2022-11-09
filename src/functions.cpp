#include "includes.h"

void heartBeat()
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
    
    //drawing sinusoide on oled screen
    screen->cursorPos.x = screen->cursorPos.x + 1;
    if(screen->cursorPos.x > 128)
    {
        screen->cursorPos.x = screen->cursorPos.x - 128;
        screen->pDisp->clearDisplay();
    }
    int switchSide = 0;
    for(int k = 0; k < 20;k++)
    {
        if(buffer[k] == (int)moyenne)
            Serial.println("egal");
        if(buffer[k] > moyenne){
            //vers le haut
            screen->cursorPos.y = screen->cursorPos.y - (buffer[k] - moyenne);
            
            
        }
        else{
            //vers le bas
            screen->cursorPos.y = screen->cursorPos.y + (moyenne - buffer[k]);
            
        }
        
        screen->pDisp->drawLine(screen->cursorPos.x,32,screen->cursorPos.x,screen->cursorPos.y,WHITE);
    }
    
    /*
        avoir les changement des fronts 
        il faut que a chaque changement de front on compte le temps jusqu'au prochain changement de front
        et a partir de la, calculer la frequence
    */
    if(screen->cursorPos.y > 32){
        //etat bas
        
        if(start){
            tempsEnHaut = (currentMillis - startTime); //car le calcul est sur le front descendant

            period = (tempsEnHaut - oldTime)/4;

            bpm = periodToBPM(period);
            if(bpm>30){
                screen->pDisp->setCursor(5,5);
                screen->pDisp->setTextColor(WHITE,BLACK);
                screen->pDisp->setTextSize(2);
                screen->pDisp->println(bpm);
            }
            

            oldTime = tempsEnHaut; 
            start = !start;
        }
        
    }
    else{
        //etat haut
        if(!start){
            start = !start;
            startTime = currentMillis;
        }

    }
    
}
// period * x = 60 <=> x = 60/ period
int periodToBPM(unsigned long t){
    return (60000/t);
}