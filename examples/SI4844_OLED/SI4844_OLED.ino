/*
   I2C OLED Display and buttons Example


   Arduino Pro Mini 3.3V (8MHz) and SI4844 pin connections

   | SI4844 pin |  Arduino pin |  Description                                       |
   | ---------  | ------------ | ------------ ------------------------------------- |
   |    2       |   2          | Arduino interrupt pin                              |
   |   15       |  12          | Regurlar arduino digital pin used to RESET control |
   |   16       |  A4 (SDA)    | I2C bus (Data)                                     |
   |   17       |  A5 (SCL)    | I2C bus (Clocl)                                    |

   Author: Ricardo Lima Caratti (PU2CLR)
   Oct, 2019
*/
#include <SI4844.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12
#define DEFAULT_BAND 0

#define BAND_UP 5   // Next Band
#define BAND_DOWN 6 // Previous Band
#define VOL_UP 7    // Volume Volume Up
#define VOL_DOWN 8  // Volume Down

#define MIN_ELAPSED_TIME 100

long elapsedButton = millis();

// SI4844 Band Plan
// See Si48XX ATDD PROGRAMMING GUIDE, Table 8, pages 17 and 18
byte bandPlan[] = {4, 20, 26, 27, 28, 29, 31, 33, 35, 37, 39, 40};
const int maxBand = (sizeof(bandPlan) / sizeof(byte)) - 1;
int idxBand = 0;

// OLED - Declaration for a SSD1306 display connected to I2C (SDA, SCL pins)
SSD1306AsciiAvrI2c display;
SI4844 si4844;

void setup()
{
  pinMode(BAND_UP, INPUT_PULLUP);
  pinMode(BAND_DOWN, INPUT_PULLUP);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DOWN, INPUT_PULLUP);

  display.begin(&Adafruit128x64, I2C_ADDRESS);
  display.setFont(Adafruit5x7);
  display.set2X();
  display.clear();
  display.print("\n PU2CLR");
  delay(1500);
  display.clear();
  delay(500);

  si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
  si4844.setVolume(55);
  displayDial();
}

void displayDial()
{
  float freqSi4844;
  String unit, freqDisplay, stereo;
  byte bandMode;

  bandMode = si4844.getStatusBandMode();
  freqSi4844 = si4844.getFrequency();

  if (bandMode == 0)
  {
    unit = "MHz";
    // Formatting the frequency to show on display
    freqDisplay = String((freqSi4844 / 1000.0), 1);
    freqDisplay.replace(".", ",");
    display.setCursor(5, 6);
    display.print("Stereo ");
    display.print(si4844.getStereoIndicator());
  }
  else
  {
    unit = "KHz";
    freqDisplay = (bandMode == 2) ? String((freqSi4844 / 1000.0), 3) : String(freqSi4844, 0);
    if (bandPlan[idxBand] == 40 || bandPlan[idxBand] == 26 || bandPlan[idxBand] == 27)
    {
      display.setCursor(23, 6);
      display.set1X();
      display.print("Custom Band");
    }
  }

  display.set2X();
  display.setCursor(0, 0);
  display.print(si4844.getBandMode());
  display.print("      ");
  display.print(unit);
  display.setCursor(29, 3);
  display.print(freqDisplay);
  display.print(" ");
}

void setBand(byte cmd)
{
  if (cmd == '+')
    idxBand = (idxBand < maxBand) ? (idxBand + 1) : 0;
  else
    idxBand = (idxBand > 0) ? (idxBand - 1) : maxBand;

  if (bandPlan[idxBand] == 40)
  {
    // Configure the Pre-defined Band (band index 40) to work between 27.0 to 27.5 MHz
    // See Si48XX ATDD PROGRAMMING GUIDE, pages 17,18,19 and 20.
    si4844.setCustomBand(40, 27000, 27500, 5);
  }
  else if (bandPlan[idxBand] == 26)
  {
    // Configure the Pre-defined Band (band index 26) to work between 4.5 to 5.2 MHz
    si4844.setCustomBand(26, 4500, 5200, 5);
  }
  else if (bandPlan[idxBand] == 27)
  {
    // Configure the Pre-defined Band (band index 27) to work between 5.7 to 6.3 MHz
    si4844.setCustomBand(27, 5700, 6300, 5);
  }
  else
  {
    si4844.setBand(bandPlan[idxBand]);
  }
  display.clear();
  elapsedButton = millis();
}

void setVolume( char v) {
  si4844.changeVolume(v);
  elapsedButton = millis();
}

void loop()
{
  if ( (millis() - elapsedButton) > MIN_ELAPSED_TIME ) {
    // check if some button is pressed
    if (digitalRead(BAND_UP) == LOW )
      setBand('+');
    else if (digitalRead(BAND_DOWN) == LOW )
      setBand('-');
    else if (digitalRead(VOL_UP) == LOW )
      setVolume('+');
    else if (digitalRead(VOL_DOWN) == LOW )
      setVolume('-');
  }

  if (si4844.hasStatusChanged())
    displayDial();
  
  delay(10);
}
