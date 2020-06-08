/**
 * @brief SI4844 ARDUINO LIBRARY  
 * 
 * @details This is an Arduino library for the SI4844, BROADCAST AM/FM/SW RADIO RECEIVER IC family from Silicon Labs. 
 * @details This library is intended to provide an easier interface for controlling the SI47XX by using Arduino platform. 
 * @details The communication used by this library is I2C.
 * @details This file contains: const (#define), Defined Data type and Methods declarations
 * @details You can see a complete documentation on <https://github.com/pu2clr/SI4844>
 *   
 * @see https://pu2clr.github.io/SI4844/
 *  
 * @author PU2CLR - Ricardo Lima Caratti 
 * @date  2019-2020
 */

#include <Arduino.h>
#include <Wire.h>

#define SI4844_ADDRESS 0x11

// English...: Si4844 Commands
// Portuguese: Comando usado no Si4844
// Reference.: Si48XX ATDD PROGRAMMING GUIDE, page 12
#define ATDD_POWER_DOWN 0x11
#define ATDD_POWER_UP 0xE1
#define ATDD_AUDIO_MODE 0xE2
#define GET_REV 0x10
#define ATDD_XOSCEN 0x80
#define ATDD_XOWAIT 0x40
#define ATDD_GET_STATUS 0xE0

#define SET_PROPERTY 0x12
#define GET_PROPERTY 0x13
#define RX_VOLUME 0x4000
#define RX_HARD_MUTE 0x4001
#define RX_BASS_TREBLE 0x4002
#define RX_ACTUAL_VOLUME 0x4003

#define FM_SOFT_MUTE_MAX_ATTENUATION 0x1302
#define AM_SOFT_MUTE_MAX_ATTENUATION 0x3302
#define FM_DEEMPHASIS 0x1100

/** @defgroup GA1 Union and Structures 
 * @section GA1
 *  
 * @brief SI4844 data representation
 * 
 * @details The goal of this approach is separating data from code. 
 * The SI4844  works with many internal data that can be represented by data structure 
 * or defined data type in C/C++. 
 */

/** 
 * @ingroup GA1
 * @brief Status 
 * @details Represents searching for a valid frequency data type.
 */
typedef union {
  struct
  {
    uint8_t D0 : 1; 
    uint8_t D1 : 1; 
    uint8_t D2 : 1; 
    uint8_t D3 : 1; 
    uint8_t D4 : 1; 
    uint8_t ERR : 1;    //!< 1 = Error.
    uint8_t CTS : 1;    //!< 0 = Wait before sending next command; 1 = Clear to send next command.
  } refined;
  uint8_t raw;
} si4844_status;

/**
 * @ingroup GA1 
 * @brief Device Status 
 * @details The structure below represents the four bytes response got by command ATDD_GET_STATUS
 * @see PROGRAMMING GUIDE, pages 14 and 15
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

/**
 * @ingroup GA1
 * @brief Status response 
 * @see See Si48XX ATDD PROGRAMMING GUIDE, pages 14 and 15 
*/
typedef union {
  si4844_get_status refined;
  byte raw[4];
} si4844_status_response;

/**
 * @ingroup GA1
 * @brief Firmware Information 
 * @see Si48XX ATDD PROGRAMMING GUIDE, page 22.
 */
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

/**
 * @brief Firmware Response 
 * 
 */
typedef union {
  si4844_firmware_info refined;
  byte raw[9];
} si4844_firmware_response;

/**
 * @ingroup GA1
 * @brief Audio Mode
 */
typedef union {
  struct
  {
    byte AUDIOMODE : 2;
    byte FM_MONO : 1;
    byte ADJPT_ATTN : 1;
    byte ADJPT_STEO : 1;
    byte Reserved : 2;
    byte OPCODE : 1;
  } arg1;
  byte raw;
} si4844_audiomode;

/**
 * @ingroup GA1
 * @brief Audio Status Response
 */
typedef  union {
        struct {
          byte AUDIOMODE:2;
          byte FM_MONO:1;
          byte ADJPT_ATTN:1;
          byte ADJPT_STEO:1;
          byte Reserved:1;
          byte ERR:1;
          byte CTS:1;
        } status;
        byte raw;
} si4844_audiomode_status_response;

/**
 * @brief Data type to deal with SET_PROPERTY command
 * 
 * @details Property Data type (help to deal with SET_PROPERTY command on si473X)
 */
typedef union {
  struct
  {
    uint8_t byteLow;
    uint8_t byteHigh;
  } raw;
  uint16_t value;
} si4844_property;

/**
 * @brief 
 * 
 * Handling interruptions.
 * Whenever the status of the ATDD changes, a hardware interrupt is triggered. For example, when you move the tuner
 * potenciometer, the ATDD sends a signal to the Arduino pin (INTERRUPT_PIN). The same is true when the ATDD is capturing 
 * mono FM signal and has switched to stereo. The variable below indicates a change of the ATDD status. It will need to  
 * process some action (for example show on LCD this changes).   
 */
