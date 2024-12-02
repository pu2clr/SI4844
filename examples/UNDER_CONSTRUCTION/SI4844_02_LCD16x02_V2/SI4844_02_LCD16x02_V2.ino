/*

  This sketch compiles on the Arduino Nano, Uno, Pro Mini, and LGT8F328 (LGT8Fx board manager), 
  as well as on the ATmega328 standalone (MiniCore board manager).

  
  This sketch uses the EEPROM to store the last band used and the audio level. 
  This way, the radio will start on the band you were using when it was last turned off. 
  To reset the EEPROM, turn on the radio while holding down the "Next Band (BAND_UP)" button.

  See user_manual.txt before operating the receiver.

  Wire up on ATmega328 or Arduino Pro Mini or  Nano or  LGT8F328  and SI4844 and LCD16x02 

 *  | SI4844 pin |  Arduino pin |  Description                                       |
 *  | ---------  | ------------ | -------------------------------------------------  |
 *  |    2       |   2          | Arduino interrupt pin                              |
 *  |   15       |   12         | SI4844 RESET                                       |
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

  By PU2CLR, Ricardo, Nov,  2024.
*/

#include <SI4844.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

#define RESET_PIN 12
#define INTERRUPT_PIN 2

#define BAND_UP     8 // Next Band
#define BAND_DOWN   9 // Previous Band
#define VOL_UP     10 // Volume Volume Up
#define VOL_DOWN   11 // Volume Down
#define TOGGLE_VOL 14 

// LCD 16x02 or LCD20x4 PINs
#define LCD_D7 4
#define LCD_D6 5
#define LCD_D5 6
#define LCD_D4 7
#define LCD_RS 3
#define LCD_E  13

#define MINIMUM_DELAY 200

long elapsedButton = millis();
bool toggle = true;

// EEPROM - Stroring control variables
const uint8_t app_id = 31; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;

/*
  The following band table can be adjusted according to the user's preferences or local conditions. 
  All bands on this receiver use a custom method rather than the standard SI484X method. 
  The FM band has been divided into two bands. 
  There are two MW bands, allowing the receiver to be configured according to the regional band plan and regulations. 
  Finally, the SW bands can be modified to provide a better experience for users.
*/

typedef struct {
  int8_t bandIdx;
  uint16_t botton;    // botton frequency (10350 = 103.5Mhz; 9775 = 9,775 kHz)
  uint16_t top;       // top
  uint8_t bandSpace;  // FM only (10 = 100 kHz; 20 = 200 kHz)
  char *desc; 
} Band;

/*
  To add, remove, or modify a band, simply update the table below. Each row corresponds to a specific band. 
  Here's how the parameters are defined:

  1) First parameter: An internal value used by the SI474X to identify the band (see the Datasheet AN610 - Si48XX ATDD PROGRAMMING GUIDE page 17).
  2) Second and third parameters: These specify the lower and upper frequency limits for the band:
  3) For FM bands, the values are in MHz (e.g., 7700 means 77.00 MHz, and 10800 means 108.00 MHz).
  4) For SW bands, the values are in kHz (e.g., 4600 means 4,600 kHz or 4.6 MHz).
  5) If both frequencies are set to 0, the device will use its default internal frequencies for that band.
  6) Fourth parameter: Defines the frequency step size:
  7) For FM: 10 corresponds to 100 kHz spacing, and 20 corresponds to 200 kHz spacing.
  8) For SW: 5 corresponds to 5 kHz spacing.
  9) Fifth parameter: A descriptive name for the band, making it easier to identify.

  In the following example, except for the MW1 and MW2 bands, all bands have been customized and do not follow the SI484X device's internal defaults. 
  Based on the datasheet "AN610 - Si48XX ATDD PROGRAMMING GUIDE" pages 17 and 18, you can select additional bands or configure them according to 
  your preferences.

  Note that in the following example, the FM bands have been configured to be narrower than those typically found in commercial receivers. 
  This approach was adopted to enhance the tuning experience. By narrowing the bands, it became easier to tune a station using a 100K potentiometer, 
  which offers only 180 degrees of rotation. Commercial receivers usually employ mechanical reduction systems, such as pulleys or gears, to improve 
  the user experience by providing finer tuning control. Since this prototype lacks such mechanical aids, I chose to narrow the bands to compensate 
  and optimize usability.

*/
Band tabBand[] = { { 3, 8700, 10100, 20, (char *) "FM1" },      
                   { 3, 10100, 10900, 20, (char *) "FM2" },
                   { 20, 0,0,0, (char *) "MW1" },             
                   { 21, 0,0,0, (char *) "MW2" }, 
                   { 26, 4600, 5200,5, (char *) "SW1" },
                   { 26, 5700, 6200,5, (char *) (char *) "SW2"},
                   { 27, 7100, 7600,5, (char *) "SW3"},
                   { 28, 9200, 9990,5, (char *) "SW4"},
                   { 29, 11400,12200, 5, (char *) "SW5"},
                   { 34, 13400,13990, 5, (char *) "SW6"},
                   { 36, 15090,15700, 5, (char *) "SW7"},
                   { 40, 17400,17990, 5, (char *) "SW8"},
                   { 40, 21400,27890, 5, (char *) "SW9" } };

