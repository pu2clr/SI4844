/**
 *  This sketch runs on Arduino devices with a LCD16x02 Display. 
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
 *  | SI4844 pin | Arduino pin |  Description                          |
 *  | ---------  | ------------| ------------------------------------- |
 *  |    2       |  2          | Arduino interrupt pin                 |
 *  |   15       |  12         | RESET                                 |
 *  |   16       |  A4 (SDA)   | I2C bus (Data)                        |
 *  |   17       |  A5 (SCL)   | I2C bus (Clock)                       | 
 *  | -----------| ------------| --------------------------------------|
 *  |  OLED      |             |                                       |
 *  | -----------| ------------| --------------------------------------|                        
 *  |   SDA      |  A4         | It shares the I2C bus with the SI4844 |
 *  |   CLK      |  A5         | It shares the I2C bus with the SI4844 |       
 *  | -----------| ------------| --------------------------------------|
 *  |  Tune LED  |  10         | Optional Tune LED                     |  
 *
 *  Important: In this configuration, when using a slide switch or a rotary band switch, 
 *             the BAND pin of the Si48XX must be connected to the band switch instead of +VCC. 
 *             The BAND pin connected to the +VCC should be used if you want to select the band 
 *             via microcontroller using a push button or similar device.
 *             Also, take note of the LNA_EN pin: if it is pulled up with a 10K resistor, 
 *             the Si48XX will use the default parameters for the selected bands. 
 *             However, if the LNA_EN pin is left floating (not connected), the microcontroller can 
 *             apply custom parameters, such as lower and upper frequency limits. 
 *
 *  Author: Ricardo Lima Caratti (PU2CLR)
 *  Oct, 2019
 * 
*/

#include <SI4844.h>
#include <LiquidCrystal.h>

#define RESET_PIN 12
#define INTERRUPT_PIN 2
#define TUNE_LED 10

// LCD 16x02 or LCD20x4 PINs
#define LCD_D7 7
#define LCD_D6 6
#define LCD_D5 5
#define LCD_D4 4
#define LCD_RS 3
#define LCD_E  13

#define MINIMUM_DELAY 200

long elapsedButton = millis();


/*
   Pre-defined Band Table - See the SI48XX ATDD - PROGRAMMING GUIDE page 17.
*/

String tabBand[] = { "FM01", "FM02", "FM03", "FM04", "FM05", "FM06", "FM07", "FM08", "FM09", "FM10", "FM11", "FM12", "FM13", "FM14", "FM15", "FM16", "FM17", "FM18", "FM19", "FM20",
                     "AM01", "AM02", "AM03", "AM04", "AM05",
                     "SW01", "SW02", "SW03", "SW04", "SW05", "AM06", "SW07", "SW08", "SW09", "SW10", "SW11", "AM12", "SW13", "SW14", "SW15", "SW16" };

const int8_t lastBand = (sizeof tabBand / sizeof(String)) - 1;
int8_t bandIdx = 0;

char *stmo[] = {(char *) "Mo", (char *) "St"}; 

LiquidCrystal display(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

SI4844 rx;

void setup() {

  pinMode(TUNE_LED, OUTPUT);

  display.begin(16, 2);

  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // rx.setCrystalOscillatorStabilizationWaitTime(1);
  rx.setupSlideSwitch(RESET_PIN, INTERRUPT_PIN);

  // You must calibrate the default volume
  rx.setVolume(58);
  delay(100);
    
  showStatus();

}


void showStatus() {

  String unit, freqDisplay, stereo;

  
  display.clear();

  display.setCursor(0, 0);
  display.print(rx.getBandMode());

  display.setCursor(12, 0);
  bandIdx = rx.getStatusBandIndex();
  display.print(tabBand[bandIdx]);

  if (rx.getFrequencyInteger() > 999) 
    unit =  "MHZ";
  else
    unit =  "kHz";  
  
  display.setCursor(13, 1);
  display.print(unit);

  if (rx.getStatusBandMode() == 0) {
    display.setCursor(0, 1);
    display.print(stmo[rx.getStatusStereo()]);
  }

  display.setCursor(7, 0); 
  if ( rx.getStatusStationIndicator() != 0) {
      display.print((char *)"OK");
      digitalWrite(TUNE_LED, HIGH);
  } else { 
      display.print((char *)"  ");
      digitalWrite(TUNE_LED, LOW);
  }


  display.setCursor(5, 1);
  display.print(rx.getFormattedFrequency(2,'.'));  

}



/**
 * Main loop
 */
void loop() {

  if (rx.hasStatusChanged()) {
    if (rx.hasBandChanged()) {
      rx.setBandSlideSwitch();
    }

    showStatus();  
  }

  delay(50);
}