volatile static bool data_from_si4844;

static void interrupt_hundler()
{
 
  data_from_si4844 = true;
}


/**
 * @brief SI4844 Class 
 * 
 * @details This class implements all functions to help you to control the Si4844 devices. 
 */

class SI4844
{

private:

  si4844_status_response status_response; 
  si4844_firmware_response firmware_response;
  unsigned int resetPin;
  unsigned int interruptPin;
  byte currentBand; 

  inline void setClockLow(void) { Wire.setClock(10000); };
  inline void setClockHigh(void) { Wire.setClock(50000); };
  inline void waitInterrupr(void);
  inline bool isClearToSend(void);
  inline void waitToSend(void);

  // SI4844 band description (FM = 0; AM = 1; SW = 2)
  const char *bandmode_table[3] = {"FM", "AM", "SW"};
  const char *stereo_indicator_table[2] = {"Off","On "};

  byte volume = 48;
  byte bassTreble = 4;   

public :
  void setProperty(uint16_t propertyNumber, uint16_t parameter);
  uint16_t getProperty(uint16_t propertyNumber);
  void sendCommand(uint8_t cmd, int parameter_size, const uint8_t *parameter);
  void getCommandResponse(int response_size, uint8_t *response);

  void setup(unsigned int, unsigned int, byte);
  void reset(void );
  void setBand(byte);
  
  void changeVolume(char);  // deprecated
  void volumeUp(void);
  void volumeDown(void);
  void setVolume(byte);
  void setBassTreble(byte bass_treble);
  void bassTrebleUp();
  void bassTrebleDown();
  void audioMute(byte value);
  void setAudioMute(bool on);

  void setAmSoftMuteMaxAttenuation(uint8_t value);
  void setFmSoftMuteMaxAttenuation(uint8_t value);
  void setFmDeemphasis(uint8_t value);


  si4844_audiomode_status_response
  setAudioMode(byte audiomode, byte fm_mono, byte adjpt_attn, byte adjpt_steo, byte opcode);

  si4844_status_response *getStatus(void);
  si4844_firmware_response *getFirmware(void);
  // customize the frequency range of a band
  void setCustomBand(byte, unsigned, unsigned, byte);
 
  void powerDown(void);
  void powerUp(void);

  float getFrequency(void);
  bool hasStatusChanged(void);
  void resetStatus(void);


  /**
   * @brief Gets the current audio volume level
   * 
   * @return byte 
   */
  inline byte getVolume() {return volume; };
  byte getVolumeProperty();

  // return 0 = "FM mode"; 1 = "AM mode"; 2 = "SW mode".
  inline String getBandMode(){ return bandmode_table[status_response.refined.BANDMODE]; };
  // return char * "Off" or stereo "On"
  inline String getStereoIndicator(){ return stereo_indicator_table[status_response.refined.STATION]; };
  
 
  inline unsigned getStatusBCFG0() { return status_response.refined.BCFG0; };
  inline unsigned getStatusBCFG1() { return status_response.refined.BCFG1; };
  inline unsigned getStatusStereo() { return status_response.refined.STEREO; };
  inline unsigned getStatusStationIndicator() { return status_response.refined.STATION; };
  inline unsigned getStatusInformationReady() { return status_response.refined.INFORDY; };
  inline unsigned getStatusHostPowerUp() { return status_response.refined.HOSTPWRUP; };
  inline unsigned getStatusHostReset() { return status_response.refined.HOSTRST; };
  inline unsigned getStatusBandMode() { return status_response.refined.BANDMODE; };
  inline unsigned getStatusBandIndex() { return status_response.refined.BANDIDX; };
  inline unsigned getStatusCTS() { return status_response.refined.CTS; };


  inline unsigned getFirmwareReserved() { return firmware_response.refined.RESERVED; };
  inline unsigned getFirmwareErr() { return firmware_response.refined.ERR; };
  inline unsigned getFirmwareCTS() { return firmware_response.refined.CTS; };
  inline unsigned getFirmwarePartNumber() { return firmware_response.refined.PN; };
  inline unsigned getFirmwareMajorRevision() { return firmware_response.refined.FWMAJOR; };
  inline unsigned getFirmwareMinorRevision() { return firmware_response.refined.FWMINOR; };
  inline unsigned getFirmwareComponentMajorRevision() { return firmware_response.refined.CMPMAJOR; };
  inline unsigned getFirmwareComponentMinorRevision() { return firmware_response.refined.CMPMINOR; };
  inline unsigned getFirmwareChipRevision() { return firmware_response.refined.CHIPREV; };
};



