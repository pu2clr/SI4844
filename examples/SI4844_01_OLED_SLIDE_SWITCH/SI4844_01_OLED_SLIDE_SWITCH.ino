/**
 *  This sketch runs on Arduino devices with I2C OLED Display. 
 *  This project works on Si4822/26/27/40/44
 *
 * This application uses a mechanical switch-based band selection method and a resistor network ("ATDD device detects Band"). 
 * For this band selection approach, the BAND pin of the Si48XX or Si4827 must be connected to a resistor network configured 
 * as a voltage divider (similar to the Si4825 configuration). Although this hardware design is more complex, particularly 
 * due to the construction of the band selection switch and the higher project cost, there are some advantages to this approach:
 *
 * 1. There is no need to store the receiver's current settings in EEPROM for retrieval when the receiver is powered on, 
 *    simplifying the Arduino code needed to control the device.
 * 2. The mechanical switch offers the same tactile experience as vintage radios, making this project suitable for developing
 *    vintage-style models.
 * 3. To some extent, this model can be more user-friendly due to its simplified operation of the radio's features.
 *
 *  References: 
 *        Si4844-B DEMO BOARD USER’S GUIDE 
 *        Si4827 DEMO BOARD USER’S GUIDE
 *        Si4822/26/27/40/44 ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES - AN602
 *
 *
 *  Arduino and SI48XX pin connections
 *
 *  | SI4844 pin | Arduino pin |  Description                                       |
 *  | ---------  | ------------  | -------------------------------------------------  |
 *  |    2       |  2            | Arduino interrupt pin                            |
 *  |   15       |  12           | RESET                                              |
 *  |   16       |  A4 (SDA)     | I2C bus (Data)                                     |
 *  |   17       |  A5 (SCL)     | I2C bus (Clock)                                    | 
 *  | -----------| ------------- | ---------------------------------------------------|
 *  |  OLED      |               |                                                    |
 *  | -----------| ------------- | ---------------------------------------------------|                        
 *  |   SDA      |  A4           | It shares the I2C bus with the SI4844              |
 *  |   CLK      |  A5           | It shares the I2C bus with the SI4844              |       
 *  | -----------| ------------- | ---------------------------------------------------|
 *
 *  Author: Ricardo Lima Caratti (PU2CLR)
 *  Oct, 2019
*/

#include <SI4844.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DSEG7_Classic_Regular_16.h"


#define SI4844_DEVICE


// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1  // Define proper RST_PIN if required.

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12


/*
   Pre-defined Band Table - See the SI48XX ATDD - PROGRAMMING GUIDE page 17.
*/

String tabBand[] = { "FM1", "FM1", "FM1", "FM1", "FM2", "FM2", "FM2", "FM2", "FM3", "FM3", "FM3", "FM3", "FM4", "FM4", "FM4", "FM4", "FM5", "FM5", "FM5", "FM5",
                     "AM1", "AM2", "AM3", "AM4", "AM5",
                     "SW1", "SW2", "SW3", "SW4", "SW54", "AM6", "SW7", "SW8", "SW9", "SW10", "SW11", "AM12", "SW13", "SW14", "SW15", "SW16" };

const int8_t lastBand = (sizeof tabBand / sizeof(String)) - 1;
int8_t bandIdx = 0;



// OLED - Declaration for a SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

SI4844 rx;

int8_t newBand;


void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);  // Address 0x3C for 128x32

  // display.display();
  display.setTextColor(SSD1306_WHITE);


  display.clearDisplay();


  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // rx.setCrystalOscillatorStabilizationWaitTime(1);
  rx.setupSlideSwitch(RESET_PIN, INTERRUPT_PIN);

  // You must calibrate the default volume
  rx.setVolume(50);
  delay(100);
  displayDial();
}


void displayDial() {
  String unit, freqDisplay, stereo;

  display.setFont(NULL);
  display.setCursor(0, 0);
  display.print("TESTE");

  if (rx.getFrequencyInteger() > 999)
    unit = (char *)"MHZ";
  else
    unit = (char *)"kHz";

  display.setFont(NULL);
  display.clearDisplay();


  display.setCursor(0, 0);
  display.print(rx.getBandMode());

  display.setCursor(48, 0);
  if (rx.getStatusStationIndicator() != 0)
    display.print("OK");
  else
    display.print("  ");

  bandIdx = rx.getCurrentBand();


  display.setCursor(90, 0);
  display.print(bandIdx);
  display.print("-");
  display.print(tabBand[bandIdx]);


  display.setFont(&DSEG7_Classic_Regular_16);

  display.setCursor(15, 30);
  display.print(rx.getFormattedFrequency(2, '.'));
  display.setCursor(100, 20);
  display.setFont(NULL);
  display.print(" ");
  display.print(unit);


// Stereo status does not make sense with Si4827
#ifdef SI4844_DEVICE
  if (rx.getStatusBandMode() == 0) {
    display.setCursor(75, 25);
    if (rx.getStatusStereo() == 1)
      display.print("Stereo");
    else
      display.print("Mono  ");
  }
#endif

  display.display();
}


void loop() {

  if (rx.hasStatusChanged()) {
    if (rx.hasBandChanged()) {
      rx.setBandSlideSwitch();
    }

    displayDial();  
  }

  delay(50);
}
