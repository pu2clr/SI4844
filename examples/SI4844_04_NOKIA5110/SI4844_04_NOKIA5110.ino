/*

  This sketch compiles on the Arduino Nano, Uno, Pro Mini, and LGT8F328 (LGT8Fx board manager), 
  as well as on the ATmega328 standalone (MiniCore board manager).

  
  This sketch uses the EEPROM to store the last band used and the audio level. 
  This way, the radio will start on the band you were using when it was last turned off. 
  To reset the EEPROM, turn on the radio while holding down the "Next Band (BAND_UP)" button.

  See user_manual.txt before operating the receiver.

  Wire up on ATmega328 or Arduino Pro Mini or  Nano or  LGT8F328  and SI4844 and Nokia Diaplay

  |     Device name       | Device Pin / Description      |  Arduino Pin  |
  | --------------------- | ----------------------------- | ------------  |
  |   Display NOKIA 5110  |                               |               |
  |                       | (1) RST (RESET)               |     8         |
  |                       | (2) CE or CS                  |     9         |
  |                       | (3) DC or DO                  |    10         |
  |                       | (4) DIN or DI or MOSI         |    11         |
  |                       | (5) CLK                       |    13         |
  |                       | (6) VCC  (3V-5V)              |    +VCC       |
  |                       | (7) BL/DL/LIGHT               |    +VCC       |
  |                       | (8) GND                       |    GND        |
  |    SI4844             |                               |               |
  |                       | RESET (pin 15)                |     12        |
  |                       | INTERRUPT (pin 2)             |      2        |
  |                       | SDA (pin 16)                  |     A4        |
  |                       | SCL (pin 17)                  |     A5        |
  |    Buttons            |                               |               |
  |                       | Band + (Next Band)            |      3        |
  |                       | Band - (Previous Band)        |      4        |
  |                       | Vol/bass/treble +             |      5        |
  |                       | Vol/Bass/Treble -             |      6        | 
  |                       | Toggle Vol/bass/treble        |      7        |    

  By PU2CLR, Ricardo, Nov,  2024.
*/

#include <SI4844.h>
#include <LCD5110_Graph.h> // you can download this library on http://www.rinkydinkelectronics.com/library.php?id=47
#include <EEPROM.h>

#define RESET_PIN 12
#define INTERRUPT_PIN 2

#define BAND_UP 3
#define BAND_DOWN 4
#define VOL_UP 5
#define VOL_DOWN 6
#define TOGGLE_VOL 7

// NOKIA Display pin setup
#define NOKIA_RST 8   // RESET
#define NOKIA_CE 9    // Some NOKIA devices show CS
#define NOKIA_DC 10   //
#define NOKIA_DIN 11  // MOSI
#define NOKIA_CLK 13  // SCK
#define NOKIA_LED 0   // 0 if wired to +3.3V directly

#define MINIMUM_DELAY 200

long elapsedButton = millis();
bool toggle = true;

// EEPROM - Stroring control variables
const uint8_t app_id = 31; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;

/*
  The following band table can be adjusted according to the user's preferences or local conditions. 
  All bands on this receiver use a custom method rather than the standard SI484X method. 
  The FM band has been divided into two bands. 
  There are two MW bands, allowing the receiver to be configured according to the regional band plan and regulations. 
  Finally, the SW bands can be modified to provide a better experience for users.
*/

typedef struct {
  int8_t bandIdx;
  uint16_t botton;    // botton frequency (10350 = 103.5Mhz; 9775 = 9,775 kHz)
  uint16_t top;       // top
  uint8_t bandSpace;  // FM only (10 = 100 kHz; 20 = 200 kHz)
  char *desc; 
} Band;


Band tabBand[] = { { 3, 8700, 10100, 20, (char *) "FM1" },
                   { 3, 10100, 10900, 20, (char *) "FM2" },
                   { 20, 0,0,0, (char *) "MW1" },
                   { 21, 0,0,0, (char *) "MW2" }, 
                   { 26, 4600, 5200,5, (char *) "SW1" },
                   { 26, 5700, 6200,5, (char *) (char *) "SW2"},
                   { 27, 7100, 7600,5, (char *) "SW3"},
                   { 28, 9200, 9990,5, (char *) "SW4"},
                   { 29, 11400,12200, 5, (char *) "SW5"},
                   { 34, 13400,13990, 5, (char *) "SW6"},
                   { 36, 15090,15700, 5, (char *) "SW7"},
                   { 40, 17400,17990, 5, (char *) "SW8"},
                   { 40, 21400,27890, 5, (char *) "SW9" } };

const int8_t lastBand = (sizeof tabBand / sizeof(Band)) - 1;
int8_t bandIdx = 0;

char *stmo[] = {(char *) "Mono", (char *) "Stereo"}; 

extern uint8_t SmallFont[]; // Font Nokia
extern uint8_t BigNumbers[];

// Nokia 5110 display
LCD5110 nokia(NOKIA_CLK, NOKIA_DIN, NOKIA_DC, NOKIA_RST, NOKIA_CE);


SI4844 rx;

