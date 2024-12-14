/**
 *  I2C OLED Display and buttons Example
 *
 * This sketch uses the EEPROM to store the last band used and the audio level. 
 * This way, the radio will start on the band you were using when it was last turned off. 
 * To reset the EEPROM, turn on the radio while holding down the "Next Band (BAND_UP)" button.
 *
 *  Arduino Pro Mini 3.3V (8MHz) or LGT8F328 and SI4827 pin connections
 *
 *  Arduino Nano and SI4827 pin connections
 *
 *  | SI4827 pin |  Arduino pin |  Description                                       |
 *  | ---------  | ------------ | -------------------------------------------------  |
 *  |    1       |   2          | Arduino interrupt pin                              |
 *  |    9       |   12         | Arduino A0 (Digital pin 14) for SI4827 RESET       |
 *  |   10       |  A4 (SDA)    | I2C bus (Data)                                     |
 *  |   11       |  A5 (SCL)    | I2C bus (Clock)                                    | 
 *  | -----------| -------------| ---------------------------------------------------|
 *  |  OLED      |              |                                                    |
 *  | -----------| -------------| ---------------------------------------------------|                        
 *  |   SDA      |  A4          | It shares the I2C bus with the SI4827              |
 *  |   CLK      |  A5          | It shares the I2C bus with the SI4827              |       
 *  | -----------| -------------| ---------------------------------------------------|
 *  |Push Buttons|                                                                   |
 *  | -----------| -------------| ---------------------------------------------------| 
 *  |  BAND_UP   |     8        | Next band                                          |                          
 *  |  BAND_DOWN |     9        | Previous band                                      | 
 *  |  BASS_UP   |    10        | Bass Treble control (Bass up)                      | 
 *  |  BASS_DOWN |    11        | Bass Treble control (Treble up)                    |  
 *
 *  ATTENTION: Arduino Nano and Uno are 5V based board. Check the board voltage you are using
 *  If you are using the LGT8F328 see: https://blog.eletrogate.com/tutorial-pro-mini-evb-lgt8f328p-arduino-ide/
 

 *  Author: Ricardo Lima Caratti (PU2CLR)
 *  Nov, 2022
*/

#include <SI4844.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include <EEPROM.h>

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12

#define BAND_UP 8    // Next Band
#define BAND_DOWN 9  // Previous Band
#define BASS_UP 10    // Bass  Up and Treble Down
#define BASS_DOWN 11  // Treble Up and Bass Down

#define MIN_ELAPSED_TIME 100

// EEPROM - Stroring control variables
const uint8_t app_id = 27; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;

long elapsedButton = millis();

/*
  The following band table can be adjusted according to the user's preferences or local conditions. 
  Most bands on this receiver use a custom method rather than the standard SI48XX method. 
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

  In the following example, except for the MW1 and MW2 bands, all bands have been customized and do not follow the SI48XX device's internal defaults. 
  Based on the datasheet "AN610 - Si48XX ATDD PROGRAMMING GUIDE" pages 17 and 18, you can select additional bands or configure them according to 
  your preferences.

  Note that in the following example, the FM bands have been configured to be narrower than those typically found in commercial receivers. 
  This approach was adopted to enhance the tuning experience. By narrowing the bands, it became easier to tune a station using a 100K potentiometer, 
  which offers only 180 degrees of rotation. Commercial receivers usually employ mechanical reduction systems, such as pulleys or gears, to improve 
  the user experience by providing finer tuning control. Since this prototype lacks such mechanical aids, I chose to narrow the bands to compensate 
  and optimize usability.

*/
Band tabBand[] = { { 3, 8700, 10810, 20, (char *) "FM " },      
                   { 20, 0,0,0, (char *) "MW1" },             
                   { 21, 0,0,0, (char *) "MW2" }, 
                   { 25, 4600, 5200,5, (char *) "60m" },
                   { 26, 5700, 6200,5, (char *) (char *) "49m"},
                   { 27, 7100, 7600,5, (char *) "41m"},
                   { 29, 9200, 9990,5, (char *) "31m"},
                   { 31, 11400,12200, 5, (char *) "25m"},
                   { 33, 13400,13990, 5, (char *) "22m"},
                   { 35, 15090,15700, 5, (char *) "19m"},
                   { 38, 17400,17990, 5, (char *) "16m"},
                   { 40, 21400,21790, 5, (char *) "13m" } };

