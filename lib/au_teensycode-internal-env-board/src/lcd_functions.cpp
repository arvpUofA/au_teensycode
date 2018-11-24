#include <ShiftLCD.h>



#define  DATAPIN  11
#define  CLOCKPIN 13
#define  LATCHPIN 15
#define  MAXCOLUMNS  16
#define  MAXLINES  2
#define  FOURBIT  4

void setup_lcd(void){
    ShiftLCD lcd(DATAPIN,LATCHPIN,CLOCKPIN,FOURBIT);

    lcd.begin(MAXCOLUMNS,MAXLINES);
}


void display_to_lcd(float temp,float hum,float pressure, float volt){
    //0123456789ABCDE
    //T:---   H:--
    //P:--.-
    lcd.setCursor(0, 0);
    lcd.print("T:%3.0f   H:%2.0f%%", temp, hum);
    lcd.setCursor(0, 1);
    lcd.print("P:%2.1f", pressure);


}

void display_voltages_to_lcd(float battery1, float battery2, float battery3, float battery4){
    //0123456789ABCDE
    //B1:--.- B2:--.-
    //B3:--.- B4:--.-
    lec.setCursor(0, 0);
    lcd.print("B1:%2.1f   B2:%2.1f%%", battery1, battery2);
    lcd.setCursor(0, 1);
    lcd.print("B3:%2.1f  B4:%2.1f", battery3, battery4);
}