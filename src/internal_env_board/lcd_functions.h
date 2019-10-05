#ifndef LCD_FUNCTIONS_H 
    #define LCD_FUNCTIONS_H
#endif


extern void setup_lcd(void);
extern void display_to_lcd(float temp,float hum,float pressure);
extern void display_voltages_to_lcd(float battery1, float battery2, float battery3, float battery4);