/*

  Wire up on Arduino UNO, Nano, Pro mini or or LGT8F328

  | Device name     | Device Pin / Description |  Arduino Pin  |
  | ----------------| -------------------------| ------------  |
  |  TM1638         |                          |               |
  |                 | STB                      |    4          |
  |                 | CLK                      |    7          |
  |                 | DIO                      |    8          |
  |                 | VCC                      |    3.3V       |
  |                 | GND                      |    GND        |
  | SI4844 pin      |                                               |
  | --------------- | -------------------------| ------------- |
  |    2            | interrupt pin            |    2          | 
  |   15            | RESET control            |   12          |
  |   16            | I2C bus (Data)           |   A4 (SDA)    | 
  |   17            | I2C bus (Clocl)          |   A5 (SCL)    |
  
 */

#include <TM1638lite.h>
#include <SI4844.h>

#define INTERRUPT_PIN 2
#define RESET_PIN 12
#define DEFAULT_BAND 1

#define TM1638_STB   4
#define TM1638_CLK   7
#define TM1638_DIO   8

// TM1638 - Buttons controllers
#define BAND_BUTTON_FM  1       // S1 FM
#define BAND_BUTTON_AM  2       // S1 AM
#define BAND_BUTTON_SW1 3       // S1 SW1
#define BAND_BUTTON_SW2 4       // S2 SW2
#define BAND_BUTTON_SW3 5       // S3 SW3
#define BAND_BUTTON_SW4 6       // S4 SW4
#define VOLUME_UP 7             // S5 VOL +
#define VOLUME_DOWN 8           // S5 VOL -



TM1638lite tm(TM1638_STB, TM1638_CLK, TM1638_DIO);
SI4844 rx;

void setup() {

  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // rx.setCrystalOscillatorStabilizationWaitTime(1);
  rx.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
  showStatus();
  delay(200);
  rx.setVolume(48);
  showStatus();

}

void showStatus() {

}

/**
 * Shows the static content on  display
 */
void showSplash()
{
  const char *s7= "-SI4844-";
  for (int i = 0; i < 8; i++) {
    tm.setLED(i, 1);
    delay(200);
    tm.displayASCII(i, s7[i]);
    tm.setLED(i, 0);
    delay(200);
  }
  delay(1000);
  tm.reset();
}


void showFrequency()
{
  char bufferDisplay[9];
  bufferDisplay[8] = '\0';

  // TODO  

  for (int pos = 3; pos < 8; pos++ )
     tm.displayASCII(pos,bufferDisplay[pos-3]);

}


/**
 * Shows the current mode (FM, MW or SW)
 */
void showMode() {
    for (int i = 4; i < 8; i++ ) {
       tm.setLED (i, (i - 4) == rx.getStatusBandMode());
    }
}

void showRxStatus () {

    if (rx.getStatusBandMode() == 0 ) {
        tm.setLED (0, rx.getStatusStereo()); // Indicates Stereo or Mono 
    }


} 




void loop() {

    uint8_t tm_button = tm.readButtons();
    switch(tm_button) {
        case BAND_BUTTON_FM:       
        case BAND_BUTTON_AM:       
        case BAND_BUTTON_SW1:  
        case BAND_BUTTON_SW2:  
        case BAND_BUTTON_SW3:  
        case BAND_BUTTON_SW4:  
        case VOLUME_UP:  
        case VOLUME_DOWN: 
        default: 
        break;
    }

    delay(1);

}




