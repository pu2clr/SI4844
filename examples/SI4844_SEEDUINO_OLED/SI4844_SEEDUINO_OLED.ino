/**
 *  This sketch runs on Seeeduino devices.
 *  I2C OLED Display and buttons Example
 *
 *  ABOUT SEEEDUINO XIAO and EEPROM
 *    The Seeeduino XIAO does no have internal EEPROM.
 *    To provide a convenient way to store and retrieve the reciver useful data, this sketch uses a library emulated-EEPROM with flash memory
 *    See FlashStorage_SAMD Arduino library on https://github.com/khoih-prog/FlashStorage_SAMD
 *

 *
 *  SEEEDUINO and SI4844 pin connections
 *
 *  | SI4844 pin | SEEEDUINO pin |  Description                                       |
 *  | ---------  | ------------  | -------------------------------------------------  |
 *  |    2       |  2            | SEEEDUINO interrupt pin                            |
 *  |   15       |  6            | RESET                                              |
 *  |   16       |  A4 (SDA)     | I2C bus (Data)                                     |
 *  |   17       |  A5 (SCL)     | I2C bus (Clock)                                    | 
 *  | -----------| ------------- | ---------------------------------------------------|
 *  |  OLED      |               |                                                    |
 *  | -----------| ------------- | ---------------------------------------------------|                        
 *  |   SDA      |  A4           | It shares the I2C bus with the SI4844              |
 *  |   CLK      |  A5           | It shares the I2C bus with the SI4844              |       
 *  | -----------| ------------- | ---------------------------------------------------|
 *  |Push Buttons|               |                                                    |                                                                     |
 *  | -----------| ------------- | ---------------------------------------------------| 
 *  |  BAND_UP   |     7         |                                                    |                          
 *  |  BAND_DOWN |     8         |                                                    | 
 *  |  VOL_UP    |     9         |                                                    | 
 *  |  VOL_DOWN  |    10         |                                                    |  
 *
 *  ATTENTION: Arduino Nano and Uno are 5V based board. Check the board voltage you are using
 *  If you are using the LGT8F328 see: https://blog.eletrogate.com/tutorial-pro-mini-evb-lgt8f328p-arduino-ide/
 

 *  Author: Ricardo Lima Caratti (PU2CLR)
 *  Oct, 2019
*/

#include <SI4844.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FlashAsEEPROM_SAMD.h> // Install this library from Github: https://github.com/khoih-prog/FlashStorage_SAMD#why-do-we-need-this-flashstorage_samd-library
#include "DSEG7_Classic_Regular_21.h"



// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 6

#define BAND_UP 7    // Next Band
#define BAND_DOWN 8  // Previous Band
#define VOL_UP 9    // Volume Volume Up
#define VOL_DOWN 10  // Volume Down

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
Band tabBand[] = { { 3, 6400, 8800, 20, (char *) "FM0" },
                   { 3, 8700, 10100, 20, (char *) "FM1" },      
                   { 3, 10100, 10900, 20, (char *) "FM2" },
                   { 20, 0,0,0, (char *) "MW1" },             
                   { 21, 0,0,0, (char *) "MW2" }, 
                   { 25, 2300, 3200,  5, (char *) "90m"},
                   { 25, 3200, 4200,  5, (char *) "80m"},
                   { 25, 4600, 5200,  5, (char *) "60m" },
                   { 26, 5700, 6200,  5, (char *) "49m"},
                   { 27, 7100, 7600,  5, (char *) "41m"},
                   { 29, 9200, 9990,  5, (char *) "31m"},
                   { 31, 11400,12200, 5, (char *) "25m"},
                   { 33, 13400,13990, 5, (char *) "22m"},
                   { 35, 15090,15700, 5, (char *) "19m"},
                   { 38, 17400,17990, 5, (char *) "16m"},
                   { 40, 21400,21790, 5, (char *) "13m"},
                   { 40, 24890,25100, 5, (char *) "12m"},
                   { 40, 25600,26610, 5, (char *) "11m"},
                   { 40, 27000,27700, 5, (char *) "11m"},
                   { 40, 28000,28500, 5, (char *) "11m"} };

const int8_t lastBand = (sizeof tabBand / sizeof(Band)) - 1;
int8_t bandIdx = 0;



