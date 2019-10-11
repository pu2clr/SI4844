/*
 * This sketch implements a radio based on Silicon Labs SI4844 and SI4844 Arduino Library. 
 * The radio has the follow devices:
 * 
 * Display OLED  - Show Current Frequency, Stereo Status, Band (FM/MW/SW) and volume level.
 * Encoder with push button - Switch Band and Volume up and down, 
 * 
 * Author: Ricardo Lima Caratti (PU2CLR)
 * September, 2019
 */

#include <SI4844.h>
#include <Wire.h>

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12

#define DEFAULT_BAND 4

SI4844 si4844;

void setup()
{
    // display setup


    // Si4844 setup
    si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
    si4844.setVolume(55);


    // encoder setup

}

/*
 * Handle radio display
 */

void display_radio() {

}

/*
 * Handle encoder
 */




void loop () {



    if (si4844.hasStatusChanged())
    {
        display_radio();
    }
}