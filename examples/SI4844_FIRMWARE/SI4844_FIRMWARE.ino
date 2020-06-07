/*
 * SI4844 radio. Firmware information example. 
 * This Sketch shows the use of SI4844 Library to extract the device firmware information. 
 * 
 *  SI4844 and Arduino Pro Mini connections
 *  
 *  | SI4844 pin |  Arduino pin |  Description                                       |
 *  | ---------  | ------------ | -------------------------------------------------  |
 *  |    2       |   2          | Arduino interrupt pin                              |
 *  |   15       |  12          | Regurlar arduino digital pin used to RESET control |
 *  |   16       |  A4 (SDA)    | I2C bus (Data)                                     |
 *  |   17       |  A5 (SCL)    | I2C bus (Clocl)                                    |
 * 
 *  By Ricardo Lima Caratti (PU2CLR), Oct, 2019.
 */

#include <SI4844.h>
// Tested on Arduino Pro Mini
#define INTERRUPT_PIN 2  
#define RESET_PIN 12
// Pages 17 and 18 from Si48XX ATDD PROGRAMMING GUIDE
#define DEFAULT_BAND 0   // FM => 0 to 19; AM => 20 to 24; SW => 25 to 40

SI4844 si4844; 

void setup() {

  Serial.begin(9600);
  delay(500);  
  si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);

  Serial.println("\nSI4844 -  Firmware information\n");
  // See Si48XX ATDD PROGRAMMING GUIDE, page 22.   
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

void loop() {
  // If you move the tuner, hasStatusChanged will return true
  if (si4844.hasStatusChanged())
  {
    Serial.print("[Band..: ");
    Serial.print(si4844.getBandMode());
    Serial.print(" - Frequency: ");    
    Serial.print(si4844.getFrequency(),0);
    Serial.print(" KHz");
    if (si4844.getStatusBandMode() == 0) {
      Serial.print(" - Stereo ");
      Serial.print(si4844.getStereoIndicator());
    }
    Serial.println("]");
  }
}
