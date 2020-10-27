/*
 * SI4844 radio. Bass, treble and volume test.  
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
 * 
 */
#include <SI4844.h>
// Tested on Arduino Pro Mini
#define INTERRUPT_PIN 2  
#define RESET_PIN 12
// Pages 17 and 18 from Si48XX ATDD PROGRAMMING GUIDE
#define DEFAULT_BAND 4  // FM => 0 to 19; AM => 20 to 24; SW => 25 to 40

SI4844 si4844; 

void setup() {
  Serial.begin(9600);
  Serial.println("Bass, treble and volume test.");
  delay(500);  

  si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
  si4844.setVolume(55); // It can be from 0 to 63.

  // See Si48XX ATDD PROGRAMMING GUIDE, page 21
  // 3 = Mixed mode 2 (bass/treble and digital volume coexist, max volume = 63) 
  // 0 = Stereo audio output (default)
  // 1 = {–0 dB, -0dB, –0 dB} i.e., adjacent points same volume levels
  // 0 = Adjacent points allow stereo separation and stereo indicator on (default) 
  // 0 = Set audio mode and settings
  si4844.setAudioMode(3,0,1,0,0);
  
  instructions();
}

// Shows instruções
void instructions() {
  Serial.println("---------------------------------------------------");
  Serial.println("Type + or - to sound volume");
  Serial.println("Type B to Bass; T to Treeble");  
  Serial.println("Type M to mute; U to unmute");
  Serial.println("---------------------------------------------------");
  delay(2000);
}

void loop() {
 if (Serial.available() > 0) {
    char key = Serial.read();
    switch (key)
    {
      case 'b':
      case 'B': 
       si4844.bassTrebleDown();
      break;
      case 't':
      case 'T':
        si4844.bassTrebleUp();
      break;
      case '+': // sound volume control
      si4844.volumeUp();
      break;
    case '-':
      si4844.volumeDown();
      break;  
    case 'M':
    case 'm': 
      si4844.setAudioMute(true);  
      break;
    case 'U':
    case 'u':
      si4844.setAudioMute(false);
      break;
    default: 
      instructions();
      break;  
    }
  }
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
