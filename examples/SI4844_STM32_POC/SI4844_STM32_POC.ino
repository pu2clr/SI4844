/*
* 
 *  SI4844 and STM32F1 connections
 *  
 *  | SI4844 pin |  STM32F1 pin |  Description                                       |
 *  | ---------  | ------------ | -------------------------------------------------  |
 *  |    2       |   PB4        | Interrupt pin                                      |
 *  |   15       |   PB5        | Regurlar digital pin used to RESET control         |
 *  |   16       |   PB7 (SDA)  | I2C bus (Data)                                     |
 *  |   17       |   PB6 (SCL)  | I2C bus (Clocl)                                    |
 * 
 *
 *   By Ricardo Lima Caratti, 2024.
*/

#include <SI4844.h>

#define RESET_PIN PB5
#define INTERRUPT_PIN PB4
#define DEFAULT_BAND 1

#define I2C_SDA   PB7
#define I2C_SCL   PB6


SI4844 rx; 

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.print(F("\nSTM32F1 - Starting...\n"));
  Serial.flush();

  // Wire.begin(I2C_SDA, I2C_SCL);
  Wire.begin(); 
  
  instructions();
  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // rx.setCrystalOscillatorStabilizationWaitTime(1);
  rx.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
  Serial.print(F("\nSTM32F1 - Working...\n"));
  showStatus();
  delay(200);
  rx.setVolume(48);
  showStatus();
}


// Shows instruções
void instructions() {
  Serial.println(F("-------------------STM32F1---CONTROLLER------------------"));
  Serial.println(F("Type F - FM; A - AM; 1 to 7 - SW1 to SW7"));
  Serial.println(F("Type f - Custom FM - from 77 to 109 MHz - Step 200kHz"));
  Serial.println(F("Type h - Custom FM - from 101 to 104 MHz - Step 200kHz"));
  Serial.println(F("Type + or - to sound volume"));  
  Serial.println(F("Type B to Bass; T to Treeble and N to Normal"));  
  Serial.println(F("Type c - custom band 5.7 to 6.2 MHz"));
  Serial.println(F("Type C - CB (custom Band 27.0 to 27.5 MHz"));
  Serial.println(F("Type o to Power Down"));
  Serial.println(F("Type I to Firmware Information"));
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
    Serial.print(F("Band Index: "));
    Serial.print(rx.getStatusBandIndex());
    Serial.print(F(" - "));
    Serial.print(rx.getBandMode());
    Serial.print(F(" - Frequency: "));    
    Serial.print(rx.getFrequency(),0);
    Serial.print(F(" KHz"));
    if (rx.getStatusBandMode() == 0) {
      Serial.print(F(" - Stereo "));
      Serial.print(rx.getStereoIndicator());
    }
    Serial.print(F(" - Volume: "));
    Serial.print(rx.getVolume());
    Serial.println("");  
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
      Serial.println(F("Custom FM Band:  from to 77 to 109 MHz - Step 200 kHz"));
      rx.setCustomBand(3,7700,10900,20);    
      break;
    case 'h': 
      Serial.println(F("Custom FM Band:  from to 101 to 104 MHz - Step 200 kHz"));
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
       Serial.println(F("Power Down"));
       delay(500); 
      rx.powerDown();
      break;  
    case 'c':
      // Configure the Pre-defined Band (band index 26) to work between 5.7 to 6.2 MHz
      // See Si48XX ATDD PROGRAMMING GUIDE, pages 17,18,19 and 20.
      Serial.println(F("Custom Band:  5.7 to 6.2 MHz"));
      rx.setCustomBand(26,5700,6200,5);  
      break;      
    case 'C': 
      // Configure the Pre-defined Band (band index 40) to work between 27.0 to 27.5 MHz
      // See Si48XX ATDD PROGRAMMING GUIDE, pages 17,18,19 and 20.
      Serial.println(F("Custom Band: 27.0 to 27.5 MHz"));
      rx.setCustomBand(40,27000,27500,5);  
      break;  
    case 'I': 
    case 'i':
      show_firmware_information();
      break;
    }
  }
  if (rx.hasStatusChanged())
  {
    showStatus();
  }
}

