#include <Arduino.h>
#include "ShiftLCD.h"

#define  DATAPIN  11
#define  CLOCKPIN 13
#define  LATCHPIN 15
#define  MAXCOLUMNS  16
#define  MAXLINES  2
#define  FOURBIT  4

static ShiftLCD *lcd = nullptr;

void setup_lcd(void){
    pinMode(6, OUTPUT);
    digitalWrite(6, HIGH);
    lcd = new ShiftLCD(DATAPIN,CLOCKPIN,LATCHPIN,FOURBIT);
    lcd->begin(MAXCOLUMNS,MAXLINES);
}


void display_to_lcd(float temp,float hum,float pressure){
    //0123456789ABCDE
    //T:---   H:--.--
    //P:-----        
    char buf[17];

    sprintf(buf, "T:%04.1f   H:%05.2f", temp, hum);
    lcd->setCursor(0, 0);
    lcd->print(buf);

    sprintf(buf, "P:%5.0f        ", pressure);
    lcd->setCursor(0, 1);
    lcd->print(buf);
}

void display_voltages_to_lcd(float battery1, float battery2, float battery3, float battery4){
    //0123456789ABCDE
    //B1:--.- B2:--.-
    //B3:--.- B4:--.-
    char buf[17];

    sprintf(buf,"B1:%04.1f B2:%04.1f ", battery1, battery2);
    lcd->setCursor(0,0);
    lcd->print(buf);

    sprintf(buf,"B3:%04.1f B4:%04.1f ", battery3, battery4);
    lcd->setCursor(0,1);
    lcd->print(buf);
}