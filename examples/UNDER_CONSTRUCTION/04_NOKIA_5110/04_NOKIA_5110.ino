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

#include <SI4SI4844.h>
#include <EEPROM.h>

#include <Adafruit_GFX.h>      // Core graphics library
#include <Adafruit_PCD8544.h>  // See: https://www.electronoobs.com/eng_arduino_Adafruit_PCD8544.php

#define RESET_PIN 12 

// NOKIA Display pin setup
#define NOKIA_RST 8  // RESET
#define NOKIA_CE 9   // Some NOKIA devices show CS
#define NOKIA_DC 10  //
#define NOKIA_DIN 11 // MOSI
#define NOKIA_CLK 13 // SCK
#define NOKIA_LED 0  // 0 if wired to +3.3V directly


#define BAND_UP     3
#define BAND_DOWN   4
#define VOL_UP      5
#define VOL_DOWN    6
#define TOGGLE_VOL  7


// Nokia 5110 display
Adafruit_PCD8544 display = Adafruit_PCD8544(NOKIA_DC, NOKIA_CE, NOKIA_RST);


SI4844 rx;

void setup()
{
  // Encoder pins
  pinMode(BAND_UP, INPUT_PULLUP);
  pinMode(BAND_DOWN, INPUT_PULLUP);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOLUME_DOWN, INPUT_PULLUP);
  pinMode(TOGGLE_VOL, INPUT_PULLUP);

  // Start the Nokia display device
  display.begin();
  // ATTENTION: YOU MUST VERIFY THE BEST LAVEL FOR THE CONTRAST OF YOUR DISPLAY.
  display.setContrast(60);  // You may need adjust this value for you Nokia 5110
  splash();                 // Show Splash - Remove this line if you do not want it.


  showStatus();
}

/**
 * Shows splash message
 */
void splash()
{
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
  currentFrequency = rx.getFrequency();
  display.setTextSize(2);
  display.setCursor(3, 8);
  display.print(rx.getFormattedFrequency(2));
  display.display();
}


/**
 * Shows some basic information on display
 */
void showStatus() {

  display.fillRect(0, 0, 84, 23, WHITE);
  display.setTextColor(BLACK);
  showFrequency();
  display.display();
}

/*
 *  Shows the volume level on LCD
 */
void showVolume()
{

}




/**
 *   Sets Band up (1) or down (!1)
 */
void setBand(int8_t up_down)
{

}

/**
 * Main loop
 */
void loop()
{
  delay(10);
}
