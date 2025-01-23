/*
 * SI4844 custom band example
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
 * By Ricardo Lima Caratti (PU2CLR), Oct, 2019.
 */
#include <SI4844.h>
// Tested on Arduino Pro Mini
#define INTERRUPT_PIN 2  
#define RESET_PIN 12
// Pages 17 and 18 from Si48XX ATDD PROGRAMMING GUIDE

SI4844 si4844; 

void setup() {
  Serial.begin(9600);
  delay(500);  

  Serial.print("\nStarting...\n");
  si4844.setCrystalOscillatorStabilizationWaitTime(0);
  si4844.setup(RESET_PIN, INTERRUPT_PIN, -1);

  // See Si48XX ATDD PROGRAMMING GUIDE, pages 17,18,19 and 20.

  // Legacy Method
  // si4844.setCustomBand(40, 27000, 27500, 5); // Defines and immediately selects Band 40 to operate between 27.0 and 27.5 MHz

  // New Method
  si4844.addCustomBand(40, 27000, 27500, 5); // Adds Band 40 with a frequency range of 27.0 to 27.5 MHz
  si4844.setBand(40);                        // Selects Band 40 for operation

  Serial.print("\nRunning...\n");
  showStatus();

}

void showStatus() {
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

void loop() {
  // If you move the tuner, hasStatusChanged returns true
  if (si4844.hasStatusChanged())
  {
    showStatus();
  }
}
