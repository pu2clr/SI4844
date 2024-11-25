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
#define DEFAULT_FM_BAND 1

#define TM1638_STB 4
#define TM1638_CLK 7
#define TM1638_DIO 8

// TM1638 - Buttons controllers
#define BAND_BUTTON_FM 1   // S1 FM
#define BAND_BUTTON_AM 2   // S1 AM
#define BAND_BUTTON_SW1 4  // S1 SW1
#define BAND_BUTTON_SW2 8  // S2 SW2
#define BAND_BUTTON_SW3 16  // S3 SW3
#define BAND_BUTTON_SW4 32  // S4 SW4
#define VOLUME_UP 64        // S5 VOL +
#define VOLUME_DOWN 128      // S5 VOL -

int custom_fm_band = 1;
/*
TODO
int custom_sw1_band = 1; 
int custom_sw2_band = 1; 
int custom_sw3_band = 1; 
int custom_sw4_band = 1; 
*/

uint8_t selectBand = 1;

TM1638lite tm(TM1638_STB, TM1638_CLK, TM1638_DIO);
SI4844 rx;

void setup() {

  showSplash();
  // Some crystal oscillators may need more time to stabilize. Uncomment the following line if you are experiencing issues starting the receiver.
  // rx.setCrystalOscillatorStabilizationWaitTime(1);
  rx.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_FM_BAND);
  showStatus();
  rx.setVolume(48);
  showStatus();
}

void showStatus() {
  showFrequency();
  showMode();
  showRxSignal();
}

void showBand() {
  tm.displayASCII(0, selectBand + 48);
}

/**
 * Shows the static content on  display
 */
void showSplash() {
  const char* s7 = "-SI4844-";
  for (int i = 0; i < 8; i++) {
    tm.setLED(i, 1);
    delay(200);
    tm.displayASCII(i, s7[i]);
    tm.setLED(i, 0);
    delay(100);
  }
  delay(250);
  tm.reset();
}


void showFrequency() {
  char* pFreq;
  rx.getStatus();
  pFreq = rx.getFormattedFrequency(1);

  for (int pos = 2; pos < 8; pos++)
    tm.displayASCII(pos, pFreq[pos - 2]);
}


/**
 * Shows the current mode (FM, MW or SW)
 */
void showMode() {
  for (int i = 4; i < 8; i++) {
    tm.setLED(i, (i - 4) == (int) rx.getStatusBandMode());
  }
}

void showRxSignal() {

  if (rx.getStatusBandMode() == 0) {
    tm.setLED(0, rx.getStatusStereo());  // Indicates Stereo or Mono
  }
}




void loop() {

  uint8_t button = tm.readButtons(); 

  switch (button) {
    case BAND_BUTTON_FM:
      if (custom_fm_band == 0 ) {
        rx.setBand(DEFAULT_FM_BAND);  // FM band
         selectBand = 1;
      } else if ( custom_fm_band == 1 ) {
        rx.setCustomBand(3,8700,10100,20); 
        selectBand = 8;
      } else if ( custom_fm_band == 2 ) {
        rx.setCustomBand(3,10100,10900,20); 
        selectBand = 9;
      } 
      
      custom_fm_band++;
      if ( custom_fm_band > 2) custom_fm_band = 0;
     
      break;
    case BAND_BUTTON_AM:
      rx.setBand(20);  // AM band
      selectBand = 2;
      break;
    case BAND_BUTTON_SW1:
      rx.setBand(28);  // SW1 band
      selectBand = 3;
      break;
    case BAND_BUTTON_SW2:
      rx.setBand(29);  // SW2 band
      selectBand = 4;
      break;
    case BAND_BUTTON_SW3:
      rx.setBand(31);  // SW3 band
      selectBand = 5;
      break;
    case BAND_BUTTON_SW4:
      rx.setBand(35);  // SW5 band
      selectBand = 6;
      break;
    case VOLUME_UP:
      rx.volumeUp();
      break;
    case VOLUME_DOWN:
      rx.volumeDown();
      break;
    default:
      break;
  }

  if (rx.hasStatusChanged()) {
    showBand();
    showStatus();
  }
  delay(50);
}
