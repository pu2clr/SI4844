/*
 * It is a Proof of Concept (POC) of using Si4844 with the Arduino Library for rx.
 * 
 *  SI4844 and Arduino Pro Mini, Arduino Nano, Uno or LGT8F328  connections
 *  
 *  | SI4844 pin |  Arduino pin |  Description                                       |
 *  | ---------  | ------------ | -------------------------------------------------  |
 *  |    2       |   2          | Arduino interrupt pin                              |
 *  |   15       |  12          | Regurlar arduino digital pin used to RESET control |
 *  |   16       |  A4 (SDA)    | I2C bus (Data)                                     |
 *  |   17       |  A5 (SCL)    | I2C bus (Clocl)                                    |
 * 
 *  ATTENTION: Arduino Nano and Uno are 5V based board. Check the board voltage you are using
 *  If you are using the LGT8F328 see: https://blog.eletrogate.com/tutorial-pro-mini-evb-lgt8f328p-arduino-ide/
 * 
 * Author: Ricardo Lima Caratti (PU2CLR)
 * September, 2019
 */
#include <SI4844.h>
// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12
#define DEFAULT_BAND 1

SI4844 rx; 

char str[160];

int8_t lastBandIdx;

void setup() {
  
  Serial.begin(9600);
  delay(700);

  Serial.print("\nStarting the system.");
  rx.setupSlideSwitch(RESET_PIN, INTERRUPT_PIN);
  rx.setVolume(40);
  sprintf(str,"\nSystem Started at the band %d, Band Mode %s, and current frequency %d ", rx.getCurrentBand(), rx.getBandMode(), rx.getFrequencyInteger());
  Serial.print(str);
  lastBandIdx = rx.getStatusBandIndex();

}



// Shows instruções
void instructions() {

}
// Shows firmware information
void show_firmware_information() {
  Serial.println(F("\nSI4844 -  Firmware information\n"));
  rx.getFirmware();
  Serial.print(F("Final 2 digits of Part Number..: "));
  Serial.println(rx.getFirmwarePartNumber(), HEX);
  Serial.print(F("Firmware Major Revision........: "));
  Serial.println(rx.getFirmwareMajorRevision());
  Serial.print(F("Firmware Minor Revision........: "));
  Serial.println(rx.getFirmwareMinorRevision());
  Serial.print(F("Component Major Revision.......: "));
  Serial.println(rx.getFirmwareComponentMajorRevision());
  Serial.print(F("Component Minor Revision.......: ")); 
  Serial.println(rx.getFirmwareComponentMinorRevision());
  Serial.print(F("Chip Revision..................: "));
  Serial.println(rx.getFirmwareChipRevision());
  Serial.println(F("*****************************"));
}

void showStatus() {

}

// Control
void loop() {

  if (rx.hasStatusChanged())
  { 
      rx.getAllReceiverInfo(); 
      if ( lastBandIdx != rx.getStatusBandIndex()) {
        Serial.print("\nNew Band");
        rx.setBandSlideSwitch();  
        lastBandIdx = rx.getStatusBandIndex();
      }
      sprintf(str,"\nSystem Started at the band %d, Band Mode %s\n", rx.getCurrentBand(), rx.getBandMode());
      Serial.print(str);  
      Serial.print(rx.getFrequencyInteger());    
  }
  
  delay(10);
}