const int8_t lastBand = (sizeof tabBand / sizeof(Band)) - 1;
int8_t bandIdx = 0;

char *stereoStatus[] = { (char *) "Mono  ", (char *) "Stereo"};

// OLED - Declaration for a SSD1306 display connected to I2C (SDA, SCL pins)
SSD1306AsciiAvrI2c display;
SI4844 rx;

void setup()
{
  pinMode(BAND_UP, INPUT_PULLUP);
  pinMode(BAND_DOWN, INPUT_PULLUP);
  pinMode(BASS_UP, INPUT_PULLUP);
  pinMode(BASS_DOWN, INPUT_PULLUP);
  display.begin(&Adafruit128x32, I2C_ADDRESS);
  display.setFont(Adafruit5x7);
  display.clear();

  // RESET EEPROM
  if (digitalRead(BAND_UP) == LOW)
  {
    EEPROM.update(eeprom_address, 0);
    display.setCursor(0,0);
    display.print((char *)"EEPROM RESET");
    delay(1500);
  }
  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // rx.setCrystalOscillatorStabilizationWaitTime(1);
  rx.setup(RESET_PIN, INTERRUPT_PIN, -1, 100000);

  if (EEPROM.read(eeprom_address) == app_id)
    readAllReceiverInformation();
  else
    rx.setBassTreble(4);

  rx.setVolume(40);

  if ( tabBand[bandIdx].botton != 0)
    rx.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top,tabBand[bandIdx].bandSpace);
  else 
    rx.setBand(tabBand[bandIdx].bandIdx);  

  displayDial();

}

/*
 *  writes the conrrent receiver information into the eeprom.
 *  The EEPROM.update avoid write the same data in the same memory position. It will save unnecessary recording.
 */
void saveAllReceiverInformation()
{
  EEPROM.update(eeprom_address, app_id);             // stores the app id;
  EEPROM.update(eeprom_address + 1, rx.getBassTreble()); // stores the current Volume
  EEPROM.update(eeprom_address + 2, bandIdx);        // Stores the current band index
}

void readAllReceiverInformation()
{
  rx.setBassTreble(EEPROM.read(eeprom_address + 1)); // Gets the stored volume;
  bandIdx = EEPROM.read(eeprom_address + 2);

}


void displayDial()
{
  String unit, freqDisplay, stereo;

  if (rx.getFrequencyInteger() > 999) 
    unit = (char *) "MHZ";
  else
    unit = (char *) "kHz";  


  display.set1X();

  display.setCursor(0, 0);
  display.print(rx.getBandMode());
  display.setCursor(50, 0);
  if ( rx.getStatusStationIndicator() != 0) 
    display.print("  OK ");
  else 
    display.print("     ");
  display.setCursor(110, 0);
  display.print(tabBand[bandIdx].desc);
  display.setCursor(110, 3);
  display.print(unit);
  display.set2X();
  display.setCursor(15, 2);
  display.print("       ");
  display.setCursor(15, 2);
  display.print(rx.getFormattedFrequency(2,'.'));


  /* Does not work for SI4827 (it is mono)
  display.setCursor(35, 3);
  if ( rx.getStatusBandMode() == 0) {
      display.print(stereoStatus[rx.getStatusStereo()]);
  }
  */

}

void setBand(byte cmd)
{
  if (cmd == '+')
    bandIdx = (bandIdx < lastBand) ? (bandIdx + 1) : 0;
  else
    bandIdx = (bandIdx > 0) ? (bandIdx - 1) : lastBand;

  if ( tabBand[bandIdx].botton != 0)
    rx.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top,tabBand[bandIdx].bandSpace);
  else 
    rx.setBand(tabBand[bandIdx].bandIdx);  

  display.clear();
  saveAllReceiverInformation();
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
    else if (digitalRead(BASS_UP) == LOW ) {
      rx.bassTrebleUp();
      elapsedButton = millis();
    }
    else if (digitalRead(BASS_DOWN) == LOW ) {
      rx.bassTrebleDown();
      elapsedButton = millis();
    }
  }

  if (rx.hasStatusChanged())
    displayDial();
  
  delay(10);
}
