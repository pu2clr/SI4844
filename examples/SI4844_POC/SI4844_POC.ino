/*
 * It is a Proof of Concept (POC) of using Si4844 with the Arduino Library for Si4844.
 * Author: Ricardo Lima Caratti (PU2CLR)
 * September, 2019
 */
#include <SI4844.h>
// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12
#define DEFAULT_BAND 0

SI4844 si4844; 

void setup() {
  Serial.begin(9600);
  instructions();
  delay(500);
  si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
  si4844.setVolume(55);
}
// Shows instruções
void instructions() {
  Serial.println("---------------------------------------------------");
  Serial.println("Type F - FM; A - AM; 1 to 7 - SW1 to SW7");
  Serial.println("Type + or - to sound volume");
  Serial.println("Type B to Bass; T to Treeble and N to Normal");  
  Serial.println("Type c - custom band 5.7 to 6.2 MHz");
  Serial.println("Type C - CB (custom Band 27.0 to 27.5 MHz)");
  Serial.println("Type o to Power Down");
  Serial.println("Type I to Firmware Information  ");
  Serial.println("---------------------------------------------------");
  delay(2000);
}
// Shows firmware information
void show_firmware_information() {
  Serial.println("\nSI4844 -  Firmware information\n");
  si4844.getFirmware();
  Serial.print("Final 2 digits of Part Number..: ");
  Serial.println(si4844.getFirmwarePartNumber(), HEX);
  Serial.print("Firmware Major Revision........: ");
  Serial.println(si4844.getFirmwareMajorRevision());
  Serial.print("Firmware Minor Revision........: ");
  Serial.println(si4844.getFirmwareMinorRevision());
  Serial.print("Component Major Revision.......: ");
  Serial.println(si4844.getFirmwareComponentMajorRevision());
  Serial.print("Component Minor Revision.......: "); 
  Serial.println(si4844.getFirmwareComponentMinorRevision());
  Serial.print("Chip Revision..................: ");
  Serial.println(si4844.getFirmwareChipRevision());
  Serial.println("*****************************");
}
// Control
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
      si4844.setBand(0); // FM band
      break;
    case 'a':
    case 'A':
      si4844.setBand(20); // AM band
      break;
    case '1':
      si4844.setBand(28); // SW1 band
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
      si4844.volumeUp();
      break;
    case '-':
      si4844.volumeDown();
      break;  
    case 'o':
       Serial.println("Power Down");
       delay(500); 
      si4844.powerDown();
      break;  
    case 'c':
      // Configure the Pre-defined Band (band index 26) to work between 5.7 to 6.2 MHz
      // See Si48XX ATDD PROGRAMMING GUIDE, pages 17,18,19 and 20.
      Serial.println("Custom Band:  5.7 to 6.2 MHz");
      si4844.setCustomBand(26,5700,6200,5);  
      break;      
    case 'C': 
      // Configure the Pre-defined Band (band index 40) to work between 27.0 to 27.5 MHz
      // See Si48XX ATDD PROGRAMMING GUIDE, pages 17,18,19 and 20.
      Serial.println("Custom Band: 27.0 to 27.5 MHz");
      si4844.setCustomBand(40,27000,27500,5);  
      break;  
    case 'I': 
    case 'i':
      show_firmware_information();
      break;
    default:
      instructions();    
    }
  }
  if (si4844.hasStatusChanged())
  {
    Serial.print("Band Index: ");
    Serial.print(si4844.getStatusBandIndex());
    Serial.print(" - ");
    Serial.print(si4844.getBandMode());
    Serial.print(" - Frequency: ");    
    Serial.print(si4844.getFrequency(),0);
    Serial.print(" KHz");
    if (si4844.getStatusBandMode() == 0) {
      Serial.print(" - Stereo ");
      Serial.print(si4844.getStereoIndicator());
    }
    Serial.println("");
  }
}
