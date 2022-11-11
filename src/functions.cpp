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

unsigned char clock::readRegister(unsigned char addr)
{ 
  unsigned char i;
  unsigned char data = 0;
  // RST monte a "1", debut de transaction
  digitalWrite(resetPin, HIGH);
  delay(1);
  // Bit de lecture/ecriture mis a lecture
  digitalWrite(dataPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, LOW);   
  delay(1);
  // Ecriture de l'adresse
//Serial.print("Adresse: ");
  for (i=0 ; i<=4 ; i++)
  {
    digitalWrite(dataPin, (addr & (1<<i))>>i);   
    delay(1);
//Serial.print((adresse & (1<<i))>>i);
    digitalWrite(clockPin, HIGH);   
    delay(1);
    digitalWrite(clockPin, LOW);   
    delay(1);
  }
//Serial.println();
  // Bit de horloge/RAM mis a horloge
  digitalWrite(dataPin, LOW);   
  delay(1);
  digitalWrite(clockPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, LOW);   
  delay(1);
  // 8 eme bit toujours un "1"
  digitalWrite(dataPin, HIGH);   
  delay(1);
  digitalWrite(clockPin, HIGH);   
  delay(1);
      // Attention, ici on n'abaisse pas le clk car c'est une lecture
  // Lecture de la donnee de 8 bits
  pinMode(dataPin, INPUT);    // On passe en mode de lecture
  delay(1);
  for (i=0 ; i<=7 ; i++)
  {
    digitalWrite(clockPin, LOW);   
    delay(1);
    data = data + (digitalRead(dataPin)<<i);   
    digitalWrite(clockPin, HIGH);   
    delay(1);
  }
  // RST baisse a "0", fin de transaction
  digitalWrite(clockPin, LOW);
  delay(1); 
  digitalWrite(clockPin, LOW);   
  delay(1);
  pinMode(dataPin, OUTPUT);    // On passe en mode d'ecriture
  delay(1);
  return data;
}

bool clock::initClock()
{
  writeRegister(7,0b00000000);    // access aux registres
  writeRegister(0,0b00000000);    // secondes
  writeRegister(1,0b00011000);    // minutes
  writeRegister(2,0b00000000);    // 24 heures
  writeRegister(3,0b00010010);    // jour (numero)
  writeRegister(4,0b00010001);    // mois
  writeRegister(5,0b00000110);    // date (de 1-7 ex mardi)
  writeRegister(6,0b00100010);    // annee 
  return true;
}
void clock::afficheHeure()
{

  heart->screen->pDisp->fillRect(80-2,5-2,50,10,BLACK);
  heart->screen->pDisp->setCursor(80,5);
  heart->screen->pDisp->setTextSize(1.5);
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