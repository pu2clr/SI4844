/**
 *  This sketch runs on Seeeduino devices.
 *  I2C OLED Display and buttons Example
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
 *
 *  Author: Ricardo Lima Caratti (PU2CLR)
 *  Oct, 2019
*/

#include <SI4844.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DSEG7_Classic_Regular_16.h"



// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 6


/*
   Pre-defined Band Table - See the SI48XX ATDD - PROGRAMMING GUIDE page 17.
*/

String tabBand[] = { "FM1", "FM1", "FM1", "FM1", "FM2", "FM2", "FM2", "FM2", "FM3", "FM3", "FM3", "FM3","FM4", "FM4", "FM4", "FM4", "FM5", "FM5", "FM5", "FM5",
                     "AM1", "AM2", "AM3", "AM4", "AM5", 
                     "SW1", "SW2", "SW3", "SW4", "SW54", "AM6", "SW7", "SW8", "SW9", "SW10", "SW11", "AM12", "SW13", "SW14", "SW15", "SW16" };   

const int8_t lastBand = (sizeof tabBand / sizeof(String)) - 1;
int8_t bandIdx = 0;



// OLED - Declaration for a SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

SI4844 si4844;

si4844_status_response *rxStatus;

void setup()
{

  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS); // Address 0x3C for 128x32

  // display.display();
  display.setTextColor(SSD1306_WHITE);


  display.clearDisplay();

 
  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // si4844.setCrystalOscillatorStabilizationWaitTime(1);
  si4844.setup(RESET_PIN, INTERRUPT_PIN, -1, 100000);

  displayDial();

}


void displayDial()
{
  String unit, freqDisplay, stereo;

  rxStatus = si4844.getStatus();

  if (si4844.getFrequencyInteger() > 999) 
    unit = (char *) "MHZ";
  else
    unit = (char *) "kHz";  

  display.setFont(NULL);
  display.clearDisplay();

 
  display.setCursor(0, 0);
  display.print(si4844.getBandMode());

  display.setCursor(48, 0);  
  if ( si4844.getStatusStationIndicator() != 0) 
    display.print("OK");
  else 
    display.print("  ");

  bandIdx = rxStatus->refined.BANDIDX;
  
  display.setCursor(105, 0);  
  display.print(tabBand[bandIdx]);


  display.setFont(&DSEG7_Classic_Regular_16);

  display.setCursor(15, 30);
  display.print(si4844.getFormattedFrequency(2,'.'));
  display.setCursor(100, 20);
  display.setFont(NULL);
  display.print(" ");
  display.print(unit);
  
  // It does not make sense with Si4827  -- Temove the comment if you are using the Si4844 or si4840
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


void loop()
{
  if (si4844.hasStatusChanged())
    displayDial();
  
  delay(10);
}
