#ifndef __LCD_H
#define __LCD_H

#include <avr/io.h>
#include <util/delay.h>

void lcd_init();
void lcd_cmd(unsigned char ch);
void lcd_clr();
void lcd_print(char *str);
void lcd_print_pos(unsigned char row, unsigned char pos, char *str);

#endif
