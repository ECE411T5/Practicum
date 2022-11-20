#include <LiquidCrystal.h>

#define DDRB *((volatile byte*) 0x24)
#define PORTB *((volatile byte*) 0x25)

#define DDRC *((volatile byte*) 0x27)
#define PORTC *((volatile byte*) 0x28)

#define DDRD *((volatile byte*) 0x2A)
#define PORTD *((volatile byte*) 0x2B)

// Pins D[19:14] from PC[5:0]
#define RS 14
#define EN 15
#define D7 19
#define D6 18
#define D5 17
#define D4 16

int main(void){
  DDRC = 0x3F;    // Set PC[5:0] as Output

  LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

  lcd.begin(16,2);
  lcd.clear();
  lcd.print("helloo");
  
  while(1) {
    lcd.setCursor(0, 1);
    lcd.print("hello world!");
  }

  return 0;
}
