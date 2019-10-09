
#include <Arduino.h>
#include <Wire.h>

#define SI4844_ADDRESS 0x11

// English...: Si4844 Commands
// Portuguese: Comando usado no Si4844
// Reference.: Si48XX ATDD PROGRAMMING GUIDE, page 12
#define ATDD_POWER_DOWN 0x11
#define ATDD_POWER_UP 0xE1
#define GET_REV 0x10
#define ATDD_XOSCEN 0x80
#define ATDD_XOWAIT 0x40
#define ATDD_GET_STATUS 0xE0

#define SET_PROPERTY 0x12
#define RX_VOLUME 0x40

// Arduino Pin (tested on pro mini)
#define INTERRUPT_PIN 2
#define RESET_PIN 12

// English...: Four bytes response structure for command ATDD_GET_STATUS
// Portuguese: Estrutura de dados correspondente a uma resposta do comando ATDD_GET_STATUS
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

typedef union {
  si4844_firmware_info refined;
  byte raw[9];
} si4844_firmware_response;




/* 
 * English:
 * Handling interruptions.
 * Whenever the status of the ATDD changes, a hardware interrupt is triggered. For example, when you move the tuner
 * potenciometer, the ATDD sends a signal to the Arduino pin (INTERRUPT_PIN). The same is true when the ATDD is capturing 
 * mono FM signal and has switched to stereo. The variable below indicates a change of the ATDD status. It will need to  
 * process some action (for example show on LCD this changes).   
 * 
 * Portuguese:
 * Manipulando Interrupções
 * Sempre que o estado do ATDD é modificado, uma interrupção de hardware é disparada. Esta interrupção é manipulada por
 * uma função implementada neste sketch, que nada mais faz que dizer que isso ocorreu para que algumas ações seja executadas adiante. 
 * Por exemplo: Qundo potenciômetro do sintonizador for girado, o ATDD envia um sinal para o pino do Arduino, no nosso caso, o
 * pino 2 do mini pro (INTERRUPT_PIN). Da mesma forma, quando um sinal de FM está sendo capturado no modo MONO e o sinal, por 
 * alguma razão, melhora e passa para FM, esta interrupção ocorre indicando que devemos apresentar FM Estéreo. 
 * 
 */

volatile static bool data_from_si4844;

static void interrupt_hundler()
{
 
  data_from_si4844 = true;
}


class SI4844
{

private:

  si4844_status_response status_response; 
  si4844_firmware_response firmware_response;

  inline void setClockLow(void) { Wire.setClock(100000); };
  inline void setClockHigh(void) { Wire.setClock(500000); };
  inline void waitInterrupr(void);
  inline bool isClearToSend(void);
  inline void waitToSend(void);

  // SI4844 band description (FM = 0; AM = 1; SW = 2)
  const char *bandmode_table[3] = {"FM", "AM", "SW"};
  byte volume = 44;  

public : 
  void setup(void);
  void reset(void );
  void setBand(byte);
  void setVolume(char );
  si4844_status_response *getStatus(void);
  si4844_firmware_response *getFirmware(void);

  String getFrequency(void);
  bool hasStatusChanged(void);
  void resetStatus(void); 
  
};
