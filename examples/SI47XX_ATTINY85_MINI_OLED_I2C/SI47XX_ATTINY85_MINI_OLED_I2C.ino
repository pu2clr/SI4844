/*
  ATTENTION: Under construction...... 
  
  Test and validation of the SI4844 Arduino Library with ATtiny85.
  It is a simple AM/FM radio implementation.

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, Jan 2020.
*/

#include <SI4844.h>
#include <Tiny4kOLED.h>


#define INT_PIN     PB3     // Pin 2 is used to control interrupt.

#define RST_PIN     PB4     // Pin 3 is used to control the RESET of the device. 
#define BAND_SWITC  PB1     // Pin 6 is used to switch the band. 

#define AM 20
#define FM  0

byte currentBand = FM;
float currentFrequency;

SI4844 radio;

void setup()
{

  pinMode(BAND_SWITC, INPUT_PULLUP);
  pinMode(RST_PIN, INPUT_PULLUP);


  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT8X16);
  oled.setCursor(0, 0);
  oled.print("Si4844-Attiny85");
  oled.setCursor(0, 2);
  oled.print("   By PU2CLR   ");
  delay(2000);
  oled.clear();

  // radio.setup(INT_PIN, RST_PIN, currentBand);
  
  // Comment the line above and uncomment the line bellow to debug. See the function debugDevice dicumentation
  radio.debugDevice(RST_PIN, INT_PIN, 0, showMsg);

  
  radio.setVolume(30);


  showStatus();
}


void showMsg( char * msg ) {
  oled.clear();
  oled.setCursor(0, 0);
  oled.print(msg);
}


/*
    Shows the currend frequency
*/

void showStatus() {
  // Clear just the frequency information space.
  // oled.setCursor(0, 0);
  currentFrequency = radio.getFrequency();

  if (currentBand == FM ) {
    oled.print("FM ");
    oled.setCursor(38, 0);
    oled.print("      ");
    oled.setCursor(38, 0);
    oled.print(currentFrequency);
    oled.setCursor(95, 0);
    oled.print("MHz");
  } else {
    oled.print("AM ");
    oled.setCursor(38, 0);
    oled.setCursor(38, 0);
    oled.print("      ");
    oled.setCursor(38, 0);
    oled.print(currentFrequency);
    oled.setCursor(95, 0);
    oled.print("KHz");
  }
}


/*
   Show the current Signal Quality
*/



void loop()
{

  if (digitalRead(BAND_SWITC) == LOW ) {
    currentBand = (currentBand == FM) ? AM : FM;
    radio.setBand(currentBand);
  }

  if (radio.hasStatusChanged())
    showStatus();

  delay(50);
}
