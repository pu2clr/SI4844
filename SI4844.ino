/*
 * It is a Proof of Concept (POC) of using Si4844  
 * 
 * To build this code I used the followed sources: 
 * 1. Radio project based on Si4844-A10: https://github.com/Derek123454
 * 2. How to Build an Arduino-Controlled AM/FM/SW Radio: https://www.allaboutcircuits.com/projects/build-an-arduino-controlled-am-fm-sw-radio/
 * 3. Silicon Labs; Si48XX ATDD PROGRAMMING GUIDE
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

  Serial.println("Setup COncluído");

}

/*
void show_status() {

  
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
*/

void loop() {

  if ( si4844.status_changed()  ) {
    Serial.println(si4844.get_frequency());
  }

}
