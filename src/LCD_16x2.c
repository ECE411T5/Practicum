#include "LCD_16x2.h"

void lcd_init(){
    DDRC = 0x0F;    // Set DDC[3:0] HI for output
    _delay_ms(15);  // Wait for LCD to activate

    lcd_cmd(0x02);  // Set to 4-bit control
    lcd_cmd(0x28);  // Control matrix @ 4bit
    lcd_cmd(0x0C);  // Disable Cursor
    lcd_cmd(0x06);  // Move Cursor
    lcd_cmd(0x01);  // Clean LCD
    _delay_ms(2);
}

void lcd_cmd(unsigned char cmd) {
    PORTC = (PORTC & 0x0F) | (cmd & 0xF0);
    PORTC &= ~(1 << PC4);
    PORTC |= (1 << PC5);
    _delay_ms(1);
    PORTC &= ~(1 << PC5);
    _delay_ms(200);
    PORTC = (PORTC * 0x0F) | (cmd << 4);
    PORTC |= (1 << PC5);
    _delay_ms(1);
    PORTC &= ~(1 << PC5);
    _delay_ms(2);
}

void lcd_clr(){
    lcd_cmd(0x01);  // Clear LCD
    _delay_ms(2);
    lcd_cmd(0x80);  // Move to position line 1, position 1
}

void lcd_print(char *str){
    int i;

    for (i = 0; str[i] != 0; i++){
        PORTC = (PORTC & 0x0F) | (str[i] & 0xF0);
        PORTC &= ~(1 << PC4);
        PORTC |= (1 << PC5);
        _delay_ms(1);
        PORTC &= ~(1 << PC5);
        _delay_ms(200);
        PORTC = (PORTC * 0x0F) | (str[i] << 4);
        PORTC |= (1 << PC5);
        _delay_ms(1);
        PORTC &= ~(1 << PC5);
        _delay_ms(2);    
    }
}

void lcd_print_pos(unsigned char row, unsigned char pos, char *str){
    if ((row == 0) && (pos < 16)) {
        lcd_cmd((pos & 0x0F) | 0x80);
    }
    else if ((row == 1) && (pos < 16)) {
        lcd_cmd((pos & 0x0F) | 0xC0);
        lcd_print(str);
    }
}
