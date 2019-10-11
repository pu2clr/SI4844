/*
 * This sketch implements a radio based on Silicon Labs SI4844 and SI4844 Arduino Library. 
 * The radio has the follow devices:
 * 
 * Display OLED  - Show Current Frequency, Stereo Status, Band (FM/MW/SW) and volume level.
 * Encoder with push button - Switch Band and Volume up and down, 
 * 
 * Author: Ricardo Lima Caratti (PU2CLR)
 * September, 2019
 * 
 * *****>>> Atention: Under construction
 * 
 */

#include <SI4844.h>
#include <Wire.h>

// OLED include files
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"


// SI4844 device defined constants 
#define INTERRUPT_PIN 2
#define RESET_PIN 12
#define DEFAULT_BAND 4

// OLED defined constants 
#define I2C_OLED_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

// Enconder defined constant PINs
#define ENCODER_PIN_A  8 // Arduino  D08
#define ENCODER_PIN_B  9 // Arduino  D09

#define BUTTON_BAND 7    // Switch band

#define MIN_ELAPSED_TIME 300

// Global variables

// Band plan table: See pages 17 and 18 of Si48XX ATDD PROGRAMMING GUIDE for other options
const byte band_index_table[] = {  4,  // FM
                                  23,  // MW (AM)
                                  27,  // SW1 
                                  29,  // SW2
                                  31,  // SW3
                                  33,  // SW4
                                  35,  // SW5
                                  37,  // SW6 
                                  39}; // SW7

const int max_band_index = (sizeof band_index_table / sizeof(byte)) - 1;
volatile int currentBand = 0; // Default band will be 4 (position 0 in table above)
long volatile elapsedTimeInterrupt = millis(); // will control the minimum time to process an interrupt action

volatile bool isBandChanged = false;

// Create an instance of SI4844 class
SI4844 si4844;

// OLED - Declaration for a SSD1306 display connected to I2C (SDA, SCL pins)
SSD1306AsciiAvrI2c oled;

void setup()
{
    // display setup
    // Initiating the OLED Display
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    oled.setFont(Adafruit5x7);
    oled.set2X();
    oled.clear();
    display.print("\n PU2CLR");
    delay(3000);
    display.clear();

    // Si4844 setup
    si4844.setup(RESET_PIN, INTERRUPT_PIN, band_index_table[currentBand]);
    si4844.setVolume(55);


    // encoder setup
    attachInterrupt(digitalPinToInterrupt(BUTTON_BAND), switch_band, RISING); // whenever the BUTTON_VFO_BFO goes from LOW to HIGH
    
    delay(500);

}

/*
 * Handle radio display
 */

void display_dial() {

    String freq;
    String unitFreq;

    if (si4844.getBandMode() == 0 ) { 
        freq = String((si4844.getFrequency() / 1000.0),1);
        unitFreq = "MHz"; 
    } else {
        freq = String(si4844.getFrequency(), 0);
        unitFreq = "KHz";
    }

    oled.set2X();
    oled.setCursor(0, 0);
    oled.print(freq);
    oled.print("     ");

    oled.set1X();
    oled.print("\n\n\n");
    oled.print(unitFreq);

    // oled.print("\nBand: ");
    // oled.print();

}

/*
 * Handle encoder
 */


/*
 * Switch Band
 */
void switch_band()
{
    if ((millis() - elapsedTimeInterrupt) < MIN_ELAPSED_TIME)
        return; // nothing to do if the time less than MIN_ELAPSED_TIME

    noInterrupts();            //  disable global interrupts:

    currentBand = (currentBand < max_band_index)? (currentBand + 1) : 0;
    isBandChanged = true;

    elapsedTimeInterrupt = millis();

    interrupts(); // enable interrupts
}

void loop () {

    // Check if the bush button was pressed  
    if ( isBandChanged ) {
        si4844.setBand(band_index_table[currentBand]);
    }


    if (si4844.hasStatusChanged())
    {
        display_dial();
    }
}