void setup() {

  pinMode(BAND_UP, INPUT_PULLUP);
  pinMode(BAND_DOWN, INPUT_PULLUP);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DOWN, INPUT_PULLUP);
  pinMode(TOGGLE_VOL, INPUT_PULLUP);

  nokia.InitLCD();
  nokia.setContrast(70); // 0 - 120 -> Set the appropriated value for your Nokia 5110 display
  splash();              // Show Splash - Remove this line if you do not want it.

  // If you want to reset the eeprom, keep the VOLUME_UP button pressed during statup
  if (digitalRead(BAND_UP) == LOW)
  {
    EEPROM.update(eeprom_address, 0);
    nokia.print((char *)"EEPROM RESET",0,0);
    nokia.update();
    delay(2000);
    nokia.clrScr();
    nokia.update();
  }


  // Start the SI484X device and link it to the MCU pins.  
  // -1 means no band is selected here and 400000 means 400kHz I2C bus speed 
  rx.setup(RESET_PIN, INTERRUPT_PIN, -1, 400000);

  if (EEPROM.read(eeprom_address) == app_id)
    readAllReceiverInformation();
  else
    rx.setVolume(48);

  // Select the band. See tabBand array structure 
  rx.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top, tabBand[bandIdx].bandSpace);
  showStatus();

}

/*
 *  writes the conrrent receiver information into the eeprom.
 *  The EEPROM.update avoid write the same data in the same memory position. It will save unnecessary recording.
 */
void saveAllReceiverInformation()
{
  EEPROM.update(eeprom_address, app_id);             // stores the app id;
  EEPROM.update(eeprom_address + 1, rx.getVolume()); // stores the current Volume
  EEPROM.update(eeprom_address + 2, bandIdx);        // Stores the current band index
}

void readAllReceiverInformation()
{
  rx.setVolume(EEPROM.read(eeprom_address + 1)); // Gets the stored volume;
  bandIdx = EEPROM.read(eeprom_address + 2);
}



void splash() {
  nokia.setFont(SmallFont);
  nokia.clrScr();
  nokia.print(F("PU2CLR"), 20, 25);
  nokia.update();
  delay(1000);
  nokia.clrScr();
  nokia.print(F("SI4844"), 0, 0);
  nokia.print(F("Arduino"), 0, 15);
  nokia.print(F("Library"), 0, 30);
  nokia.update();
  delay(1000);
  nokia.clrScr();
  nokia.update();
}


void showStatus() {
  char *mode;
  nokia.clrScr();
  nokia.drawLine(0, 10, 84, 10);
  nokia.drawLine(0, 38, 84, 38);
  showFrequency();
  nokia.setFont(SmallFont);
  nokia.print(rx.getBandMode(),0,0);
  nokia.print(tabBand[bandIdx].desc,63,0);
  if (rx.getFrequencyInteger() > 999) 
    mode = (char *) "MHZ";
  else
    mode = (char *) "kHz";  
  nokia.print(mode,63,40);

  if (rx.getStatusBandMode() == 0) {
    nokia.print(stmo[rx.getStatusStereo()],0,40);
  }

  if ( rx.getStatusStationIndicator() != 0) {
    nokia.print((char *)"OK",35,0);
  }

  nokia.update();
}


/**
 * Shows frequency information on Display
 */
void showFrequency() {
  char *p;
  uint8_t l = 0;
  nokia.setFont(BigNumbers);

  p = rx.getFormattedFrequency(2,'.');
  if (p[0] == ' ') l++;
  if (p[1] == ' ') l++;

  nokia.print(&p[l], 0+l, 13);
  nokia.update();
}

void nextBand() {
  if (bandIdx < lastBand)
    bandIdx++;
  else
    bandIdx = 0;

  if ( tabBand[bandIdx].botton == 0) 
    rx.setBand(tabBand[bandIdx].bandIdx);
  else    
    rx.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top, tabBand[bandIdx].bandSpace);

  delay(MINIMUM_DELAY);
  showStatus();
  saveAllReceiverInformation();
}

void previousBand() {
  if (bandIdx > 0)
    bandIdx--;
  else
    bandIdx = lastBand;

  if ( tabBand[bandIdx].botton == 0) 
    rx.setBand(tabBand[bandIdx].bandIdx);
  else    
    rx.setCustomBand(tabBand[bandIdx].bandIdx, tabBand[bandIdx].botton, tabBand[bandIdx].top, tabBand[bandIdx].bandSpace);    

  delay(MINIMUM_DELAY);
  showStatus();
  saveAllReceiverInformation();
}

void audioControlUp() {
  if (toggle)
    rx.changeVolume('+');
  else
    rx.bassTrebleUp();
  delay(MINIMUM_DELAY);
  showStatus();
}

void audioControlDown() {
  if (toggle)
    rx.changeVolume('-');
  else
    rx.bassTrebleDown();
  delay(MINIMUM_DELAY);
}


/**
 * Main loop
 */
void loop() {

  if (digitalRead(BAND_UP) == LOW)
    nextBand();  // goes to the next band.
  else if (digitalRead(BAND_DOWN) == LOW)
    previousBand();  // goes to the previous band.
  else if (digitalRead(VOL_UP) == LOW)
    audioControlUp();
  else if (digitalRead(VOL_DOWN) == LOW)
    audioControlDown();
  else if (digitalRead(TOGGLE_VOL) == LOW) {
    toggle = !toggle;
    delay(MINIMUM_DELAY);
  }

  if (rx.hasStatusChanged())
    showStatus();

  delay(50);
}

