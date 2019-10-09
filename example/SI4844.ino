/*
 * It is a Proof of Concept (POC) of using Si4844 with the Arduino Library for Si4844.
 * 
 * Author: Ricardo Lima Caratti (PU2CLR)
 * September, 2019
 */
#include "SI4844.h"
#include <Wire.h>

SI4844 si4844; 

void setup() {
  
  Serial.begin(9600);
  delay(500);

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interrupt_hundler, RISING);
  
  Serial.println("Início");
  
  si4844.setup();

  Serial.println("Setup Concluído");

  Serial.println("-------------------------------------------------------------------------------");
  Serial.println("Type F to FM; A to AM; 1 to 7 to SW; + or - to sound volume; or I to firmware Information  ");
  Serial.println("-------------------------------------------------------------------------------");
}

void show_firmware_information() {

  Serial.println("\nSI4844 -  Firmware information\n");

  Serial.println("Final 2 digits of Part Number..: " + si4844.getFirmwarePartNumber());
  Serial.println("Firmware Major Revision........: " + si4844.getFirmwareMajorRevision());
  Serial.println("Firmware Minor Revision........: " + si4844.getFirmwareMinorRevision());
  Serial.println("Component Major Revision.......: " + si4844.getFirmwareComponentMajorRevision());
  Serial.println("Component Minor Revision.......: " + si4844.getFirmwareComponentMinorRevision());
  Serial.println("Chip Revision..................: " + si4844.getFirmwareChipRevision());

  Serial.println("*****************************");

}


void show_atdd_status() {

  Serial.println("\nStatus Atual do SI4844\n");

  Serial.println("CTS..................: " + si4844.getStatusCTS());
  Serial.println("Host Reset...........: " + si4844.getStatusHostReset());
  Serial.println("Host Power Up........: " + si4844.getStatusHostPowerUp());
  Serial.println("Information Ready....: " + si4844.getStatusInformationReady());
  Serial.println("Station Indicator....: " + si4844.getStatusStationIndicator());
  Serial.println("Stereo...............: " + si4844.getStatusStereo());
  Serial.println("BCFG1................: " + si4844.getStatusBCFG1());
  Serial.println("BCFG0................: " + si4844.getStatusBCFG0());

  Serial.println("*****************************");

}


void loop() {

  // Read from keyboar (Arduino Serial Monitor)
  // Band switch and sound volume control
  // It can be replaced by your keyboar, encoder or push button device.
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
    case 'f':
    case 'F':
      si4844.setBand(4); // FM band
      break;
    case 'a':
    case 'A':
      si4844.setBand(23); // AM band
      break;
    case '1':
      si4844.setBand(27); // SW1 band
      break;
    case '2':
      si4844.setBand(29); // SW2 band
      break;
    case '3':
      si4844.setBand(31); // SW3 band
      break;
    case '4':
      si4844.setBand(33); // SW4 band
      break;
    case '5':
      si4844.setBand(35); // SW5 band
      break;
    case '6':
      si4844.setBand(37); // SW6 band
      break;
    case '7':
      si4844.setBand(39); // SW7 band
      break;
    case '+': // sound volume control
    case '-':
      si4844.setVolume(key); // if key = '+' then up; if key = '-' then  down; other, default volume.
      break;
    case 'I': 
    case 'i':
      show_firmware_information();
      break;
    default:
      Serial.println("Type F to FM; A to AM; 1 to 7 to SW; + or - to sound volume; or I to firmware Information  ");
    }
  }

  if (si4844.hasStatusChanged())
  {
    Serial.print(si4844.getFrequency());
    Serial.println(" KHz")
  }

}
