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
 * 
 * Author: Ricardo Lima Caratti (PU2CLR)
 * September, 2019
 */
#include <SI4844.h>
#include <EEPROM.h>

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12
#define DEFAULT_BAND 1

#define STORE_TIME 10000 // Time of inactivity to make the current receiver status writable (10s / 10000 milliseconds).

// EEPROM - Stroring control variables
const uint8_t app_id = 31; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;
long storeTime = millis();


SI4844 rx; 

void setup() {
  Serial.begin(9600);
  delay(700);

  Serial.print(F("\nBegin...\n"));
 
  EEPROM.begin();

  instructions();
  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // rx.setCrystalOscillatorStabilizationWaitTime(1);
  rx.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
  if (EEPROM.read(eeprom_address) == app_id)
  {
    readAllReceiverInformation();
  } else 
    rx.setVolume(48);

  showStatus();
}

void saveAllReceiverInformation()
{
  int addr_offset;

  EEPROM.update(eeprom_address, app_id);                        // stores the app id;
  EEPROM.update(eeprom_address + 1, rx.getVolume());            // stores the current Volume
  EEPROM.update(eeprom_address + 2, rx.getStatusBandIndex());   // Stores the current band

}

void readAllReceiverInformation()
{
  rx.setVolume(EEPROM.read(eeprom_address + 1));
  rx.setBand(EEPROM.read(eeprom_address + 2));
}

// Shows instruções
void instructions() {
  Serial.println(F("---------------------------------------------------"));
  Serial.println(F("Type F - FM; A - AM; 1 to 7 - SW1 to SW7"));
  Serial.println(F("Type f - Custom FM - from 77 to 109 MHz - Step 200kHz"));
  Serial.println(F("Type h - Custom FM - from 101 to 104 MHz - Step 200kHz"));
  Serial.println(F("Type + or - to sound volume"));  
  Serial.println(F("Type B to Bass; T to Treeble and N to Normal"));  
  Serial.println(F("Type c - custom band 5.7 to 6.2 MHz"));
  Serial.println(F("Type C - CB (custom Band 27.0 to 27.5 MHz"));
  Serial.println(F("Type o to Power Down"));
  Serial.println(F("Type I to Firmware Information"));
  Serial.println(F("Type R to RESET the EEPROM"));
  Serial.println(F("---------------------------------------------------"));
  delay(500);
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

    char str[80];
    char aux[15];
    char* unt;
    if (rx.getStatusBandMode() == 0) {
      strcpy(aux,"Stereo ");
      strcat(aux, rx.getStereoIndicator() );
      unt = "MHZ";
    } else {
      strcpy(aux,"AM MONO");
      unt = "kHz";
    }
    sprintf(str,"\nMode: %d - Idx: %2.2d %s - Freq.: %s %s - %s - Vol.: %d", rx.getStatusBandMode(), rx.getStatusBandIndex(), rx.getBandMode(), rx.getFormattedFrequency(2), unt, aux, rx.getVolume() );
    Serial.print(str);  

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
    case 'F':
      rx.setBand(1); // FM band
      break;
    case 'f': 
      Serial.println(F("\nCustom FM Band:  from to 77 to 109 MHz - Step 200 kHz"));
      rx.setCustomBand(3,7700,10900,20);    
      break;
    case 'h': 
      Serial.println(F("\nCustom FM Band:  from to 101 to 104 MHz - Step 200 kHz"));
      rx.setCustomBand(3,10100,10400,20);  
      break;    
    case 'a':
    case 'A':
      rx.setBand(20); // AM band
      break;
    case '1':
      rx.setBand(28); // SW1 band
      break;
    case '2':
      rx.setBand(29); // SW2 band
      break;
    case '3':
      rx.setBand(31); // SW3 band
      break;
    case '4':
      rx.setBand(33); // SW4 band
      break;
    case '5':
      rx.setBand(35); // SW5 band
      break;
    case '6':
      rx.setBand(37); // SW6 band
      break;
    case '7':
      rx.setBand(39); // SW7 band
      break;
    case '+': // sound volume control
      rx.volumeUp();
      break;
    case '-':
      rx.volumeDown();
      break;  
    case 'o':
       Serial.println(F("\nPower Down"));
       delay(500); 
      rx.powerDown();
      break;  
    case 'c':
      // Configure the Pre-defined Band (band index 26) to work between 5.7 to 6.2 MHz
      // See Si48XX ATDD PROGRAMMING GUIDE, pages 17,18,19 and 20.
      Serial.println(F("\nCustom Band:  5.7 to 6.2 MHz"));
      rx.setCustomBand(26,5700,6200,5);  
      break;      
    case 'C': 
      // Configure the Pre-defined Band (band index 40) to work between 27.0 to 27.5 MHz
      // See Si48XX ATDD PROGRAMMING GUIDE, pages 17,18,19 and 20.
      Serial.println(F("\nCustom Band: 27.0 to 27.5 MHz"));
      rx.setCustomBand(40,27000,27500,5);  
      break;  
    case 'I': 
    case 'i':
      show_firmware_information();
      break;
    case 'R':
      EEPROM.update(eeprom_address, 0);
      Serial.println(F("\nEEPROM RESET..."));  
      break;  
    }
  }
  if (rx.hasStatusChanged())
  {
    showStatus();
    if ( (millis() - storeTime) > STORE_TIME  ) {
      storeTime = millis();
      saveAllReceiverInformation();
    }
  }
}
