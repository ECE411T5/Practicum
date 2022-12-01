#include <LiquidCrystal.h>
#include <Keypad.h>

//#define DDRB *((volatile byte*) 0x24)
//#define PORTB *((volatile byte*) 0x25)

//#define DDRC *((volatile byte*) 0x27)
//#define PORTC *((volatile byte*) 0x28)

//#define DDRD *((volatile byte*) 0x2A)
//#define PORTD *((volatile byte*) 0x2B)

// 4-bit Mode for LCD 1602A 16x2 Display
// Pins D[19:14] from PC[5:0]
#define RS 14
#define EN 15
#define D7 19
#define D6 18
#define D5 17
#define D4 16

// 4x4 Keypad w/ 4 rows and 4 cols
#define ROW 4
#define COL 4

// The formatting of 4x4 Keypad
char keys[ROW][COL] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pins D[6:9] from PD[6:7] and PB[0:1]
// Pins D[10:13] from PB[2:5]
byte pin_R[ROW] = {6, 7, 8, 9};
byte pin_C[COL] = {10, 11, 12, 13};

// Objects for Keypad and LCD
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
Keypad keypad = Keypad(makeKeymap(keys), pin_R, pin_C, ROW, COL);

int main(void){
  //DDRC = 0x3F;    // Set PC[5:0] HI (Output)

  

  lcd.begin(16,2); // Define 16 Cols and 2 Rows of LCD
  lcd.display(); // PWR ON backlight
   
  while(1) {
    lcd.setCursor(0, 0);  // Position to write from first row, first col
    lcd.print("PRESS any KEY"); // Can write 16 chars per line

    char key = keypad.getKey();
    lcd.setCursor(0, 1);  // Position to write from second row, first col
    lcd.print("Key Pressed: ");

    
    if (key) {
      lcd.print(key);
      delay(100);
    }
  }

  return 0;
}