const int8_t lastBand = (sizeof tabBand / sizeof(Band)) - 1;
int8_t bandIdx = 0;

char *stmo[] = {(char *) "Mo", (char *) "S"}; 

extern uint8_t SmallFont[]; // Font Nokia
extern uint8_t BigNumbers[];


LiquidCrystal display(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


SI4844 rx;

void setup() {

  pinMode(BAND_UP, INPUT_PULLUP);
  pinMode(BAND_DOWN, INPUT_PULLUP);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DOWN, INPUT_PULLUP);
  pinMode(TOGGLE_VOL, INPUT_PULLUP);

  display.begin(16, 2);

  splash();              // Show Splash - Remove this line if you do not want it.

  // If you want to reset the eeprom, keep the VOLUME_UP button pressed during statup
  if (digitalRead(BAND_UP) == LOW)
  {
    EEPROM.update(eeprom_address, 0);
    // nokia.print((char *)"EEPROM RESET",0,0);
    // nokia.update();
    delay(2000);
    // nokia.clrScr();
    // nokia.update();
  }


  // Start the SI484X device and link it to the MCU pins.  
  // -1 means no band is selected here and 400000 means 400kHz I2C bus speed 
  rx.setup(RESET_PIN, INTERRUPT_PIN, -1, 400000);

  if (EEPROM.read(eeprom_address) == app_id)
    readAllReceiverInformation();
  else
    rx.setVolume(48);

  // Select the band. See tabBand array structure 
  rx.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top, tabBand[bandIdx].bandSpace);
  showStatus();

}

/*
 *  writes the conrrent receiver information into the eeprom.
 *  The EEPROM.update avoid write the same data in the same memory position. It will save unnecessary recording.
 */
void saveAllReceiverInformation()
{
  EEPROM.update(eeprom_address, app_id);             // stores the app id;
  EEPROM.update(eeprom_address + 1, rx.getVolume()); // stores the current Volume
  EEPROM.update(eeprom_address + 2, bandIdx);        // Stores the current band index
}

void readAllReceiverInformation()
{
  rx.setVolume(EEPROM.read(eeprom_address + 1)); // Gets the stored volume;
  bandIdx = EEPROM.read(eeprom_address + 2);
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


void splash() {
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
}


void showStatus() {
  char *mode;

  display.clear();

  showFrequency();

  display.setCursor(0, 1);
  display.print(rx.getBandMode());

  display.setCursor(13, 0);
  display.print(tabBand[bandIdx].desc);

  if (rx.getFrequencyInteger() > 999) 
    mode = (char *) "MHZ";
  else
    mode = (char *) "kHz";  
  
  display.setCursor(12, 1);
  display.print(mode);

  if (rx.getStatusBandMode() == 0) {
    display.setCursor(13, 0);
    display.print(stmo[rx.getStatusStereo()]);
  }

  if ( rx.getStatusStationIndicator() != 0) {
      display.setCursor(7, 0); 
      display.print((char *)"OK");
  }

}


/**
 * Shows frequency information on Display
 */
void showFrequency() {
  display.setCursor(5, 1);
  display.print(rx.getFormattedFrequency(2,'.'));
}

void nextBand() {
  if (bandIdx < lastBand)
    bandIdx++;
  else
    bandIdx = 0;

  if ( tabBand[bandIdx].botton == 0) 
    rx.setBand(tabBand[bandIdx].bandIdx);
  else    
    rx.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top, tabBand[bandIdx].bandSpace);

  delay(MINIMUM_DELAY);
  showStatus();
  saveAllReceiverInformation();
}

void previousBand() {
  if (bandIdx > 0)
    bandIdx--;
  else
    bandIdx = lastBand;

  if ( tabBand[bandIdx].botton == 0) 
    rx.setBand(tabBand[bandIdx].bandIdx);
  else    
    rx.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top, tabBand[bandIdx].bandSpace);    

  delay(MINIMUM_DELAY);
  showStatus();
  saveAllReceiverInformation();
}

void audioControlUp() {
  if (toggle)
    rx.changeVolume('+');
  else
    rx.bassTrebleUp();
  delay(MINIMUM_DELAY);
  showStatus();
}

void audioControlDown() {
  if (toggle)
    rx.changeVolume('-');
  else
    rx.bassTrebleDown();
  delay(MINIMUM_DELAY);
}


/**
 * Main loop
 */
void loop() {

  if (digitalRead(BAND_UP) == LOW)
    nextBand();  // goes to the next band.
  else if (digitalRead(BAND_DOWN) == LOW)
    previousBand();  // goes to the previous band.
  else if (digitalRead(VOL_UP) == LOW)
    audioControlUp();
  else if (digitalRead(VOL_DOWN) == LOW)
    audioControlDown();
  else if (digitalRead(TOGGLE_VOL) == LOW) {
    toggle = !toggle;
    delay(MINIMUM_DELAY);
  }

  if (rx.hasStatusChanged())
    showStatus();

  delay(50);
}

