#include "ShiftLCD.h"



#define  DATAPIN  11
#define  CLOCKPIN 13
#define  LATCHPIN 15
#define  MAXCOLUMNS  16
#define  MAXLINES  2
#define  FOURBIT  4

static ShiftLCD *lcd = nullptr;

void setup_lcd(void){
    lcd = new ShiftLCD(DATAPIN,LATCHPIN,CLOCKPIN,FOURBIT);

    lcd->begin(MAXCOLUMNS,MAXLINES);
}


void display_to_lcd(float temp,float hum,float pressure){
    //0123456789ABCDE
    //T:---   H:--
    //P:--.-
    lcd->setCursor(0, 0);
    lcd->printf("T:%f   H:%f", temp, hum);
    lcd->setCursor(0, 1);
    lcd->printf("P:%f", pressure);


}

void display_voltages_to_lcd(float battery1, float battery2, float battery3, float battery4){
    //0123456789ABCDE
    //B1:--.- B2:--.-
    //B3:--.- B4:--.-
    lcd->setCursor(0, 0);
    lcd->printf("B1:%2.1f   B2:%2.1f%%", battery1, battery2);
    lcd->setCursor(0, 1);
    lcd->printf("B3:%2.1f  B4:%2.1f", battery3, battery4);
}