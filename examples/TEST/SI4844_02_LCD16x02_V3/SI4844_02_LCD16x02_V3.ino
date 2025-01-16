/*

  This sketch compiles on the Arduino Nano, Uno, Pro Mini, and LGT8F328 (LGT8Fx board manager), 
  as well as on the ATmega328 standalone (MiniCore board manager).

  
  This sketch uses the EEPROM to store the last band used and the audio level. 
  This way, the radio will start on the band you were using when it was last turned off. 
  To reset the EEPROM, turn on the radio while holding down the "Next Band (BT_FM_BAND)" button.

  See user_manual.txt before operating the receiver.

  Wire up on ATmega328 or Arduino Pro Mini or  Nano or  LGT8F328  and SI4844 and LCD16x02 

 *  | SI4844 pin  |  Arduino pin |  Description                                       |
 *  | ---------   | ------------ | -------------------------------------------------  |
 *  |    2        |   2          | Arduino interrupt pin                              |
 *  |   15        |   12         | SI4844 RESET                                       |
 *  |   16        |  A4 (SDA)    | I2C bus (Data)                                     |
 *  |   17        |  A5 (SCL)    | I2C bus (Clock)                                    | 
 *  | ----------- | -------------| ---------------------------------------------------|
 *  |  LCD 16x02  |                                                                   |
 *  | ----------- | -------------| ---------------------------------------------------|                        
 *  |      D4     |     D4       | Arduino Digital Pin 7                              |
 *  |      D5     |     D5       | Arduino Digital Pin 6                              | 
 *  |      D6     |     D6       | Arduino Digital pin 5                              |
 *  |      D7     |     D7       | Arduino Digital Pin 4                              | 
 *  |      RS     |     D3       | Arduino Digital Pin 3 for LCD RESET controle       | 
 *  |      E/ENA  |     D13      | Arduino Digital Pin 13                             |
 *  |RW & VSS & K |     GND      |                                                    |
 *  | A & VDD     |    +Vcc      |                                                    |
 *  |      VO     |              | (see 5K tripot connection)                         |
 *  | ----------- | -------------| ---------------------------------------------------|
 *  |Push Buttons |                                                                   |                                                                     |
 *  | ----------- | -------------| ---------------------------------------------------| 
 *  | BT_FM_BAND  |     11       | Selects the FM BAND                                |                          
 *  | BT_AM_BAND  |     10       | Selects the AM BAND                                | 
 *  | BT_SW1_BAND |      9       | Selects SW1                                        | 
 *  | BT_SW2_BAND |      8       | Selects SW2                                                    |   
 * 
 *  ATTENTION: Arduino Nano and Uno are 5V based board. Check the board voltage you are using
 *  If you are using the LGT8F328 see: https://blog.eletrogate.com/tutorial-pro-mini-evb-lgt8f328p-arduino-ide/
 *
 *  By PU2CLR, Ricardo, Nov,  2024.
*/

#include <SI4844.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

#define RESET_PIN 12
#define INTERRUPT_PIN 2

#define BT_FM_BAND 11
#define BT_AM_BAND 10
#define BT_SW1_BAND 9
#define BT_SW2_BAND 8

// LCD 16x02 or LCD20x4 PINs
#define LCD_D7 7
#define LCD_D6 6
#define LCD_D5 5
#define LCD_D4 4
#define LCD_RS 3
#define LCD_E 13

#define MINIMUM_DELAY 200

long elapsedButton = millis();
bool toggle = true;

// EEPROM - Stroring control variables
const uint8_t app_id = 44;  // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;

String tabBand[] = { "FM ", "AM ", "SW1", "SW2" };

const int8_t lastBand = (sizeof tabBand / sizeof(String)) - 1;
int8_t bandIdx = 0;


char *stmo[] = { (char *)"Mo", (char *)"St" };

extern uint8_t SmallFont[];  // Font Nokia
extern uint8_t BigNumbers[];


LiquidCrystal display(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


SI4844 rx;

void setup() {

  pinMode(BT_FM_BAND, INPUT_PULLUP);
  pinMode(BT_AM_BAND, INPUT_PULLUP);
  pinMode(BT_SW1_BAND, INPUT_PULLUP);
  pinMode(BT_SW2_BAND, INPUT_PULLUP);

  display.begin(16, 2);

  // If you want to reset the eeprom, keep the VOLUME_UP button pressed during statup
  if (digitalRead(BT_FM_BAND) == LOW) {
    EEPROM.update(eeprom_address, 0);
    display.setCursor(0, 0);
    display.print((char *)"EEPROM RESET");
    delay(2000);
  }


  // Start the SI48XX device and link it to the MCU pins.
  // -1 means no band is selected here and 400000 means 400kHz I2C bus speed
  rx.setup(RESET_PIN, INTERRUPT_PIN, -1, 400000);

  if (EEPROM.read(eeprom_address) == app_id)
    readAllReceiverInformation();


  rx.setVolume(58);  // Please adjust it

  setBand(bandIdx);

  showStatus();
}

/*
 *  writes the conrrent receiver information into the eeprom.
 *  The EEPROM.update avoid write the same data in the same memory position. It will save unnecessary recording.
 */
void saveAllReceiverInformation() {
  EEPROM.update(eeprom_address, app_id);       // stores the app id;
  EEPROM.update(eeprom_address + 2, bandIdx);  // Stores the current band index
}

void readAllReceiverInformation() {
  bandIdx = EEPROM.read(eeprom_address + 2);
}



void showStatus() {
  char *mode;

  display.clear();

  showFrequency();

  display.setCursor(0, 0);
  display.print(rx.getBandMode());

  display.setCursor(13, 0);
  display.print(tabBand[bandIdx]);

  if (rx.getFrequencyInteger() > 999)
    mode = (char *)"MHZ";
  else
    mode = (char *)"kHz";

  display.setCursor(13, 1);
  display.print(mode);

  if (rx.getStatusBandMode() == 0) {
    display.setCursor(0, 1);
    display.print(stmo[rx.getStatusStereo()]);
  }

  if (rx.getStatusStationIndicator() != 0) {
    display.setCursor(7, 0);
    display.print((char *)"OK");
  }
}


/**
 * Shows frequency information on Display
 */
void showFrequency() {
  display.setCursor(5, 1);
  display.print(rx.getFormattedFrequency(2, '.'));
}


void setBand(uint8_t band) {

  if (band == 0)
    rx.setCustomBand(3, 8700, 10800, 20);  // Custom FM Band (from 87MHz to 108MHz and 200kHz step)
  else if (band == 1)
    rx.setBand(20);  // AM band
  else if (band == 2)
    rx.setCustomBand(26, 5900, 14000, 5);  // From 5.9 to 14.0 MHz
  else if (band == 3)
    rx.setCustomBand(26, 15000, 22000, 5);  // From 15.0 to 22.0 MHz

  saveAllReceiverInformation();

  delay(MINIMUM_DELAY);

}

/**
 * Main loop
 */
void loop() {

  if (digitalRead(BT_FM_BAND) == LOW) 
   setBand(bandIdx = 0);
  else if (digitalRead(BT_AM_BAND) == LOW) 
    setBand (bandIdx = 1);
  else if (digitalRead(BT_SW1_BAND) == LOW) 
    setBand(bandIdx = 2);
  else if (digitalRead(BT_SW2_BAND) == LOW) 
    setBand(bandIdx = 3);

  if (rx.hasStatusChanged())
    showStatus();

  delay(10);
}
