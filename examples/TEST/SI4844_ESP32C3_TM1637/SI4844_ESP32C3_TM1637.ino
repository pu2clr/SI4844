/**
 *  This Sketch was designed to be compiled and installed on the ESP32C3 (Super Mini). 
 *  However, it can be easily adapted for another ESP32 model, as well as a different 
 *  family of microcontrollers. Refer to the following table. It can guide you in making 
 *  the connections between the Si48XX and the ESP32C3.
 *
 *  It is important to note that this project aimed at transforming receivers based on the Si4825 
 *  (with an analog interface) into receivers with a digital interface using the Si4827. However, 
 *  You can easily adapt it to an standalone project. 
 *
 *  ESP32C3 and Si4827 pin connections
 *
 *  | SI4827 pin | ESP32C3 pin   |  Description                                       |
 *  | ---------  | ------------  | -------------------------------------------------  |
 *  |    1       |  GPIO2        | ESP32C3 interrupt pin                              |
 *  |    9       |  GPIO3        | RESET                                              |
 *  |   10       |  GPIO9 (SDA)  | I2C bus (Data)                                     |
 *  |   11       |  GPIO8 (SCL)  | I2C bus (Clock)                                    | 
 *  | -----------| ------------- | ---------------------------------------------------|
 *  |  TM1637    |               |  7 Seg. Display based on TM1637                    |
 *  | -----------| ------------- | ---------------------------------------------------|                        
 *  |   Data     |  GPIO1        |                                                    |
 *  |   CLOCK    |  GPIO0        |                                                    |       
 *  | -----------| ------------- | ---------------------------------------------------|
 *  |Push Buttons|               |                                                    |
 *  | -----------| ------------- | ---------------------------------------------------| 
 *  |  BAND_UP   |     GPIO6     | Next Band                                          |                          
 *  |  BAND_DOWN |     GPIO5     | Previous Band                                      | 
 *  | -----------| ------------- | ---------------------------------------------------| 
 *  |  LEDs      |               |                                                    |
 *  | -----------| ------------- | ---------------------------------------------------| 
 *  |  DIAL      |     3         | Indicates that the analog dial shows the current frequency, matching the one displayed on the screen |
 *  |  TUNE      |    10         | Indicates that the receiver is tuned in a valid channel |
 *  Author: Ricardo Lima Caratti (PU2CLR)
 *  Feb, 2025
*/

#include <SI4844.h>
#include <EEPROM.h> 
#include <TM1637TinyDisplay6.h> // Install this library

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2 // GPIO2
#define RESET_PIN 3     // GPIO3

#define CLK_PIN 8   // GPIO8
#define SDA_PIN 9   // GPIO9

#define DIAL_INDICATOR 3
#define TUNE_INDICATOR 10

#define TM1637_CLK  0   // GPIO0
#define TM1637_DIO  1   // GPIO1

#define BAND_UP 6    // GPIO6 - Next Band
#define BAND_DOWN 5  // GPIO 5 - Previous Band

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
  bool analogDial;    // True if the band is shown on the dial Panel
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
Band tabBand[] = { { 3, 6400, 8800, 20, (char *) "FM1", false },    
                   { 3, 8700, 10800, 20, (char *) "FM2", true },   
                   { 20, 0,0,0, (char *) "MW1", true },             
                   { 21, 0,0,0, (char *) "MW2", true }, 
                   { 25, 2300, 3200,  5, (char *) "90m", false},
                   { 25, 3200, 4200,  5, (char *) "80m", false},
                   { 25, 4750, 5050,  5, (char *) "60m", true },
                   { 26, 5950, 6200,  5, (char *) "49m", true },
                   { 27, 7000, 7600,  5, (char *) "41m", false},
                   { 29, 9200, 9990,  5, (char *) "31m", true },
                   { 31, 11600,12200, 5, (char *) "25m", true },
                   { 33, 13400,13990, 5, (char *) "22m", false},
                   { 35, 15000,15900, 5, (char *) "19m", true },
                   { 38, 17400,17990, 5, (char *) "16m", false},
                   { 40, 21400,21790, 5, (char *) "13m", false},
                   { 40, 24890,25100, 5, (char *) "12m", false},
                   { 40, 25600,26610, 5, (char *) "11m", false},
                   { 40, 27000,27700, 5, (char *) "11m", false},
                   { 40, 28000,28500, 5, (char *) "10m", false} };

const int8_t lastBand = (sizeof tabBand / sizeof(Band)) - 1;
int8_t bandIdx = 0;



TM1637TinyDisplay6 display(TM1637_CLK, TM1637_DIO);

SI4844 rx;

void setup()
{
  pinMode(BAND_UP, INPUT_PULLUP);
  pinMode(BAND_DOWN, INPUT_PULLUP);
  pinMode(DIAL_INDICATOR, OUTPUT);
  pinMode(TUNE_INDICATOR, OUTPUT);

  delay(200); // Needed to make the OLED starts
  display.begin();
  display.setBrightness(BRIGHT_LOW);
  display.showString("pu2clr");

  // RESET EEPROM
  if (digitalRead(BAND_UP) == LOW)
  {
    EEPROM.write(eeprom_address, 0);
    display.showString("rSt");
    delay(1500);
  }

  Wire.begin(SDA_PIN, CLK_PIN);  
    
  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // rx.setCrystalOscillatorStabilizationWaitTime(1);
  rx.setup(RESET_PIN, INTERRUPT_PIN, -1, 100000);
  
  if (EEPROM.read(eeprom_address) == app_id)
    readAllReceiverInformation();
  else
    rx.setVolume(60);
  

  if ( tabBand[bandIdx].botton != 0)
    rx.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top,tabBand[bandIdx].bandSpace);
  else 
    rx.setBand(tabBand[bandIdx].bandIdx);  

  digitalWrite(DIAL_INDICATOR, tabBand[bandIdx].analogDial); // Turn the LED ON or OFF if the current Band is shown on the Dial

  display.clear(); 
  displayDial();

}

/*
 *  writes the conrrent receiver information into the eeprom.
 *  The EEPROM.write avoid write the same data in the same memory position. It will save unnecessary recording.
 */
void saveAllReceiverInformation()
{
  EEPROM.write(eeprom_address, app_id);             // stores the app id;
  EEPROM.write(eeprom_address + 1, rx.getVolume()); // stores the current Volume
  EEPROM.write(eeprom_address + 2, bandIdx);        // Stores the current band index
  EEPROM.commit();
}

void readAllReceiverInformation()
{
  rx.setVolume(EEPROM.read(eeprom_address + 1)); // Gets the stored volume;
  bandIdx = EEPROM.read(eeprom_address + 2);

}

uint32_t oldFrequency = 0;
uint8_t oldStatusStationIndicator = 9;

void displayDial()
{
  float f = rx.getFrequency(); 

  display.showNumber(f / 1000., (rx.getStatusBandMode() == 0)? 2:3 );

  digitalWrite(TUNE_INDICATOR, (rx.getStatusStationIndicator() != 0) ); 
 
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

  digitalWrite(DIAL_INDICATOR, tabBand[bandIdx].analogDial); // Turn the LED ON or OFF if the current Band is shown on the Dial

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
  }

  if (rx.hasStatusChanged())
    displayDial();
  
  delay(10);
}
