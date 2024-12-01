/*

  This sketch compiles on the Arduino Nano, Uno, Pro Mini, and LGT8F328 (LGT8Fx board manager), 
  as well as on the ATmega328 standalone (MiniCore board manager).

  See user_manual.txt before operating the receiver.

  Wire up on Arduino Pro Mini, Nano, LGT8F328 and SI4844

  | Device name               | Device Pin / Description      |  Arduino Pin  |
  | ----------------          | ----------------------------- | ------------  |
  | Display NOKIA 5110        |                               |               |
  |                           | (1) RST (RESET)               |     8         |
  |                           | (2) CE or CS                  |     9         |
  |                           | (3) DC or DO                  |    10         |
  |                           | (4) DIN or DI or MOSI         |    11         |
  |                           | (5) CLK                       |    13         |
  |                           | (6) VCC  (3V-5V)              |    +VCC       |
  |                           | (7) BL/DL/LIGHT               |    +VCC       |
  |                           | (8) GND                       |    GND        |
  |     SI4844                |                               |               |
  |                           | RESET (pin 15)                |     12        |
  |                           | INTERRUPT (pin 2)             |      2        |
  |                           | SDA (pin 16)                  |     A4        |
  |                           | SCL (pin 17)                  |     A5        |

  |    Buttons                |                               |               |
  |                           | Band + (Next Band)            |      3        |
  |                           | Band - (Previous Band)        |      4        |
  |                           | Vol/bass/treble +             |      5        |
  |                           | Vol/Bass/Treble -             |      6        | 
  |                           | Toggle Vol/bass/treble        |      7        |    

  By PU2CLR, Ricardo, Nov,  2024.
*/

#include <SI4844.h>
// #include <EEPROM.h>

#include <Adafruit_GFX.h>      // Core graphics library
#include <Adafruit_PCD8544.h>  // See: https://www.electronoobs.com/eng_arduino_Adafruit_PCD8544.php

#define RESET_PIN 12
#define INTERRUPT_PIN 2

// NOKIA Display pin setup
#define NOKIA_RST 8   // RESET
#define NOKIA_CE 9    // Some NOKIA devices show CS
#define NOKIA_DC 10   //
#define NOKIA_DIN 11  // MOSI
#define NOKIA_CLK 13  // SCK
#define NOKIA_LED 0   // 0 if wired to +3.3V directly


#define BAND_UP 3
#define BAND_DOWN 4
#define VOL_UP 5
#define VOL_DOWN 6
#define TOGGLE_VOL 7

#define MIN_ELAPSED_TIME 100
#define MINIMUM_DELAY 200

long elapsedButton = millis();
bool toggle = true;

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
                   { 26, 4600, 5200, 5, (char *) "SW1" },
                   { 26, 0, 0, 0, (char *) (char *) "SW2"},
                   { 27, 0, 0, 0, (char *) "SW3"},
                   { 28, 0, 0, 0, (char *) "SW4"},
                   { 29, 0, 0, 0, (char *) "SW5"},
                   { 34, 0, 0, 0, (char *) "SW6"},
                   { 36, 0, 0, 0, (char *) "SW7"},
                   { 39, 0, 0, 0, (char *) "SW8"},
                   { 40, 21450, 21800, 5, (char *) "SW9" } };

const int8_t lastBand = (sizeof tabBand / sizeof(Band)) - 1;
int8_t bandIdx = 0;


// Nokia 5110 display
Adafruit_PCD8544 display = Adafruit_PCD8544(NOKIA_DC, NOKIA_CE, NOKIA_RST);


SI4844 rx;

void setup() {
  // Encoder pins
  pinMode(BAND_UP, INPUT_PULLUP);
  pinMode(BAND_DOWN, INPUT_PULLUP);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DOWN, INPUT_PULLUP);
  pinMode(TOGGLE_VOL, INPUT_PULLUP);

  // Start the Nokia display device
  display.begin();
  // ATTENTION: YOU MUST VERIFY THE BEST LAVEL FOR THE CONTRAST OF YOUR DISPLAY.
  display.setContrast(45);  // You may need adjust this value for you Nokia 5110
  splash();                 // Show Splash - Remove this line if you do not want it.

  rx.setup(RESET_PIN, INTERRUPT_PIN, 0);
  rx.setVolume(48);

  showStatus();
}

/**
 * Shows splash message
 */
void splash() {
  display.clearDisplay();
  display.display();
  display.setTextColor(BLACK);
  // Splash - Change it by the your introduction text.
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.print("SI4844");
  display.setCursor(0, 15);
  display.print("Arduino");
  display.setCursor(0, 30);
  display.print("Library");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}


/**
 * Shows frequency information on Display
 */
void showFrequency() {
  display.setTextSize(2);
  display.setCursor(3, 8);
  display.print(rx.getFormattedFrequency(2));
  display.display();
}


/**
 * Shows some basic information on display
 */
void showStatus() {
  rx.getStatusBandIndex();
  display.fillRect(0, 0, 84, 23, WHITE);
  display.setTextColor(BLACK);
  showFrequency();
  display.display();
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

  delay(5);
}
