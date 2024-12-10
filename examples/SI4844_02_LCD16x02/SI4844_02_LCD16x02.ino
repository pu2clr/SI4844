/**
 *  LCD16x02 example
 *
 *  Arduino Nano and SI4844 pin connections
 *  You can find LCD20x2 3.3V version on AliExpress, eBay etc. 
 *  If you are using an LCD 5V version, you will need a bidirectional voltage converter. 
 *
 *  | SI4844 pin |  Arduino pin |  Description                                       |
 *  | ---------  | ------------ | -------------------------------------------------  |
 *  |    2       |   2          | Arduino interrupt pin                              |
 *  |   15       |   12         | SI4844 RESET                                       |
 *  |   16       |  A4 (SDA)    | I2C bus (Data)                                     |
 *  |   17       |  A5 (SCL)    | I2C bus (Clock)                                    | 
 *  | -----------| -------------| ---------------------------------------------------|
 *  |  LCD 16x02 |                                                                   |
 *  | -----------| -------------| ---------------------------------------------------|                        
 *  |      D7    |     D7       | Arduino Digital Pin 7                              |
 *  |      D6    |     D6       | Arduino Digital Pin 6                              | 
 *  |      D5    |     D5       | Arduino Digital pin 5                              |
 *  |      D4    |     D4       | Arduino Digital Pin 4                              | 
 *  |      RS    |     D3       | Arduino Digital Pin 3 for LCD RESET controle       | 
 *  |      E/ENA |     D13      | Arduino Digital Pin 13                             |
 *  |RW & VSS & K|     GND      |                                                    |
 *  | A & VDD    |    +Vcc      |                                                    |
 *  |      VO    |              | (see 5K tripot connection)                         |
 *  | -----------| -------------| ---------------------------------------------------|
 *  |Push Buttons|                                                                   |                                                                     |
 *  | -----------| -------------| ---------------------------------------------------| 
 *  |  BAND_UP   |     8        |                                                    |                          
 *  |  BAND_DOWN |     9        |                                                    | 
 *  |  VOL_UP    |    10        |                                                    | 
 *  |  VOL_DOWN  |    11        |                                                    |  
 * 
 *  ATTENTION: Arduino Nano and Uno are 5V based board. Check the board voltage you are using
 *  If you are using the LGT8F328 see: https://blog.eletrogate.com/tutorial-pro-mini-evb-lgt8f328p-arduino-ide/
 *
 *  Author: Ricardo Lima Caratti (PU2CLR)
 *  Oct, 2019 2020
*/

#include <SI4844.h>
#include <LiquidCrystal.h>

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12
#define DEFAULT_BAND 0

// LCD 16x02 or LCD20x4 PINs
#define LCD_D7 7
#define LCD_D6 6
#define LCD_D5 5
#define LCD_D4 4
#define LCD_RS 3
#define LCD_E  13


#define BAND_UP     8 // Next Band
#define BAND_DOWN   9 // Previous Band
#define VOL_UP     10 // Volume Volume Up
#define VOL_DOWN   11 // Volume Down

#define MIN_ELAPSED_TIME 100

long elapsedButton = millis();

// SI4844 Band Plan
// See Si48XX ATDD PROGRAMMING GUIDE, Table 8, pages 17 and 18
byte bandPlan[] = {4, 20, 26, 27, 28, 29, 31, 33, 35, 37, 39, 40};
const int maxBand = (sizeof(bandPlan) / sizeof(byte)) - 1;
int idxBand = 0;

LiquidCrystal display(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

SI4844 si4844;

void setup()
{
  
  pinMode(BAND_UP, INPUT_PULLUP);
  pinMode(BAND_DOWN, INPUT_PULLUP);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DOWN, INPUT_PULLUP);

  pinMode(INTERRUPT_PIN, INPUT);

  display.begin(16, 2);
  // Splash
  display.clear();
  display.setCursor(5, 0);
  display.print("SI4844");
  display.setCursor(0, 1);
  display.print("Arduino Library");
  Flash(2000);
  display.setCursor(0, 0);
  display.print("FM / AM(MW & SW)");
  display.setCursor(0, 1);
  display.print("By RICARDO/2020");
  Flash(3000);
  display.clear();
  delay(500);
  // si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND, 400000);  // if you want to use 400kHz I2C speed
  si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
  si4844.setVolume(55);
  
  displayDial();
}


/**
 *  Cleans the screen with a visual effect.
 */
void Flash (int d)
{
  delay(d);
  display.clear();
  display.noDisplay();
  delay(500);
  display.display();
}


void displayDial()
{
  char *bufferDisplay;

  bufferDisplay = si4844.getFormattedFrequency(2);
  display.setCursor(4, 1);
  display.print(bufferDisplay);

  display.setCursor(0, 0);
  display.print(si4844.getBandMode());
 
}

void setBand(byte cmd)
{
  if (cmd == '+')
    idxBand = (idxBand < maxBand) ? (idxBand + 1) : 0; 
  else
    idxBand = (idxBand > 0) ? (idxBand - 1) : maxBand;

  if (bandPlan[idxBand] == 40) // See bandPlan array declared at the begin of this sketch
  {
    // Configure the Pre-defined Band (band index 40) to work between 27.0 to 27.5 MHz
    // See Si48XX ATDD PROGRAMMING GUIDE, pages 17,18,19 and 20.
    si4844.setCustomBand(40, 27000, 27500, 5);
  }
  else if (bandPlan[idxBand] == 26)
  {
    // Configure the Pre-defined Band (band index 26) to work between 4.5 to 5.2 MHz
    si4844.setCustomBand(26, 4500, 5200, 5);
  }
  else if (bandPlan[idxBand] == 27)
  {
    // Configure the Pre-defined Band (band index 27) to work between 5.7 to 6.3 MHz
    si4844.setCustomBand(27, 5700, 6300, 5);
  }
  else
  {
    si4844.setBand(bandPlan[idxBand]);
  }
  display.clear();
  elapsedButton = millis();
}

void setVolume( char v) {
  si4844.changeVolume(v);
  elapsedButton = millis();
}

void loop()
{
  if ( (millis() - elapsedButton) > MIN_ELAPSED_TIME ) {
    // check if some button is pressed
    if (digitalRead(BAND_UP) == LOW ) 
      setBand('+'); // goes to the next band. 
    else if (digitalRead(BAND_DOWN) == LOW )
      setBand('-'); // goes to the previous band. 
    else if (digitalRead(VOL_UP) == LOW )
      setVolume('+');
    else if (digitalRead(VOL_DOWN) == LOW )
      setVolume('-');
  }

  if (si4844.hasStatusChanged())
    displayDial();
  
  delay(10);
}
