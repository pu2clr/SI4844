# Examples of using the SI4844 Library for Arduino

This folder has a set of examples that might help you in your project with Si4844 and Arduino.


__This project is about a library to control the SI4844 device and the focus of this project is the library and its functionalities. Please, don't ask me to assist you in your: displays (LCD, OLED, TFT etc), encoders, buttons or something else out the library scope. Thanks.__


## Minimal Radio with SI4844 (SI4844_MINIMAL)

The [SI4844_MINIMAL.ino](https://github.com/pu2clr/SI4844/blob/master/examples/SI4844_MINIMAL/SI4844_MINIMAL.ino) is a sketch with just 35 lines. It is enough to make a simple radio based on SI4844.  


## Proof of Concept

The [SI4844_POC.ino](https://github.com/pu2clr/SI4844/blob/master/examples/SI4844_POC/SI4844_POC.ino) is a proof of concept for SI4844 controlled by Arduino and the SI4844 Library. This Arduino Sketch only works on your IDE (Arduino IDE). However, you can replace the Serial Monitor functions that deal the SI4844 and arduino with functions that will manipulate the LCD, encoder and push buttons appropriated for your project.  

## Extended SW band frequency ranges from 2.3–5.6 MHz and 22–28.5 MHz 

The sketch [SI4844_CUSTOM_BAND.ino](https://github.com/pu2clr/SI4844/blob/master/examples/SI4844_POC/SI4844_CUSTOM_BAND.ino) shows how to extend a SW band frequency ranges. You can define band from from 2.3–5.6 MHz and 22–28.5 MHz. 



## SI4844 with OLED

The sketch [SI4844_OLED.ino](https://github.com/pu2clr/SI4844/tree/master/examples/SI4844_OLED) shows an example of using an I2C OLED display. __This sketch uses the SSD1306Ascii Arduino Library by Bill Greiman__. You must install SSD1306Ascii via Manage Libraries on your Arduino IDE.

The schematics below show how to add the OLED and buttons. 


#### Arduino and OLED circuit

![schematic Arduino and OLED](https://github.com/pu2clr/SI4844/blob/master/extras/images/schematic_Arduino_circuit.png)

#### SI4844 minimal circuit

![schematic SI4844 minimal circuit](https://github.com/pu2clr/SI4844/blob/master/extras/images/schematic_SI4844_minimal_circuit.png)



## Sound Control

The sketch [SI4844_BASS_TREBLE.ino](https://github.com/pu2clr/SI4844/tree/master/examples/SI4844_BASS_TREBLE) show how to use sound control (treble, bass, mute etc).
