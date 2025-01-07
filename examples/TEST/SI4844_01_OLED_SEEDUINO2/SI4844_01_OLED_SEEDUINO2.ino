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
 *  Test with the mechanical band selection method (using a resistor network).
 *  In this approach, there is no need for push buttons or storing the receiver's status 
 *  in EEPROM. The system will start at the frequency and band determined by the tuning 
 *  potentiometer and the position of the band switch.
 *  NOTE:
 *  1) The Si4822/26/40/44 SSOP24 packaged parts have a pull up resistor option (at pin 1 LNA_EN) 
 *     to force the ATDD device to use its default band properties rather than the values programmed by the system controller.
 *     when the ATDD device pin 1 is pulled up, it will ignore the band properties programmed by the system.
 *  2) At power up, the system controller is required to read the band configuration state bits from the ATDD device and 
 *     determine which configuration option is responsible for the band detection. 
 *  3) The system controller is able to read this information from the band configuration state bits from the ATDD device.
 *  4) The Si4827 SOIC16 package ATDD part doesn't have the pin pull-up option. However, the host controller can send an 
 *     extra argument byte in the ATDD_POWER_UP command to specify this band properties priority.
 *  5)   
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
  si4844.setupSlideSwitch(RESET_PIN, INTERRUPT_PIN, 100000); 

  // You must calibrate the default volume
  si4844.setVolume(50);

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
  
  // Stereo status does not make sense with Si4827 
  #ifdef SI4844_DEVICE
  if ( si4844.getStatusBandMode() == 0) {
    display.setCursor(75, 25);
    if (si4844.getStatusStereo() == 1)
      display.print("Stereo");
    else   
      display.print("Mono  ");
  }
  #endif

  display.display();
}


void loop()
{
  if (si4844.hasStatusChanged()) {
    uint8_t newBand = si4844.getValidBandIndex();
    if (newBand != si4844.getCurrentBand() ) {
      if (si4844.needHostReset()) 
        si4844.setBand(newBand);
    }
    displayDial();
  }
  
  delay(50);
}
