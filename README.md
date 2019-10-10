# Si4844 Library for Arduino

This is a library for the SI4844, BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER,  ICs from Silicon Labs for the Arduino development environment.  This library is intended to provide an easier interface for controlling the SI4844.

By Ricardo Lima Caratti, Oct, 2019. 

__Attention: Under construction.__


## Summary
1. [Your support is important](https://github.com/pu2clr/SI4844#your-support-is-important)
2. [About the SI4844 Architecture](https://github.com/pu2clr/SI4844#about-the-si4844-architecture)
3. [Library Installation](https://github.com/pu2clr/SI4844#library-installation)
4. [Hardware Requirements and Setup](https://github.com/pu2clr/SI4844#hardware-requirements-and-setup)
   1. [Schematic](https://github.com/pu2clr/SI4844#schematic)
   2. [Component Parts](https://github.com/pu2clr/SI4844#parts)
   3. [Photos](https://github.com/pu2clr/SI4844#photos)
5. [API Documentation]()
   1. [Overview]()
   2. [Defined Data Types and Structures]()
   3. [Public Methods]()
6. [References](https://github.com/pu2clr/SI4844#references)
7. [Videos](https://github.com/pu2clr/SI4844#videos) 


## Your support is important.

If you would like to support this library development, consider joining this project via Github. Thank you!

## About the SI4844 Architecture 

The Si4844 is an analog-tuned digital-display AM/FM/SW radio receiver. It has an analog-tune while frequency, band, and setero/mono information can be displayed on LCD. It works with a I2C protocol that allows a microcontroller send command and receive data. 

See more about SI4844 on [BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER](https://www.silabs.com/documents/public/data-sheets/Si4840-44-A10.pdf) 


## Library Installation

You can install the library is via the Arduino Library Manager. Go to the menu __Sketch__ > __Include Library__ > __Manage Libraries...__, and in the search box,  type __"PU2CLR SI4844"__.

The images below show that actions

![IDE 01](images/ide_01.png)

![IDE 01](images/ide_02.png)

![IDE 01](images/ide_03.png)


## Hardware Requirements and Setup

This library has been written for the Arduino platform and has been successfully tested on Pro Mini. I beleave it will wotk on any other Arduino with I2C support.

The SI4844 is a +3.3 V only part. If you are not using a +3.3 V version of Arduino, you have to use a converter.

To use this labrary you need to build a radio based on SI4844 coneccted to Arduino. The schematic and photos below show the hardware and setup requirments for start using this library.

### Schematic

The signal amplifier was not required fo test. 

![schematic](./images/SI4844_POC_01.png)
__Original Source:__ [Raymond Genovese, May 26, 2016 - How to Build an Arduino-Controlled AM/FM/SW Radio](https://www.allaboutcircuits.com/projects/build-an-arduino-controlled-am-fm-sw-radio/)
 
The document [BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER](https://www.silabs.com/documents/public/data-sheets/Si4840-44-A10.pdf), chapter two, page 11,  has a typical application schematic. 

The figure below shows that schematic

![schematic](./images/SI4844_SILICON_LABS1.png)
__Source: Silicon Labs (Si4840/44-A10)__

### Parts

Parts list  used by the first schematic

The table below is based on [Raymond Genovese, May 26, 2016 - How to Build an Arduino-Controlled AM/FM/SW Radio](https://www.allaboutcircuits.com/projects/build-an-arduino-controlled-am-fm-sw-radio/) .  

|Part	| Description |
|-------| ------------ |
|(*1) B1 	| ferrite bead 2.5 kOhm (100 mHz) B1, ferrite bead 2.5 kOhm (100 mHz), 81-BLM18BD252SZ1D |
|C1,C2,C5 |	4.7 uf non polarized capacitor |
|C3,C4 |	22 pf non polarized capacitor |
|C6,C7,C9 |	.1 uf non polarized capacitor |
|(*2) C8	| __4.7 uf non polarized capacitor__ (the orinal value is 47uF. I used 4.7uF) |
|C10, (*1) C11 |	.47 uf non polarized capacitor |
|(*1) C12, (*1) C14 |	33 nf non polarized capacitor |
|C13	| 33 pf non polarized capacitor |
|(*1) C15	| 10 pf non polarized capacitor |
|IC1	| Si4844-A10 radio receiver |
|(*1) Q1	| SS9018 NPN transistor |
|R1, R2	| 2.2K |
|(*1) R3	| 1K |
|R4, (*1) R7	| 100K |
|(*1) R5	| 10 Ohms |
|(*1) R6	| 120K |
|R8	| 100 Ohms |
|L1	| 270 nH Inductor (0,270 uH) |
|VR1 |	100K linear potentiometer |
|Y1	| 32.768 kHz crystal |
|ANT1 |	ferrite antenna |
|ANT2 | telescopic/whip antenna |

1. (*1) - not used for this project.
2. (*2) - the value used is 4.7uF and not 47uF as suggested by the original circuit.



### Photos 

### SI4844 soldered on adapter


It was a bit hard to solder the Si4844 on adapter. However, by using a electronic magnifier it was possible.

![SI4844 soldered on adapter 01](./images/si4844_board_01.png)

![SI4844 soldered on adapter 02](./images/si4844_board_02.png)

![SI4844 soldered on adapter 03](./images/si4844_board_03.png)


#### Protoboard

![SI4844 and Arduino on protoboard 01](./images/protoboard_01.png)

![SI4844 and Arduino on protoboard 02](./images/protoboard_02.png)

![SI4844 and Arduino on protoboard 03](./images/protoboard_04.png)



## API Documentation


### Overview


### Defined Data Types and Structures


```cpp
// English...: Four bytes response structure for command ATDD_GET_STATUS
// Reference.: Si48XX ATDD PROGRAMMING GUIDE, pages 14 and 15
typedef struct
{
  byte BCFG0 : 1;     // Bit 0
  byte BCFG1 : 1;     // bit 1
  byte STEREO : 1;    // bit 2
  byte STATION : 1;   // bit 3
  byte INFORDY : 1;   // bit 4
  byte HOSTPWRUP : 1; // bit 5
  byte HOSTRST : 1;   // bit 6
  byte CTS : 1;       // bit 7
  byte BANDIDX : 6;   // Form bit 0 to 5
  byte BANDMODE : 2;  // From bit 6 to 7
  byte d2 : 4;        // Frequency digit 2
  byte d1 : 4;        // Frequency digit 1
  byte d4 : 4;        // Frequency digit 4
  byte d3 : 4;        // frequency digit 3
} si4844_get_status;
```


```cpp
// English:
// Uses a C language feature to represent the 4 response bytes (status) sent by the ATDD_GET_STATUS.
// It is needed to undertand the C language union concept
// Portuguese:
// Usa um recurso da linguagem C para representar os 4 bytes de resposta (status) enviados pelo ATDD (SI4844).
// É preciso entender o conceito de UNION da linguagem C.
typedef union {
  si4844_get_status refined;
  byte raw[4];
} si4844_status_response;
```


```cpp
// English:
// GET_REV structure. The structure below represents 9 bytes response for GET_REV command.
// STATUS and RESP1 to RESP8.  See Si48XX ATDD PROGRAMMING GUIDE; AN610, page 22.
// Portuguese:
// A estrutura de dados a seguir representa 9 bytes de resposta para o comando GET_REV.
// Veja a página 22 do guia de programação (Si48XX ATDD PROGRAMMING GUIDE; AN610)
typedef struct
{
  byte RESERVED : 6; // Bit 0 to 5
  byte ERR : 1;      // bit 6
  byte CTS : 1;      // bit 2
  byte PN;           // Final 2 digits of Part Number (HEX).
  byte FWMAJOR;      // Firmware Major Revision (ASCII).
  byte FWMINOR;      // Firmware Minor Revision (ASCII).
  byte CMPMAJOR;     // Component Major Revision (ASCII).
  byte CMPMINOR;     // Component Minor Revision (ASCII).
  byte CHIPREV;      // Chip Revision (ASCII).
} si4844_firmware_info;
```


```cpp
typedef union {
  si4844_firmware_info refined;
  byte raw[9];
} si4844_firmware_response;
```



### Public Methods


void setup(unsigned int, unsigned int, byte)



void reset(void )


void setBand(byte);


void changeVolume(char);


void setVolume(byte);


si4844_status_response *getStatus(void);


si4844_firmware_response *getFirmware(void);


float getFrequency(void);


bool hasStatusChanged(void);


void resetStatus(void);


inline String getBandMode()



inline String getStereoIndicator()


inline unsigned getStatusBCFG0() 



inline unsigned getStatusBCFG1() 



inline unsigned getStatusStereo() 



inline unsigned getStatusStationIndicator() 



inline unsigned getStatusInformationReady() 



inline unsigned getStatusHostPowerUp() 



inline unsigned getStatusHostReset() 



inline unsigned getStatusBandMode() 



inline unsigned getStatusBandIndex() 



inline unsigned getStatusCTS() 


inline unsigned getFirmwareReserved() 


inline unsigned getFirmwareErr()


inline unsigned getFirmwareCTS()


inline unsigned getFirmwarePartNumber() 


inline unsigned getFirmwareMajorRevision() 


inline unsigned getFirmwareMinorRevision() 


inline unsigned getFirmwareComponentMajorRevision() 


inline unsigned getFirmwareComponentMinorRevision() 


inline unsigned getFirmwareChipRevision() 





## References

1. [Si48XX ATDD PROGRAMMING GUIDE](https://www.silabs.com/documents/public/application-notes/AN610.pdf)
2. [BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER](https://www.silabs.com/documents/public/data-sheets/Si4840-44-A10.pdf)
3. [Si4822/26/27/40/44 ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES](https://www.silabs.com/documents/public/application-notes/AN602.pdf)
4. [How to Build an Arduino-Controlled AM/FM/SW Radio](https://www.allaboutcircuits.com/projects/build-an-arduino-controlled-am-fm-sw-radio/)

   
## Videos

1. [Prova de Conceito com SI4844 e Arduino (vídeo sobre este projeto)](https://youtu.be/DAQbXZZR7VQ)

