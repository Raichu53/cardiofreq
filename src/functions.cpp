#include "includes.h"
/// @brief calcule les bpm grace a la periode entre chaque pic sur la courbe
void heartSensor::heartBeat()
{   
    unsigned long startTime,tempsEnHaut,period = 0;
    int moyenne;
    int buffer[20] = {};

    //on rempli le tableau
    for(int i = 0; i < 20;i++)
    {
        buffer[i] = analogRead(sensorHeartCapt);
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
        delay(100);
    }
    for(int k = 0; k < 20;k++)
    {
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
    healthLeds();
}
// period * x = 60 <=> x = 60/ period
int periodToBPM(unsigned long t){
    return (60000/t);
}
/// @brief https://pdf1.alldatasheet.com/datasheet-pdf/download/254790/MAXIM/DS1302.html page 8/13
/// @param addr l'addresse du registre a ecrire
/// @param data la valeur a mettre dans le registre precedent
void clock::writeRegister(unsigned char addr,unsigned char data)
{ 
//https://pdf1.alldatasheet.com/datasheet-pdf/download/254790/MAXIM/DS1302.html 
//voir bas page 8 <=> on reproduit en code
  unsigned char i;
  // on met ce a 1 jusqu'a la fin
  digitalWrite(resetPin, HIGH);
  delay(1);
  // Read = high / Write = low, ici low car on ecrit
  digitalWrite(dataPin, LOW);   
  delay(1);
  //on met clock a 1 puis 0 pour ecrire la value dans le registre
  digitalWrite(clockPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, LOW);   
  delay(1);
  // Ecriture de l'adresse
  for (i=0 ; i<=4 ; i++)
  {
    digitalWrite(dataPin, (addr & (1<<i))>>i);   
    delay(1);
    digitalWrite(clockPin, HIGH);   
    delay(1);
    digitalWrite(clockPin, LOW);   
    delay(1);
  }
  // Ram  = HIGH et clock = LOW (ici low)
  digitalWrite(dataPin, LOW);   
  delay(1);
  digitalWrite(clockPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, LOW);   
  delay(1);
  // le dernier bit est toujours un 1
  digitalWrite(dataPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, LOW);   
  delay(1);   
  // Ecriture de la donnee de 8 bits
  for (i=0 ; i<=7 ; i++)
  {
    digitalWrite(dataPin, (data & (1<<i))>>i);   
    delay(1);
    digitalWrite(clockPin, HIGH);   
    delay(1);
    digitalWrite(clockPin, LOW);   
    delay(1);
  }
  // reset a 0, fin
  digitalWrite(resetPin, LOW);
  delay(1); 
}
/// @brief https://pdf1.alldatasheet.com/datasheet-pdf/download/254790/MAXIM/DS1302.html page 8/13
/// @param addr l'addresse du registre a lire
unsigned char clock::readRegister(unsigned char addr)
{ 
  //voir comment dans writeRegister()
  unsigned char i;
  unsigned char data = 0;
  digitalWrite(resetPin, HIGH);
  delay(1);
 
  digitalWrite(dataPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, LOW);   
  delay(1);
  // Ecriture de l'adresse
  for (i=0 ; i<=4 ; i++)
  {
    digitalWrite(dataPin, (addr & (1<<i))>>i);   
    delay(1);
    digitalWrite(clockPin, HIGH);   
    delay(1);
    digitalWrite(clockPin, LOW);   
    delay(1);
  }
  digitalWrite(dataPin, LOW);   
  delay(1);
  digitalWrite(clockPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, LOW);   
  delay(1);
  digitalWrite(dataPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, HIGH);   
  delay(1);
  // Lecture de la donnee de 8 bits donc input 
  pinMode(dataPin, INPUT);    
  delay(1);
  for (i=0 ; i<=7 ; i++)
  {
    digitalWrite(clockPin, LOW);   
    delay(1);
    data = data + (digitalRead(dataPin)<<i);   
    digitalWrite(clockPin, HIGH);   
    delay(1);
  }
  digitalWrite(clockPin, LOW);
  delay(1); 
  digitalWrite(clockPin, LOW);   
  delay(1);
  pinMode(dataPin, OUTPUT);   
  delay(1);
  return data;
}
/// @brief ecrit dans les registres les valeurs souhaités 
/// @return true si sucessfull
bool clock::initClock()
{
  writeRegister(7,0b00000000);    // access aux registres
  writeRegister(0,0b00000000);    // secondes
  writeRegister(1,0b01010010);    // minutes
  writeRegister(2,0b00010011);    // 24 heures
  writeRegister(3,0b00010010);    // jour (numero)
  writeRegister(4,0b00010001);    // mois
  writeRegister(5,0b00000110);    // date (de 1-7 ex mardi)
  writeRegister(6,0b00100010);    // annee 
  return true;
}
/// @brief cette fonction recupere la valeur des registre et les affiche sur l'ecran oled
void clock::afficheHeure()
{
  heart->screen->pDisp->setCursor(30,20);
  heart->screen->pDisp->setTextSize(2);
  heart->screen->pDisp->setTextColor(WHITE);
  //heures
  buffer = readRegister(0x02);
  heart->screen->pDisp->print((buffer & 0b00110000)>>4,DEC);
  heart->screen->pDisp->print((buffer & 0b00001111),DEC);
  heart->screen->pDisp->print(":");
  //minutes
  buffer = readRegister(0x01);
  heart->screen->pDisp->print((buffer & 0b11110000)>>4,DEC);
  heart->screen->pDisp->print((buffer & 0b00001111),DEC);
  heart->screen->pDisp->print(":");
  //secondes
  buffer = readRegister(0x00); 
  heart->screen->pDisp->print((buffer & 0b01110000)>>4,DEC);
  heart->screen->pDisp->print((buffer & 0b00001111),DEC);
}
/// @brief cette fonction sert a switch entre l'heure et la courbe bpm. montage "Pull down" pour l'heure
/// @return true si pressé, false sinon
bool oled::isButtonPressed()
{
  bButton = (bool)digitalRead(buttonPin);
  if(bButton){
    drawBlackScreen();
    heart->screen->cursorPos.x = 0;
    buffer = true;
    return true;
  }
  else{
    if(buffer){
      drawBlackScreen();
      buffer = false;
    }
    return false;
  }
}
/// @brief ecran noir
void oled::drawBlackScreen()
{
  heart->screen->pDisp->fillRect(0,0,128,64,BLACK);
}
/// @brief allume les led en fonction des bpm
void heartSensor::healthLeds()
{
  if(bpm >= 50 && bpm < 90){
    digitalWrite(greenLed,HIGH);
    digitalWrite(redLed,LOW);
    digitalWrite(yellowLed,LOW);
  }
  else if(bpm >= 90 && bpm < 170){
    digitalWrite(yellowLed,HIGH);
    digitalWrite(redLed,LOW);
    digitalWrite(greenLed,LOW);
  }else if((bpm >= 170 && bpm < 220) || bpm < 50){
    digitalWrite(redLed,HIGH);
    digitalWrite(greenLed,LOW);
    digitalWrite(yellowLed,LOW);
  }
}