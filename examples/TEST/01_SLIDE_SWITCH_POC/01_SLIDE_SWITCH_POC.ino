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

void setup() {
  
  si4844_device_status *ds;
  si4844_status_response *rs;

  int count = 0;
  Serial.begin(9600);
  delay(1000);
  Serial.print("\nStarting"); 

  rx.setupSlideSwitch(RESET_PIN, INTERRUPT_PIN);

  rx.reset(); // Step 1
  Serial.print("\nSystem Reseted"); 


  delay(2);
  ds = rx.getStatus();

  sprintf(str,"\nBCFG0.....: %d\nBCFG1.....: %d\nSTEREO....: %d\nSTATION...: %d\nINFORDY...: %d\nHOSTPWRUP.: %d \nHOSTRST...: %d\nCTS.......: %d", ds->refined.BCFG0, ds->refined.BCFG1, ds->refined.STEREO, ds->refined.STATION, ds->refined.INFORDY, ds->refined.HOSTPWRUP, ds->refined.HOSTRST, ds->refined.CTS );
  Serial.print(str);

  if ( ds->refined.BCFG0 == 0 ) {
    Serial.print("\nHardware configured to -ATDD device detects the band-"); 
  } else {
    Serial.print("\nHardware configured to -MCU detects the band-"); 
  }

  if (ds->refined.HOSTRST == 1) {
    Serial.print("\nResetting Again.....");
    rx.reset();
    delay(100);
  }
  delay(200);
  rx.powerUp();
  Serial.print("\nPowerUp 1 Finished");
  
  do { 
    delay(2);
    ds = rx.getStatus();
  } while (ds->refined.INFORDY == 0);
  Serial.print("\nCheck Status Finished");
  
  rs = rx.getAllReceiverInfo();

  sprintf(str,"\nBCFG0.....: %d\nBCFG1.....: %d\nSTEREO....: %d\nSTATION...: %d\nINFORDY...: %d\nHOSTPWRUP.: %d \nHOSTRST...: %d\nCTS.......: %d\nCount.....: %d", ds->refined.BCFG0, ds->refined.BCFG1, ds->refined.STEREO, ds->refined.STATION, ds->refined.INFORDY, ds->refined.HOSTPWRUP, ds->refined.HOSTRST, ds->refined.CTS, count );
  Serial.print(str);

  sprintf(str,"\nBand Mode.: %d\nBand Idx..: %d", rs->refined.BANDMODE, rs->refined.BANDIDX);
  Serial.print(str);

  if (ds->refined.HOSTRST == 1) {
    Serial.print("\nResetting Again.....");
    rx.reset();
    delay(100);
  }

  rx.setBandSlideSwitch();
   Serial.print("\nSet Band finished");
  rx.setVolume(50);


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

  
  delay(10);
}

