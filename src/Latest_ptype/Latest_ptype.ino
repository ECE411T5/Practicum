/* Team 5:  Mohamed Ghonim
 *          Joshua Hobby
 *          Mohamed Ashkanani
 *          Ibrahim Binmahfood
 * 
 * Inspiration:
 * https://github.com/camdeno/ECE-411-Team-5
 * 
 * NOTE: status_btn which is output from UNLOCK_BTN pin on PCB is 
 * supposed to be a pull down resistor rather than pull up.
 */
 
#include <LiquidCrystal_I2C.h> // Frank de Brabander's LiquidCrystal_I2C v1.1.2
#include <Keypad.h>

#define STEPS_PER_REV 200
#define STEPS_REQ     65
#define M_SPEED       2000

// Connected to PD2 and PD3
#define DIR           2
#define STEP          3

// Connected to PD4
#define RELAY_SIG     4

// Connected to PD5
#define UNLOCK_BTN    5

// 4x4 Keypad w/ 4 rows and 4 cols
#define ROW           4
#define COL           4

// LCD Configuration
#define LCD_ROW 2
#define LCD_COL 16

// LCD I2C Address: 0x27 from datasheet
#define LCD_I2C_ADD 0x27
  
// The formatting of 4x4 Keypad
char keymap[ROW][COL] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pins D[6:9] from PD[6:7] and PB[0:1]
// Pins D[10:13] from PB[2:5]
byte pin_R[ROW] = {6, 7, 8, 9};
byte pin_C[COL] = {10, 11, 12, 13};

String valid_key = "12312";
char clr_key = 'C';
String entered_key = "00000";
int num_keys = 0;
int l_flag = 0; // UNLOCKED(0), LOCKED(1)

// Set to 0 initially
int status_btn = 0;

// Global Object Declarations
Keypad keypad = Keypad(makeKeymap(keymap), pin_R, pin_C, ROW, COL);
LiquidCrystal_I2C lcd(LCD_I2C_ADD, LCD_COL, LCD_ROW);

void setup(){
    
    Serial.begin(9600);
    Serial.println("Initializing...");

    // Relay Position and set to be OUTPUT
    pinMode(RELAY_SIG, OUTPUT);
    relay_pos(0);   // Set signal to LO

    // Emergency Unlock button set to INPUT
    pinMode(UNLOCK_BTN, INPUT);

    // Stepper Motor Driver STEP and DIR as OUTPUTs
    pinMode(STEP, OUTPUT);
    pinMode(DIR, OUTPUT);
    
    // LCD Initialization
    lcd.init();
    lcd.backlight();
    lcd.begin(LCD_COL, LCD_ROW);
   
    scrWrite("Enter Key       ","_____         ");
    Serial.println("Enter Key");
}
void loop(){
  //Recieve the key pressed
  char key_r = keypad.getKey();
  
  if (key_r){
      relay_pos(1);     // Set signal to HI
      entered_key[num_keys] = key_r;   // store key recieved to array
      num_keys = num_keys + 1;        // increment number of keys (idx)
      Serial.println("Digit Recieved:");
      Serial.println(key_r);
  
      writeCode(num_keys - 1, key_r);
  
      // check if entered key is same as valid key
      if (entered_key == valid_key){
          num_keys = 0;
          entered_key = "00000";
  
          // Check if Unlocked (0)
          if (l_flag == 0){
              Serial.println("Entered Key... Valid! Locking Door...");
              scrWrite("Key is...Valid!", "Locking Door...");
              stepTurn(STEPS_REQ, 1, M_SPEED);
              scrWrite("       Door    ", "    Locked!    ");
              l_flag = 1; // Locked
              delay(2000);
              relay_pos(0);   // Set signal to LO
              scrWrite("Enter Key      ", "_____          ");
          }
  
          // Check if Locked (1)
          else if (l_flag == 1){
              Serial.println("Entered Key... Valid. Unlocking Door...");
              scrWrite("Key is...Valid!", "Unlocking Door...");
              stepTurn(STEPS_REQ, 0, M_SPEED);
              Serial.println("Door Unlocked.   ");
              scrWrite("     Door     ", "   Unlocked!   ");
              l_flag = 0;
              delay(2000);
              relay_pos(0);   // Set signal to LO
              scrWrite("Enter Key     ", "_____          ");
          }
      }
  
      // Check if key recieved is the Clear key 'C'
      else if (key_r == clr_key){
          Serial.println("Key cleared!");
          scrWrite("     Key    ","    Cleared    ");
          delay(2000);
          relay_pos(0);     // Set signal to LO
          scrWrite("Enter Key      ", "_____          ");
          entered_key = "00000";
          num_keys = 0;
      }
  
      // Check if number of keys recieved is greater than 5 LIMIT
      else if(num_keys >= 5){
          entered_key = "00000";
          num_keys = 0;
          Serial.println("Invalid Key. Try again...");
          scrWrite("Incorrect Key! ","Try again...");
          delay(2000);
          relay_pos(0);     // Set signal to LO
          scrWrite("Enter Key      ", "_____          ");
      }
  }
  
  // Read the UNLOCK_BTN was toggled and get returned value
  status_btn = digitalRead(UNLOCK_BTN);
  Serial.println("Status Button:");
  Serial.println(status_btn);
  
  // Check if both status returned is 1 AND the num of keys is set 0
  // NOTE: status_btn which is output from UNLOCK_BTN pin on PCB is 
  // supposed to be a pull down resistor rather than pull up. So it is
  // negated with '!'.
  if ((!(status_btn) == 1) && (num_keys == 0)){
  
    // Check if the lock flag is 0
    if (l_flag == 0){
      scrWrite("   Locking...  ", "     Door     ");
      relay_pos(1); // Set the signal HI
      delay(2000);
      stepTurn(STEPS_REQ, 1, M_SPEED);
      delay(2000);
      relay_pos(0); // Set the signal LO
      scrWrite("Enter Key     ", "_____          ");
      l_flag = 1;
      num_keys = 0;
    }
  
    // Check if the lock flag is 1
    else if (l_flag == 1){
      scrWrite(" Unlocking...  ", "    Door    ");
      relay_pos(1);  // Set the signal HI
      delay(2000);
      stepTurn(STEPS_REQ, 0, M_SPEED);
      delay(2000);
      relay_pos(0);  // Set the signal LO
      scrWrite("Enter Key      ", "_____          ");
      l_flag = 0;
      num_keys = 0;
    }
  }
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

void relay_pos(int state){
  if (state == 1){
    digitalWrite(RELAY_SIG, HIGH);
  }
  else if (state == 0){
    digitalWrite(RELAY_SIG, LOW);
  }
}

void stepTurn(int rev, int dir, int speed_m)
{
  digitalWrite(DIR, dir); // 1 for CW and 0 for CCW

  for (int x = 0; x < rev; x++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(speed_m); // Increase delay for slower rotation
    digitalWrite(STEP, LOW);
    delayMicroseconds(speed_m);
  }
}







    
