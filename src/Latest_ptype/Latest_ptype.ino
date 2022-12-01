#include <LiquidCrystal_I2C.h> // Frank de Brabander's v1.1.2
#include <Keypad.h>

#define STEPS_PER_REV 200




// 4x4 Keypad w/ 4 rows and 4 cols
#define ROW 4
#define COL 4

// The formatting of 4x4 Keypad
char keymap[ROW][COL] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// 16x2 LCD
#define LCD_ROW 2
#define LCD_COL 16

// LCD I2C Address: 0x27 from datasheet
#define LCD_I2C_ADD 0x27

// Pins D[6:9] from PD[6:7] and PB[0:1]
// Pins D[10:13] from PB[2:5]
byte pin_R[ROW] = {9, 8, 7, 6};
byte pin_C[COL] = {13, 12, 11, 10};

String valid_key = "AB123";
char clr_key = 'C';
String entered_key = "00000";
int num_keys = 0;
int l_flag = 0; // UNLOCKED(0), LOCKED(1)

// Global Object Declarations
Keypad keypad = Keypad(makeKeymap(keymap), pin_R, pin_C, ROW, COL);
LiquidCrystal_I2C lcd(LCD_I2C_ADD, LCD_COL, LCD_ROW);

void setup(){
    
    Serial.begin(9600);
   
    // LCD Initialization
    lcd.init();
    lcd.backlight();
    lcd.begin(LCD_COL, LCD_ROW);
   
    scrWrite("Enter Key       ","_____         ");
    Serial.println("Hreee\n");
}
void loop(){
        //Recieve the key pressed
        char key_r = keypad.getKey();

        if (key_r){
            //relay_pos
            entered_key[num_keys] = key_r;   // store key recieved to array
            num_keys = num_keys + 1;        // increment number of keys (idx)
            Serial.println(key_r);

            writeCode(num_keys - 1, key_r);

            // check if entered key is same as valid key
            if (entered_key == valid_key){
                num_keys = 0;
                entered_key = "00000";

                // Check if Unlocked (0)
                if (l_flag == 0){
                    Serial.println("Entered Key... Valid! Locking Door...");
                    scrWrite("Entered Key... Valid!    ", "Locking Door...");
                    //stepper_turn
                    scrWrite("       Door      ", "    Locked!      ");
                    l_flag = 1; // Locked
                    delay(2000);
                    // relay_pos
                    scrWrite("Enter Key      ", "_____              ");
                }

                // Check if Locked (1)
                else if (l_flag == 1){
                    Serial.println("Entered Key... Valid. Unlocking Door...");
                    scrWrite("Entered Key... Valid!  ", "Unlocking Door...");
                    //stepper_turn
                    Serial.println("Door Unlocked.   ");
                    scrWrite("     Door     ", "        Unlocked!     ");
                    l_flag = 0;
                    delay(2000);
                    //relay_pos
                    scrWrite("Enter Key     ", "_____            ");
                }
            }

            // Check if key recieved is the Clear key 'C'
            else if (key_r == clr_key){
                Serial.println("Key cleared.");
                scrWrite("        Key      ","     Cleared     ");
                delay(2000);
                //relay_pos
                scrWrite("Enter Key      ", "_____            ");
                entered_key = "00000";
                num_keys = 0;
            }

            // Check if number of keys recieved is greater than 5 LIMIT
            else if(num_keys >= 5){
                entered_key = "00000";
                num_keys = 0;
                Serial.println("Invalid Key. Try again...");
                scrWrite("Incorrect Key. ","Try again...   ");
                delay(2000);
                //relay_pos
                scrWrite("Enter Key       ", "_____           ");
            }
        }

        //button stuff
    }


void scrWrite(char upperWord[16], char lowerWord[16])
{
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(upperWord);
  lcd.setCursor(0, 1);
  lcd.print(lowerWord);
}

void writeCode(int cursor_pos, char key)
{
  lcd.backlight();
  lcd.setCursor(cursor_pos, 1);
  lcd.print(key);
}








    
