/**
 *  LCD16x02 example
 *
 *  Arduino Nano and SI4844 pin connections
 *
 *  | SI4844 pin |  Arduino pin |  Description                                       |
 *  | ---------  | ------------ | -------------------------------------------------  |
 *  |    2       |   2          | Arduino interrupt pin                              |
 *  |   15       |   12         | Arduino A0 (Digital pin 14) for SI4844 RESET       |
 *  |   16       |  A4 (SDA)    | I2C bus (Data)                                     |
 *  |   17       |  A5 (SCL)    | I2C bus (Clock)                                    | 
 *  | -----------| -------------| ---------------------------------------------------|
 *  |  LCD 16x02 |                                                                   |
 *  | -----------| -------------| ---------------------------------------------------|                        
 *  |      D4    |     D7       | Arduino Digital Pin 7                              |
 *  |      D5    |     D6       | Arduino Digital Pin 6                              | 
 *  |      D6    |     D5       | Arduino Digital pin 5                              |
 *  |      D7    |     D4       | Arduino Digital Pin 4                              | 
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
#define LCD_D7 4
#define LCD_D6 5
#define LCD_D5 6
#define LCD_D4 7
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

// OLED - Declaration for a SSD1306 display connected to I2C (SDA, SCL pins)

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
  char tmp[15];
  char bufferDisplay[15];
  char * unit;
  byte bandMode = si4844.getStatusBandMode();
  uint32_t currentFrequency = (bandMode == 0)? (uint32_t) si4844.getFrequency()/10 : (uint32_t) si4844.getFrequency();

  sprintf(tmp, "%5.5u", currentFrequency);
  bufferDisplay[0] = (tmp[0] == '0') ? ' ' : tmp[0];
  bufferDisplay[1] = tmp[1];
  if (bandMode == 0)
  {
    bufferDisplay[2] = tmp[2];
    bufferDisplay[3] = '.';
    bufferDisplay[4] = tmp[3];
    unit = (char *) " MHz";
    display.setCursor(10, 0);
    display.print((si4844.getStatusStationIndicator()) ? "STEREO" : "  MONO");    
  }
  else
  {
    if ( currentFrequency  < 1000 ) {
      bufferDisplay[1] = ' ';
      bufferDisplay[2] = tmp[2] ;
      bufferDisplay[3] = tmp[3];
      bufferDisplay[4] = tmp[4];
    } else {
      bufferDisplay[2] = tmp[2];
      bufferDisplay[3] = tmp[3];
      bufferDisplay[4] = tmp[4];
    }
    
    unit = (char *) " KHz";
  }
  bufferDisplay[5] = '\0';
  strcat(bufferDisplay, unit);
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

  if (bandPlan[idxBand] == 40)
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
      setBand('+');
    else if (digitalRead(BAND_DOWN) == LOW )
      setBand('-');
    else if (digitalRead(VOL_UP) == LOW )
      setVolume('+');
    else if (digitalRead(VOL_DOWN) == LOW )
      setVolume('-');
  }

  if (si4844.hasStatusChanged())
    displayDial();
  
  delay(10);
}
