/**
 *  This sketch runs on Arduino devices with a 7 Seg. TM1637 display  
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
 *  | TM1637     |             |                                       |
 *  | -----------| ------------| --------------------------------------|                        
 *  | CLK        | 6           | Arduino Digital Pin 6                 |
 *  | DIO        | 7           | Arduino Digital Pin 7                 |        
 *  | -----------| ------------| --------------------------------------|   
 *  |  Tune LED  |  10         | Optional Tune LED.Used to indicate    | 
 *  |            |             | stereo or valid station               |       
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
#include <TM1637TinyDisplay6.h> // Install this library

#define RESET_PIN 12
#define INTERRUPT_PIN 2
#define TUNE_LED 10

// TM1637 Connections
#define TM1637_CLK  6
#define TM1637_DIO  7


#define MINIMUM_DELAY 200

long elapsedButton = millis();

TM1637TinyDisplay6 display(TM1637_CLK, TM1637_DIO);
SI4844 rx;

void setup() {

  pinMode(TUNE_LED, OUTPUT);

  display.begin();
  display.clear();

  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // rx.setCrystalOscillatorStabilizationWaitTime(1);
  rx.setupSlideSwitch(RESET_PIN, INTERRUPT_PIN);

  // You must calibrate the default volume
  rx.setVolume(58);
  delay(100);
    
  showStatus();

}

uint32_t oldFrequency = 0L;
uint8_t oldStationIndicator = 99;

void showStatus() {

  float f = rx.getFrequency(); 

  display.showNumber(f / 1000., (rx.getStatusBandMode() == 0)? 2:3 );
 
  if (rx.getStatusBandMode() == 0)
    digitalWrite(TUNE_LED, (rx.getStatusStereo() == 0)? LOW:HIGH);
  else 
    digitalWrite(TUNE_LED, (rx.getStatusStationIndicator() == 0)? LOW:HIGH);

}



/**
 * Main loop
 */
void loop() {

  if (rx.hasStatusChanged()) {
    if (rx.hasBandChanged()) {
      oldFrequency = 0;
      oldStationIndicator = 99;
      rx.setBandSlideSwitch();
    }

    showStatus();  
  }

  delay(50);
}

