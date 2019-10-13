# Biblioteca Arduino para o Si4844 

Esta é uma bibnlioteca para o SI4844, BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER, uma Circuito Integrado fabricado pela Silicon Labs. É especialmente destinada aos interessados em controlar este dispositivo via o ambiente de desenvolvimento Arduino. 

Por Ricardo Lima Caratti / PU2CLR, Outunro de 2019. 

## Summary
1. [Seu suporte é importante](https://github.com/pu2clr/SI4844/blob/master/README-PT-BR.md#seu-suporte-%C3%A9-importante)
2. [Sobre a Arquitetura do SI4844](https://github.com/pu2clr/SI4844/blob/master/README-PT-BR.md#sobre-a-arquitetura-do-si4844)
3. [Terminologia](https://github.com/pu2clr/SI4844/blob/master/README-PT-BR.md#terminologia)
4. [Instalação da Biblioteca](https://github.com/pu2clr/SI4844/blob/master/README-PT-BR.md#instala%C3%A7%C3%A3o-da-biblioteca)
5. [Requisitos de Hardware e Configuração](https://github.com/pu2clr/SI4844/blob/master/README-PT-BR.md#requisitos-de-hardware-e-configura%C3%A7%C3%A3o)
   1. [Esquema](https://github.com/pu2clr/SI4844/blob/master/README-PT-BR.md#esquema)
   2. [Componentes](https://github.com/pu2clr/SI4844/blob/master/README-PT-BR.md#componentes)
   3. [Fotos](https://github.com/pu2clr/SI4844/blob/master/README-PT-BR.md#fotos)
6. [Documentação da API](https://github.com/pu2clr/SI4844/blob/master/README-PT-BR.md#documenta%C3%A7%C3%A3o-da-api)
   1. [Estruturas e Tipos Definidos de Dados]()
   2. [Métodos Públicos]()
      *  [setup](https://github.com/pu2clr/SI4844#setup)
      * [reset](https://github.com/pu2clr/SI4844#reset)
      * [setBand](https://github.com/pu2clr/SI4844#setband)
      * [changeVolume](https://github.com/pu2clr/SI4844#changevolume)
      * [setVolume](https://github.com/pu2clr/SI4844#setvolume)
      * [getStatus](https://github.com/pu2clr/SI4844#getstatus)
      * [getFirmware](https://github.com/pu2clr/SI4844#getfirmware)
      * [getFrequency](https://github.com/pu2clr/SI4844#getfrequency)
      * [hasStatusChanged](https://github.com/pu2clr/SI4844#hasstatuschanged)  
      * [resetStatus](https://github.com/pu2clr/SI4844#resetstatus)
      * [getBandMode](https://github.com/pu2clr/SI4844#getbandmode)
      * [getStereoIndicator](https://github.com/pu2clr/SI4844#getstereoindicator)
      * [getStatusBCFG0](https://github.com/pu2clr/SI4844#getstatusbcfg0)
      * [getStatusBCFG1](https://github.com/pu2clr/SI4844#getstatusbcfg1)
      * [getStatusStereo](https://github.com/pu2clr/SI4844#getstatusstereo)
      * [getStatusStationIndicator](https://github.com/pu2clr/SI4844#getstatusstationindicator)
      * [getStatusInformationReady](https://github.com/pu2clr/SI4844#getstatusinformationready)
      * [getStatusHostPowerUp](https://github.com/pu2clr/SI4844#getstatushostpowerup)
      * [getStatusHostReset](https://github.com/pu2clr/SI4844#getstatushostreset)
      * [getStatusBandMode](https://github.com/pu2clr/SI4844#getstatusbandmode)
      * [getStatusBandIndex](https://github.com/pu2clr/SI4844#getstatusbandindex)
      * [getStatusCTS](https://github.com/pu2clr/SI4844#getstatuscts)
      * [getFirmwareErr](https://github.com/pu2clr/SI4844#getfirmwareerr)
      * [getFirmwareCTS](https://github.com/pu2clr/SI4844#getfirmwarects)
      * [getFirmwarePartNumber](https://github.com/pu2clr/SI4844#getfirmwarepartnumber)
      * [getFirmwareMajorRevision](https://github.com/pu2clr/SI4844#getfirmwaremajorrevision)
      * [getFirmwareMinorRevision](https://github.com/pu2clr/SI4844#getfirmwareminorrevision)
      * [getFirmwareComponentMajorRevision](https://github.com/pu2clr/SI4844#getfirmwarecomponentmajorrevision)
      * [getFirmwareComponentMinorRevision](https://github.com/pu2clr/SI4844#getfirmwarecomponentminorrevision)
      * [getFirmwareChipRevision](https://github.com/pu2clr/SI4844#getfirmwarechiprevision) 
7. [Referências]()
8. [Vídeos]() 


## Seu suporte é importante

Se você tem interesse em fornecer suporte de desenvolvimento a esta biblioteca,junte-se a este projeto via Github.


## Sobre a Arquitetura do SI4844 

O SI4844 um rádio receptor AM/FM/OC. Ele possui um Sintonizador Analógico que permite a seleção de estações por meio de um potenciômetro (resistor variável). O SI4844 permite também a execução de comandos enviados via um microcontrolador para executar tarefas como: mudança de banda, volume, obter a frequência corrente para ser exibida em display entre outras. Há várias informações importante que podem ser de interesse para um projetista de rádio e que pode ser obtida por meio desta biblioteca para o Arduino. 
    

Conheça mais sobre a arquitetura do SI4844 na publicação da Silicon Labs [BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER](https://www.silabs.com/documents/public/data-sheets/Si4840-44-A10.pdf) 


## Terminologia

| Termo | Descrição  |
|------|--------|
| Bibliotecas Arduino | Se referem a arquivos escritos em C or C++ (.c, .cpp) capazes de fornecer funcionalidades extras aos seu Sketch Arduino. Neste contexto, a Biblioteca para o SI4844  fornece funcionalidades extras para controlar um rádio baseado no SI4844 | 
|IDE   |Ambiente integrado de Desenvolvimento (do Inglês: Integrated Development Environment)|      
|Sketch|Um nome utilizado pelo ambiente Arduino para um programa. Basicamente é o arquivo principal de um programa Arduino|
|ATDD  | Do Inglês, Analog Tune Digital Display. O termo ATDD é usado para se referir ao dispositivo (CI) SI4844 |
|interrupt (interrupção| No contexto deste documento, uma interrupção é um recurso do Arduino que permite a execução de tarefas importantes independentemente do fluxo de execução corrente do seu programa |
|C++| É uma linguagem de programação orientada a objetos. Pode ser entendida como uma extensão da linguagem C que permite o desenvolvimento de programas e bibliotecas usando a abordagem Orientada a Objetos.  |
|Guia de Programação| Se refere ao manual da Silicon Labs [Si48XX ATDD PROGRAMMING GUIDE](https://www.silabs.com/documents/public/application-notes/AN610.pdf)|
|POC| Do Inglês, Proof of Concept ou em Português, Prova de Conceito. Trata-se de uma abordagem para avaliar um dado produto, modelo ou ideia |


## Instalação da Biblioteca

É possível Instalar esta biblioteca em seu Ambienbte usando o proprio Gerenciador de Bibliotecas do Arduino. Para tanto, selecione a opção de menu __Sketch__ -> __Incluir Biblioteca__ -> Gerenciar Biblioteca. Por fim tecle na caixa de busca o texto PU2CLR Si4844.

The images below show that actions

![IDE 01](extras/images/idebr_01.png)

![IDE 01](extras/images/idebr_03.png)


## Requisitos de Hardware e Configuração

Conforme dito anteriormente, esta biblioteca foi escrita para a Plataforma Arduino e foi testada com sucesso no Arduino Pro Mini. Acredito que ela também funcionará em outras placas Arduino diferentes do Pro Mini. Contudo, é importante ressaltar que o dispositivo SI4844 opera com +3.3V. Se você não estiver usando uma versão Arduino de 3.3 V, será necessário o uso de um conversor de 5V para 3.3V.

Por fim, para usar esta biblioteca, é necessário construir um rádio baseado no SI4844. 
Os esquemas e fotos a seguir mostram como construir um rádio simples baseado no SI4844. 

### Esquema

Note no esquema proposta a seguir, que o Amplificador de sinal não é necessário para elaboração de um teste. Esta parte do circúito está destacada em vermelho. 

![schematic](./extras/images/SI4844_POC_01.png)
__Fonte:__ [Raymond Genovese, May 26, 2016 - How to Build an Arduino-Controlled AM/FM/SW Radio](https://www.allaboutcircuits.com/projects/build-an-arduino-controlled-am-fm-sw-radio/)
 
O documento [BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER](https://www.silabs.com/documents/public/data-sheets/Si4840-44-A10.pdf), capítulo 2, página 11,  tem um esquema mais simplificado ainda deste rádio. 

A figura a segui apresenta este esquema

![schematic](./extras/images/SI4844_SILICON_LABS1.png)
__Fonte: Silicon Labs (Si4840/44-A10)__

### Componentes

Listas de componentes utilizados 

A tabela a seguir é baseada na publicação de [Raymond Genovese, May 26, 2016 - How to Build an Arduino-Controlled AM/FM/SW Radio](https://www.allaboutcircuits.com/projects/build-an-arduino-controlled-am-fm-sw-radio/) .  

|Componente	| Descrição |
|-------| ------------ |
|(*1) B1 	| ferrite bead 2.5 kOhm (100 mHz) B1, ferrite bead 2.5 kOhm (100 mHz), 81-BLM18BD252SZ1D |
|C1,C2,C5 |	4.7uF capacitor não  polarizado |
|C3,C4 |	22pf capacitor não  polarizado |
|C6,C7,C9 |	.1uF capacitor não  polarizado |
|(*2) C8	| __4.7uf capacitor não  polarizado__ (atenção: no esquema original, este valor é 47uF) |
|C10, (*1) C11 |	.47uF capacitor não  polarizado |
|(*1) C12, (*1) C14 |	33nF capacitor não  polarizado |
|C13	| 33pF capacitor não  polarizado |
|(*1) C15	| 10pF capacitor não  polarizado |
|IC1	| Si4844-A10 radio receiver |
|(*1) Q1	| SS9018 NPN transistor |
|R1, R2	| 2.2K |
|(*1) R3	| 1K |
|R4, (*1) R7	| 100K |
|(*1) R5	| 10 Ohms |
|(*1) R6	| 120K |
|R8	| 100 Ohms |
|L1	| 270 nH Indutor (0,270 uH) |
|VR1 |	100K Poteciômetro Linear |
|Y1	| 32.768 kHz crital |
|ANT1 |	Antena de ferrite |
|ANT2 | Antena telescópica |

1. (*1) - Não utilizado neste projeto.
2. (*2) - O valor que utilizei foi 4.7uF e não o valor sugerido no esquema original (47uF).


### Fotos 

### Soldagem do SI4844 no adaptador

Foi um pouco difícil fazer a soldagem de um componente tão pequeno no adaptador. Contudo, com o uso de uma lente de aumento eletrônica e uma caneta (ferro de solda) foi possível fazer o trabalho. 


![SI4844 soldered on adapter 01](./extras/images/si4844_board_01.png)

![SI4844 soldered on adapter 02](./extras/images/si4844_board_02.png)

![SI4844 soldered on adapter 03](./extras/images/si4844_board_03.png)


#### Protoboard

![SI4844 and Arduino on protoboard 01](./extras/images/protoboard_01.png)

![SI4844 and Arduino on protoboard 03](./extras/images/protoboard_04.png)



## Documentação da API


A Biblioteca SI4844 foi desenvolvida em C++.  Para utilizá-la, inclua o arquivo SI4844.h e declare em seu Sketch uma variável da classe SI4844. Após instalar a Biblioteca no Arduino, você deve incluí-la em seu código conforme apresentado no código a seguir.


```cpp
// 
#include <SI4844.h>   
#include <Wire.h>

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12


#define DEFAULT_BAND 4

SI4844 si4844; 

void setup() {

    // Initiate and connect the device ATDD (SI4844) to Arduino
    si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);

}

void loop() {


  // if something changed on ATDD (SI4844), do something  
  if (si4844.hasStatusChanged())
  {
    Serial.print("Band Index: ");
    Serial.print(si4844.getStatusBandIndex());
    Serial.print(" - ");
    Serial.print(si4844.getBandMode());
    Serial.print(" - Frequency: ");    
    Serial.print(si4844.getFrequency(),0);
    Serial.print(" KHz");
    Serial.print(" - Stereo ");
    Serial.println(si4844.getStereoIndicator());
  }

}
```


### Defined Data Types and Structures

To make the SI4844 device easier to deal, some defined data types were built to handle byte and bits responses.

```cpp
/* 
 * The structure below represents the four bytes response got by command ATDD_GET_STATUS
 * See Si48XX ATDD PROGRAMMING GUIDE, pages 14 and 15
 */
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
/*
 * Uses a C language feature to represent two way for the 4 response bytes (status) sent by the ATDD_GET_STATUS.
 * It is needed to undertand the C language union concept.
 * See Si48XX ATDD PROGRAMMING GUIDE, pages 14 and 15 
*/
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


#### setup

```cpp
/*
* Initiate the SI4844 instance and connect the device (SI4844) to Arduino. 
* Calling this library should be the first thing to do to control the SI4844.
*
* @param resetPin  arduino pin used to reset the device
* @param interruprPin arduino pin used to handle interrupr      
* @param defaultBand band that the radio should start
*/
void setup(unsigned int, unsigned int, byte)
```
Example:
```cpp
  si4844.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);
```

#### reset

```cpp
/*
 * reset
 * English
 * See pages 7, 8, 9 and 10 of the programming guide.
 */
void reset(void )
```
Example:
```cpp
  si4844.reset();
```


#### setBand

```cpp
/*
 * Set the radio to a new band. 
 * See Table 8. Pre-defined Band Table in Si48XX ATDD PROGRAMMING GUIDE; AN610; pages 17 and 18  
 */
void setBand(byte);
```

Example:
```cpp
  si4844.setBand(4); // FM
```


#### changeVolume

```cpp
/*
 *  Up or down the sound volume level/  
 *  @param char '+' up and '-' down 
 */
void changeVolume(char);
```

Example:
```cpp
  si4844.changeVolume('+'); 
```


#### setVolume

```cpp
/*
 * Set the sound volume level. 
 * @param byte volumeLevel (domain: 0 to 63) 
 */
setVolume(byte level)
```

Exemple:
```cpp 
  si4844.setVolume(55);
```


#### getStatus

```cpp 
/*
 * Get tune freq, band, and others information, status of the device.
 * Use this method only if you want to deal with that information by yourself. 
 * This library has other methods to get that information easier. 
 * 
 * @return a pointer to a structure type si4844_status_response
 */
si4844_status_response *getStatus(void);
```
Exemple:
```cpp 
  si4844.getStatus();
```

#### getFirmware

```cpp
/*
 * Get part number, chip revision, firmware, patch, and component revision numbers.
 * You do not need to call this method. It is executed just once at setup methos. 
 * There are other methods that give you that information.   
 * See page 22 of programming guide.
 * 
 * @return a pointer to a structure type  with the part number, chip revision, 
 *         firmware revision, patch revision, and component revision numbers.
 */
si4844_firmware_response *getFirmware(void);
```
Exemple:
```cpp 
  si4844.getFirmware();
```

#### getFrequency

```cpp
/*
 * Get the current frequency of the radio in KHz. 
 * For example: FM, 103900 KHz (103.9 MHz);
 *              SW, 7335 KHz (7.34 MHz, 41m)   
 * 
 * @return float current frequency in KHz.  
 */
float getFrequency(void);
```

Exemple:
```cpp 
    Serial.print("Frequency: ");    
    Serial.print(si4844.getFrequency(),0);
```

#### hasStatusChanged

```cpp
/*
*  Check if the SI4844 has its status changed. If you move the tuner, for example,
*  the status of the device is changed. 
*
*  return true or false  
*/
bool hasStatusChanged(void);
```

Exemple:
```cpp 
  if (si4844.hasStatusChanged())
  {
    Serial.print(" - Frequency: ");    
    Serial.print(si4844.getFrequency(),0);
    Serial.print(" KHz");
    Serial.print(" - Stereo ");
    Serial.println(si4844.getStereoIndicator());
  }
```

#### resetStatus

```cpp
void resetStatus(void);
```


#### getBandMode

```cpp
/*
 * Get the band mode (FM/AM/SW)
 * @return 0 = "FM mode"; 1 = "AM mode"; 2 = "SW mode".
 */ 
inline String getBandMode()
```

#### getStereoIndicator

```cpp
/*
 * return 0 = stereo off; 1 stereo on
 */
inline String getStereoIndicator()
```

#### getStatusBCFG0

```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusBCFG0() 
```

#### getStatusBCFG1

```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusBCFG1() 
```

#### getStatusStereo

```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusStereo() 
```


#### getStatusStationIndicator

```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusStationIndicator() 
```


#### getStatusInformationReady

```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusInformationReady() 
```


#### getStatusHostPowerUp
```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusHostPowerUp() 
```

#### getStatusHostReset

```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusHostReset() 
```


#### getStatusBandMode

```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusBandMode() 
```

#### getStatusBandIndex

```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusBandIndex() 
```

#### getStatusCTS

```cpp
/*
 * See Si48XX ATDD PROGRAMMING GUID, AN610, pages 15 and 16
 */
inline unsigned getStatusCTS() 
```


#### getFirmwareErr

```cpp 
inline unsigned getFirmwareErr()
```

#### getFirmwareCTS
```cpp
inline unsigned getFirmwareCTS()
```

#### getFirmwarePartNumber

```cpp
/*
 * Get Firmware Final 2 digits of Part Number (HEX).
 */ 
inline unsigned getFirmwarePartNumber() 
```
__See example below__


#### getFirmwareMajorRevision

```cpp
/*
 * Get Firmware Major Revision (ASCII).
 */ 
inline unsigned getFirmwareMajorRevision() 
```
__See example below__


#### getFirmwareMinorRevision

```cpp
/*
 * Get Firmware Minor Revision (ASCII).
 */
inline unsigned getFirmwareMinorRevision() 
```
__See example below__


#### getFirmwareComponentMajorRevision

```cpp
/*
 * Get Firmware Component Major Revision (ASCII).
 */ 
inline unsigned getFirmwareComponentMajorRevision() 
```
__See example below__


#### getFirmwareComponentMinorRevision

```cpp
/* 
 * Get Firmware Component Minor Revision (ASCII).
 */
inline unsigned getFirmwareComponentMinorRevision() 
```
__See example below__


#### getFirmwareChipRevision

```cpp
/*
 * Chip Revision (ASCII).
 */
inline unsigned getFirmwareChipRevision() 
```

Exmple:

```cpp

  Serial.println("\nSI4844 -  Firmware information\n");

  si4844.getFirmware();
  Serial.print("Final 2 digits of Part Number..: ");
  Serial.println(si4844.getFirmwarePartNumber());
  Serial.print("Firmware Major Revision........: ");
  Serial.println(si4844.getFirmwareMajorRevision());
  Serial.print("Firmware Minor Revision........: ");
  Serial.println(si4844.getFirmwareMinorRevision());
  Serial.print("Component Major Revision.......: ");
  Serial.println(si4844.getFirmwareComponentMajorRevision());
  Serial.print("Component Minor Revision.......: "); 
  Serial.println(si4844.getFirmwareComponentMinorRevision());
  Serial.print("Chip Revision..................: ");
  Serial.println(si4844.getFirmwareChipRevision());

  Serial.println("*****************************");
  ```



## References

1. [Si48XX ATDD PROGRAMMING GUIDE](https://www.silabs.com/documents/public/application-notes/AN610.pdf)
2. [BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER](https://www.silabs.com/documents/public/data-sheets/Si4840-44-A10.pdf)
3. [Si4822/26/27/40/44 ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES](https://www.silabs.com/documents/public/application-notes/AN602.pdf)
4. [How to Build an Arduino-Controlled AM/FM/SW Radio](https://www.allaboutcircuits.com/projects/build-an-arduino-controlled-am-fm-sw-radio/)

   
## Videos

1. [Prova de Conceito com SI4844 e Arduino (vídeo sobre este projeto)](https://youtu.be/DAQbXZZR7VQ)

