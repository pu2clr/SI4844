/**
 *  This sketch runs on Arduino devices 
 *  This project works on Si4822/26/27/40/44
 *  
 *  This application uses a mechanical switch-based band selection method and a resistor network ("ATDD device detects Band"). 
 *  For this band selection approach, the BAND pin of the Si48XX or Si4827 must be connected to a resistor network configured 
 *  as a voltage divider (similar to the Si4825 configuration). Although this hardware design is more complex, particularly 
 *  due to the construction of the band selection switch and the higher project cost, there are some advantages to this approach:
 *
 *  1. There is no need to store the receiver's current settings in EEPROM for retrieval when the receiver is powered on, 
 *     simplifying the Arduino code needed to control the device.
 *  2. The mechanical switch offers the same tactile experience as vintage radios, making this project suitable for developing
 *     vintage-style models.
 *  3. To some extent, this model can be more user-friendly due to its simplified operation of the radio's features.
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

#define RESET_PIN 12
#define INTERRUPT_PIN 2

#define MINIMUM_DELAY 200

SI4844 rx;

void setup() {

  Serial.begin(9600);
  delay(700);

  Serial.print(F("\nPOC SLIDE SWITCH Begin...\n"));

  // Customizing the bands of the Si48XX device.   
  rx.addCustomBand(2, 8400, 10100, 20);
  rx.addCustomBand(18, 10100, 10800, 20);
  rx.addCustomBand(25, 5700, 6200, 5);
  rx.addCustomBand(27, 7200, 7600, 5);
  rx.addCustomBand(31, 11500, 12150, 5);
  rx.addCustomBand(33, 13400, 13950, 5);
  rx.addCustomBand(35, 15100, 15750, 5);
  rx.addCustomBand(37, 17300, 17900, 5);
  rx.addCustomBand(39, 21400, 21700, 5);

  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  rx.setCrystalOscillatorStabilizationWaitTime(0); // Set - Waits 600 ms for the crystal to stabilize.
  rx.setupSlideSwitch(RESET_PIN, INTERRUPT_PIN);

  // rx.setAmSoftMuteMaxAttenuation(0); // disable Soft Mute
  rx.setAmSoftMuteMaxAttenuation(20); 

  // You must calibrate the default volume -  Assuming that the volume control is external.
  rx.setVolume(60);
  
  Serial.print(F("\nPOC SLIDE SWITCH Running...\n"));

  showStatus();

}


void showStatus() {
    Serial.print(F("Band Index: "));
    Serial.print(rx.getStatusBandIndex());
    Serial.print(F(" - "));
    Serial.print(rx.getBandMode());
    Serial.print(F(" - Frequency: "));    
    Serial.print(rx.getFrequencyInteger());
    Serial.print(F(" KHz"));
    if (rx.getStatusBandMode() == 0) {
      Serial.print(F(" - Stereo "));
      Serial.print(rx.getStereoIndicator());
    }
    Serial.print(F(" - Volume: "));
    Serial.print(rx.getVolume());
    Serial.println("");  
}


void loop() {

  if (rx.hasStatusChanged()) {
    if (rx.hasBandChanged()) {
      rx.setBandSlideSwitch();
    }

    showStatus();  
  }

  delay(10);
}

