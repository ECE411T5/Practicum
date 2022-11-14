#include <avr/io.h>
#include "LCD_16x2.h"
//#include "KEYPAD_4x4.h"

int main(void) {
    DDRC = 0x0F;

    lcd_init();
    //lcd_print("hello world!");

    while(1) {
    }

    return 0;
}
