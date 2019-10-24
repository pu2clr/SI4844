/*
 * SI4844 radio. Minimal Sketch.  
 * By Ricardo Lima Caratti (PU2CLR), Oct, 2019.
 */
#include <SI4844.h>
// Tested on Arduino Pro Mini
#define INTERRUPT_PIN 2  
#define RESET_PIN 12
// Pages 17 and 18 from Si48XX ATDD PROGRAMMING GUIDE
#define DEFAULT_BAND 4   // FM => 0 to 19; AM => 20 to 24; SW => 25 to 40

SI4844 si4844; 

void setup() {
  Serial.begin(9600);
  Serial.println("Teste!!!!");
  delay(500);  
  si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
  si4844.setVolume(55); // It can be from 0 to 63.
}

void loop() {
  // If you move the tuner, hasStatusChanged returns true
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