// OLED - Declaration for a SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

SI4844 si4844;

void setup()
{
  pinMode(BAND_UP, INPUT_PULLUP);
  pinMode(BAND_DOWN, INPUT_PULLUP);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DOWN, INPUT_PULLUP);

  delay(150); // Needed to make the OLED starts
  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS); // Address 0x3C for 128x32
  display.clearDisplay();
  display.display();
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.print((char *)"PU2CLR\n\nRadio experiments");
  display.display();

  // RESET EEPROM
  if (digitalRead(BAND_UP) == LOW)
  {
    EEPROM.update(eeprom_address, 0);
    display.setCursor(0,0);
    display.print((char *)"EEPROM RESET");
    delay(1500);
  }
  
  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // si4844.setCrystalOscillatorStabilizationWaitTime(1);
  si4844.setup(RESET_PIN, INTERRUPT_PIN, -1, 100000);

  
  if (EEPROM.read(eeprom_address) == app_id)
    readAllReceiverInformation();
  else
    si4844.setVolume(60);
  

  if ( tabBand[bandIdx].botton != 0)
    si4844.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top,tabBand[bandIdx].bandSpace);
  else 
    si4844.setBand(tabBand[bandIdx].bandIdx);  


  displayDial();

}

/*
 *  writes the conrrent receiver information into the eeprom.
 *  The EEPROM.update avoid write the same data in the same memory position. It will save unnecessary recording.
 */
void saveAllReceiverInformation()
{
  EEPROM.update(eeprom_address, app_id);             // stores the app id;
  EEPROM.update(eeprom_address + 1, si4844.getVolume()); // stores the current Volume
  EEPROM.update(eeprom_address + 2, bandIdx);        // Stores the current band index
  EEPROM.commit();
}

void readAllReceiverInformation()
{
  si4844.setVolume(EEPROM.read(eeprom_address + 1)); // Gets the stored volume;
  bandIdx = EEPROM.read(eeprom_address + 2);

}

uint32_t oldFrequency = 0;
uint8_t oldStatusStationIndicator = 9;

void displayDial()
{
  String unit, freqDisplay, stereo;

  uint32_t currentFrequency = si4844.getFrequencyInteger();
  uint8_t currentStatusStationIndicator = si4844.getStatusStationIndicator() ;

  if ( oldFrequency == currentFrequency && oldStatusStationIndicator ==  currentStatusStationIndicator )
    return;

  oldFrequency = currentFrequency;
  oldStatusStationIndicator = currentStatusStationIndicator;

  if (si4844.getFrequencyInteger() > 999) 
    unit = (char *) "MHZ";
  else
    unit = (char *) "kHz";  

  display.setFont(NULL);
  display.clearDisplay();

 
  display.setCursor(3, 0);
  display.print(si4844.getBandMode());

  display.setCursor(48, 0);  
  if ( si4844.getStatusStationIndicator() != 0) 
    display.print("OK");
  else 
    display.print("  ");

  display.setCursor(105, 0);  
  display.print(tabBand[bandIdx].desc);


  display.setFont(&DSEG7_Classic_Regular_21);

  display.setCursor(16, 31);
  display.print(si4844.getFormattedFrequency(2,'.'));
  display.setCursor(104, 30);
  display.setFont(NULL);
  display.print(" ");
  display.print(unit);
  
  // It does not make sense with Si4827  -- Remove the comment if you are using the Si4844 or si4840
  /*
  if ( si4844.getStatusBandMode() == 0) {
    display.setCursor(75, 25);
    if (si4844.getStatusStereo() == 1)
      display.print("Stereo");
    else   
      display.print("Mono  ");
  }
  */

  display.display();
}

void setBand(byte cmd)
{
  if (cmd == '+')
    bandIdx = (bandIdx < lastBand) ? (bandIdx + 1) : 0;
  else
    bandIdx = (bandIdx > 0) ? (bandIdx - 1) : lastBand;

  if ( tabBand[bandIdx].botton != 0)
    si4844.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top,tabBand[bandIdx].bandSpace);
  else 
    si4844.setBand(tabBand[bandIdx].bandIdx);  

  saveAllReceiverInformation();
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
