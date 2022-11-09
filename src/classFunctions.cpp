#include "includes.h"

oled::oled()
{
    //default settings
    //modifiable a tout moment
    screenDimension.x = 128;
    screenDimension.y = 64;

    pDisp = new Adafruit_SSD1306(screenDimension.x,screenDimension.y,&Wire,-1);

    txt = "";
    size = 1;
    cursorPos.x = 0;
    cursorPos.y = 32;
    color = WHITE;
}

void oled::printText()
{
    pDisp->setTextSize(size);
    pDisp->setTextColor(color);
    pDisp->setCursor(cursorPos.x,cursorPos.y);
    pDisp->println(txt);
}

