/**
 * @brief SI4844 ARDUINO LIBRARY  
 * 
 * @details This is an Arduino library for the SI4822, SI4826, SI4827, SI4840, 4844A, and 4844B, BROADCAST AM/FM/SW RADIO RECEIVER IC family from Silicon Labs. 
 * @details When appropriate, this documentation will use the acronym SI48XX to refer to the following devices: SI4822, SI4826, SI4827, SI4840, SI4844A, and SI4844B.
 * @details In most cases, references to the SI4844 device also apply to the other aforementioned devices.
 * @details This library is intended to provide an easier interface for controlling the SI47XX by using Arduino platform. 
 * @details The communication used by this library is I2C.
 * @details This file contains: const (#define), Defined Data type and Methods declarations
 * @details You can see a complete documentation on <https://github.com/pu2clr/SI4844>
 *   
 * @see https://pu2clr.github.io/SI4844/
 * @see Si48XX ATDD PROGRAMMING GUIDE - AN610
 * @see BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER - Si4844-B20
 * @see Si4822/26/27/40/44 A NTENNA , SCHEMATIC , LAYOUT, AND DESIGN GUIDELINES - AN602
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
#define FM_DEEMPHASIS 0x1100                    // You can use setProperty(FM_DEEMPHASIS, value);
#define FM_SOFT_MUTE_RATE 0x1300                // You can use setProperty(FM_SOFT_MUTE_RATE, value);
#define FM_SOFT_MUTE_SLOPE 0x1301               // You can use setProperty(FM_SOFT_MUTE_SLOPE, value);
#define FM_STEREO_IND_BLEND_THRESHOLD 0x1207    // You can use setProperty(FM_STEREO_IND_BLEND_THRESHOLD, value);
#define FM_BLEND_RSSI_STEREO_THRESHOLD  0x1800  // You can use setProperty(FM_BLEND_RSSI_STEREO_THRESHOLD, value);
#define FM_BLEND_RSSI_MONO_THRESHOLD 0x1801     // You can use setProperty(FM_BLEND_RSSI_MONO_THRESHOLD, value); 

#define REFCLK_FREQ  0x0201         // You can use setProperty(REFCLK_FREQ, value);
#define REFCLK_PRESCALE 0x0202      // You can use setProperty(REFCLK_PRESCALE, value);

#define AM_SOFT_MUTE_RATE  0x3300
#define AM_SOFT_MUTE_SLOPE  0x3301
#define AM_SOFT_MUTE_SNR_THRESHOLD 0x3303


#define DEVICE_LAST_VALID_INDEX_BAND 40

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
 * @brief Configures band parameter 
 * @details Represents searching for a valid frequency data type.
 * @see  Page 18 of the Si48XX ATDD PROGRAMMING GUIDE - AN610
 */

typedef union {
  struct {
    uint8_t BANDIDX:6; // Band Index to Set. See Table 8. Pre-defined Band Table
    uint8_t XOWAIT: 1; // Crystal Oscillator Stabilization Wait Time After Reset. 0 = 600 ms; 1 = 900 ms
    uint8_t XOSCEN: 1; // Crystal Oscillator Enable. 0 = Use external RCLK; 1 = Use crystal oscillator (XTALI and XTALO with external 32.768 kHz crystal).
  } refined;
  uint8_t raw;
} si4844_arg_band_index;

/** 
 * @ingroup GA1
 * @brief Configures all SI48XX parameters 
 * @details Represents searching for a valid frequency data type.
 * @see  Page 18 of the Si48XX ATDD PROGRAMMING GUIDE - AN610
 */
typedef union {
    struct
    {
       uint8_t  BANDIDX :  6; // Band Index to Set. See Table 8. Pre-defined Band Table
       uint8_t  XOWAIT  :  1; // Crystal Oscillator Stabilization Wait Time After Reset. 0 = 600 ms; 1 = 900 ms
       uint8_t  XOSCEN  :  1; // Crystal Oscillator Enable. 0 = Use external RCLK; 1 = Use crystal oscillator (XTALI and XTALO with external 32.768 kHz crystal).
       uint8_t  BANDBOT_HIGH; // Band Bottom Frequency Limit; FM 6400..10900 (64.0–109.0 MHz); AM 510..1750 (510–1750 kHz); SW 2300..28500 (2.3–28.5 MHz)
       uint8_t  BANDBOT_LOW;  // Band Bottom Frequency Limit; FM 6400..10900 (64.0–109.0 MHz); AM 510..1750 (510–1750 kHz); SW 2300..28500 (2.3–28.5 MHz)
       uint8_t  BANDTOP_HIGH; // Band Top Frequency Limit; FM 6400..10900 (64.0–109.0 MHz); AM 510..1750 (510–1750 kHz); SW 2300..28500 (2.3–28.5 MHz)
       uint8_t  BANDTOP_LOW;  // Band Top Frequency Limit; FM 6400..10900 (64.0–109.0 MHz); AM 510..1750 (510–1750 kHz); SW 2300..28500 (2.3–28.5 MHz)
       uint8_t  CHSPC;       // Channel Spacing; FM 10 (e.g.,100 kHz); AM 9 or 10 (e.g., 9 kHz or 10 kHz); SW 5 (e.g., 5 kHz)
       uint8_t  DUMMY   :  5; // Set it to 00000
       uint8_t  DFBAND  :  1; // Default Band Settings; 0 = Allow host controller to override the band property settings; 1 = Force to use tuner default band property settings (Applicable to Si4827 part only)
       uint8_t  UNI_AM  :  1; // Universal AM Band. 0 = Disable universal AM band (default AFC range of 1.1 kHz); 1 = Enable universal AM band (wider AFC range in tuning); Applicable to Si4827 and Si4844B parts and AMRX mode only
       uint8_t  TVFREQ  :  1; // TV Audio Channel Frequency Display; 0 = Disable TV audio channel frequency display format; 1 = Enable TV audio channel frequency display format; Applicable to Si4827 and Si4844B parts and FMRX mode only
    } refined;
    uint8_t raw[7];
} SI4844_arg_band; 

/** 
 * @ingroup GA1
 * @brief Status 
 * @details Represents searching for a valid frequency data type.
 */
typedef union {
  struct
  {
    uint8_t D_0 : 1; 
    uint8_t D_1 : 1; 
    uint8_t D_2 : 1; 
    uint8_t D_3 : 1; 
    uint8_t D_4 : 1; 
    uint8_t ERR : 1;    //!< 1 = Error.
    uint8_t CTS : 1;    //!< 0 = Wait before sending next command; 1 = Clear to send next command.
  } refined;
  uint8_t raw;
} si4844_status;

/**
 * @ingroup GA1 
 * @brief Device Status (The first byte of the device ststus register) 
 * @details The structure below represents the first byte response got by command ATDD_GET_STATUS
 * @see PROGRAMMING GUIDE, pages 14 and 15
 */
typedef union { 
  struct  {
    uint8_t BCFG0 : 1;     // Bit 0 - 0 = ATDD device detects band; 1 = Host detects band
    uint8_t BCFG1 : 1;     // bit 1
    uint8_t STEREO : 1;    // bit 2
    uint8_t STATION : 1;   // bit 3
    uint8_t INFORDY : 1;   // bit 4
    uint8_t HOSTPWRUP : 1; // bit 5
    uint8_t HOSTRST : 1;   // bit 6
    uint8_t CTS : 1;       // bit 7
  } refined; 
  uint8_t raw;
} si4844_device_status;

/**
 * @ingroup GA1 
 * @brief All Device Information (All Status) 
 * @details The structure below represents the four bytes response got by command ATDD_GET_STATUS
 * @see PROGRAMMING GUIDE, pages 14 and 15
 */
typedef struct
{
  uint8_t BCFG0 : 1;     // Bit 0 - 0 = ATDD device detects band; 1 = Host detects band
  uint8_t BCFG1 : 1;     // bit 1
  uint8_t STEREO : 1;    // bit 2
  uint8_t STATION : 1;   // bit 3
  uint8_t INFORDY : 1;   // bit 4
  uint8_t HOSTPWRUP : 1; // bit 5
  uint8_t HOSTRST : 1;   // bit 6
  uint8_t CTS : 1;       // bit 7
  uint8_t BANDIDX : 6;   // Form bit 0 to 5
  uint8_t BANDMODE : 2;  // From bit 6 to 7
  uint8_t d2 : 4;        // Frequency digit 2
  uint8_t d1 : 4;        // Frequency digit 1
  uint8_t d4 : 4;        // Frequency digit 4
  uint8_t d3 : 4;        // frequency digit 3
} si4844_get_status;

typedef struct { 
  uint16_t dummy; 
  uint16_t CHFREQ;
} si4844_get_channel_frequency;

/**
 * @ingroup GA1
 * @brief Status response 
 * @see See Si48XX ATDD PROGRAMMING GUIDE, pages 14 and 15 
*/
typedef union {
  si4844_get_status refined;
  si4844_get_channel_frequency rawStatus;
  uint8_t raw[4];
} si4844_status_response;

/**
 * @ingroup GA1
 * @brief Firmware Information 
 * @see Si48XX ATDD PROGRAMMING GUIDE, page 22.
 */
typedef struct
{
  uint8_t RESERVED : 6; // Bit 0 to 5
  uint8_t ERR : 1;      // bit 6
  uint8_t CTS : 1;      // bit 2
  uint8_t PN;           // Final 2 digits of Part Number (HEX).
  uint8_t FWMAJOR;      // Firmware Major Revision (ASCII).
  uint8_t FWMINOR;      // Firmware Minor Revision (ASCII).
  uint8_t CMPMAJOR;     // Component Major Revision (ASCII).
  uint8_t CMPMINOR;     // Component Minor Revision (ASCII).
  uint8_t CHIPREV;      // Chip Revision (ASCII).
} si4844_firmware_info;

/**
 * @brief Firmware Response 
 * 
 */
typedef union {
  si4844_firmware_info refined;
  uint8_t raw[9];
} si4844_firmware_response;

/**
 * @ingroup GA1
 * @brief Audio Mode
 */
typedef union {
  struct
  {
    uint8_t AUDIOMODE : 2;
    uint8_t FM_MONO : 1;
    uint8_t ADJPT_ATTN : 1;
    uint8_t ADJPT_STEO : 1;
    uint8_t Reserved : 2;
    uint8_t OPCODE : 1;
  } arg1;
  uint8_t raw;
} si4844_audiomode;

/**
 * @ingroup GA1
 * @brief Audio Status Response
 */
typedef  union {
        struct {
          uint8_t AUDIOMODE:2;
          uint8_t FM_MONO:1;
          uint8_t ADJPT_ATTN:1;
          uint8_t ADJPT_STEO:1;
          uint8_t Reserved:1;
          uint8_t ERR:1;
          uint8_t CTS:1;
        } status;
        uint8_t raw;
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
 * @brief Interrupt status
 * @details This variable must be true every time an interruption occurs and then must return to the false every time an action resulting from the interruption is performed.
 * @details The variable below indicates a change of the ATDD status. When it occurs, it means the system needs to process some action (for example show on LCD this change).  
 * @see interrupt_hundler() function in SI44.cpp. 
 */
volatile static bool data_from_device;


/**
 * @brief SI4844 Class 
 * 
 * @details This class implements all functions to help you to control the Si4844 devices. 
 */

class SI4844
{

private:

  si4844_device_status device_status;               // Stores the first currente status of the device         
  si4844_status_response all_receiver_status;       // Stores all current receiver information 
  si4844_firmware_response firmware_response;
  uint16_t resetPin;
  uint16_t interruptPin;
  uint8_t   currentBand = 0; 

  uint8_t  xoscen = 1;
  uint8_t  xowait = 0;
  uint32_t clockSpeed = 50000;


  uint8_t  system_error = 0; 


  /**
   * @ingroup BF
   * @brief Sets hight clock speed to be used on I2C bus
   * @param clock (10000 = 10kHz; 50000 = 50kHz; 100000 = 100kHz ... )
   */
  inline void setClockSpeed(uint16_t clock) { this->clockSpeed = clock; }; 

  /**
   * @ingroup BF
   * @brief Sets I2C bus speed to 10kHz
   */
  inline void setClockLow(void) { Wire.setClock(10000); };

  /**
   * @ingroup BF
   * @brief Sets I2C bus speed to hight previous defined speed
   * @see setClockSpeed 
   */
  inline void setClockHigh() { Wire.setClock(this->clockSpeed); };


  inline void waitInterrupt(void);
  inline bool isClearToSend(void);
  inline void waitToSend(void);


  // SI4844 band description (FM = 0; AM = 1; SW = 2)
  const char *bandmode_table[3] = {"FM", "AM", "SW"};
  const char *stereo_indicator_table[2] = {"ST","MO"};
  char strFormattedCurrentFrequenct[8];

  uint8_t volume = 30;
  uint8_t bassTreble = 4;   

public :
  /**
   * @ingroup BF
   * @brief Set the Data Status From Device 
   * @details It is a flag that means the device triggered an interrupt.
   * @details You can use this function to back the flag status to false. This way you can check when the device triggers the next interrupt.
   * @details It is very useful when the user wants to control the interrupt instead of give this control to the library. 
   * @param value true or false
   */
  inline void setStatusInterruptFromDevice( bool value ) { data_from_device = value; };

  /**
   * @ingroup BF 
   * @brief Get the Data Status From Device 
   * @details It returns true when the device has triggered an interrupt. 
   * @return true or false
   */
  inline bool getDataStatusInterruptFromDevice() { return data_from_device; };


  void setProperty(uint16_t propertyNumber, uint16_t parameter);
  uint16_t getProperty(uint16_t propertyNumber);
  void sendCommand(uint8_t cmd, int parameter_size, const uint8_t *parameter);
  void getCommandResponse(int response_size, uint8_t *response);

  void setupSlideSwitch(uint16_t resetPin, int interruptPin, uint32_t hightClockSpeed = 50000);
  void setup(uint16_t resetPin, int interruptPin, int8_t defaultBand = 0, uint32_t hightClockSpeed = 50000);
  void debugDevice(uint16_t resetPin, uint16_t interruptPin, uint8_t defaultBand, void (*showFunc)(char *msg));
  void reset(void );
  void setBand(uint8_t newBand = 0);
  void setBandSlideSwitch();
  
  void changeVolume(char);  // deprecated
  void volumeUp(void);
  void volumeDown(void);
  void setVolume(byte);
  void setBassTreble(uint8_t bass_treble);
  void bassTrebleUp();
  void bassTrebleDown();
  uint8_t inline getBassTreble() {return this->bassTreble;};
  void audioMute(uint8_t value);
  void setAudioMute(bool on);

  void setAmSoftMuteMaxAttenuation(uint8_t value);
  void setFmSoftMuteMaxAttenuation(uint8_t value);
  void setFmDeemphasis(uint8_t value);
  void setFmSoftMuteRate(uint8_t value);
  void setFmSoftMuteSlope(uint8_t value);

  void setAmSoftMuteRate(uint8_t value);
  void setAmSoftMuteSlope(uint8_t value);
  void setAmSoftMuteSnrThreshold(uint8_t value);


  void setBlendThresholdStereoIndicator(uint16_t value);

  void setCrystalOscillatorEnable(uint8_t XOSCEN );
  void setCrystalOscillatorStabilizationWaitTime(uint8_t XOWAIT);
  void setReferenceClockFrequency(uint16_t value);
  void setReferenceClockPrescaler(uint16_t value);



  si4844_audiomode_status_response
  setAudioMode(uint8_t audiomode, uint8_t fm_mono, uint8_t adjpt_attn, uint8_t adjpt_steo, uint8_t opcode);

  si4844_device_status *getStatus();
  si4844_status_response *getAllReceiverInfo(void);
  si4844_firmware_response *getFirmware(void);

  bool isHostDetectionBandConfig();
  int8_t getValidBandIndex();
 

  // customize the frequency range of a band
  void setCustomBand(uint8_t bandIndex, uint16_t  botton, uint16_t  top, uint8_t bandSpace, uint8_t dfband = 0, uint8_t uni_am = 0, uint8_t tvreq = 0 );
 
  void setDefaultBandIndx( uint8_t bandidx);

  void powerDown(void);
  void powerUp(void);

  float getFrequency(void);
  uint32_t getFrequencyInteger(void);
  bool hasStatusChanged(void);
  bool hasBandChanged(void);
  void resetStatus(void);

  /**
   * @ingroup GB1 
   * @brief Gets the latest band set by the microcontroller.
   * @details It can be used as a feature to detect if the current band has been changed via Slide
   * @return latest band
   */
  uint8_t inline getCurrentBand() {return this->currentBand; };

  /**
   * @ingroup GB1 
   * @brief Gets the current audio volume level
   * 
   * @return Volume level 
   */
  inline uint8_t getVolume() {return volume; };
  uint8_t getVolumeProperty();

  /**
   * @ingroup GB1 
   * @brief Get the Band Mode 
   * @return char*   "FM", "AM" or "SW"
   * @see getStatusBandMode()
   */
  inline char * getBandMode(){ return (char *) bandmode_table[all_receiver_status.refined.BANDMODE]; };

  /**
   * @ingroup GB1 
   * @brief Get the Stereo Indicator 
   * @details **Call getStatus or getAllReceiverInfo before calling this function**    
   * @return char* "ON" or "OFF" 
   * @see getStatusStereo()
   */
  inline char * getStereoIndicator(){ return (char *) stereo_indicator_table[device_status.refined.STEREO]; };
  
  /** 
   * @ingroup GB1 
   * @brief Gets Band CFG0 (Band Detection Configuration).
   * @details 0 = ATDD device detects band; 1 = Host detects band
   * @details **Call getStatus or getAllReceiverInfo before calling this function** 
   * @return 0 = ATDD device detects band; 1 = Host detects band
   */
  inline uint16_t  getStatusBCFG0() { return device_status.refined.BCFG0; };

  /** 
   * @ingroup GB1 
   * @brief Gets Band CFG1 (Band Properties Priority)
   * @details **Call getStatus or getAllReceiverInfo before calling this function** 
   * @return 0 = ATDD device accepts host customized band properties; 1 = ATDD device ignores host customized band properties
   */
  inline uint16_t  getStatusBCFG1() { return device_status.refined.BCFG1; };

  /** 
   * @ingroup GB1 
   * @brief Gets Stereo indicator.
   * @details Applicable to Si4840/44 parts FM function only (id does not make sense for Si4827).
   * @details **Call getStatus or getAllReceiverInfo before calling this function**  
   * @return 0 = Stereo off; 1 = Stereo on
   * @see getStereoIndicator()
   */
  inline uint16_t  getStatusStereo() { return device_status.refined.STEREO; };


  /** 
   * @ingroup GB1 
   * @brief Gets Station Indicator.
   * @details **Call getStatus or getAllReceiverInfo before calling this function** 
   * @return 0 = Invalid Station; 1 = Valid Station
   */
  inline uint16_t  getStatusStationIndicator() { return device_status.refined.STATION; };
  
  /** 
   * @ingroup GB1 
   * @brief Gets Information Ready
   * @details During power up case, the system controller should not display any of the channel frequency, band mode, band index,
   * @details station, or stereo statuses until the INFORDY bit is set.
   * @details The host controller should not display the channel frequency when CHFREQ remains zero even when INFORDY=1.
   * @details 0 = Tune info not ready yet; 1 = Tune info ready (i.e., Band mode, band index, channel frequency, sta-tion, and stereo indicators)
   * @details **Call getStatus or getAllReceiverInfo before calling this function** 
   * @return 0 = Tune info not ready yet; 1 = Tune info ready 
   */
  inline uint16_t  getStatusInformationReady() { return device_status.refined.INFORDY; };

  /** 
   * @ingroup GB1 
   * @brief Checks Host Power Up Status
   * @details if True, the system needs to Power Up the device 
   * @details **Call getStatus or getAllReceiverInfo before calling this function**  
   * @return True: issue the ATDD_POWER_UP command with the valid band index detected. 
   */
  inline bool  needHostPowerUp() { return device_status.refined.HOSTPWRUP; };

  /**
    * @ingroup GB1
    * @brief   Checks HOST Reset Status  
    * @details Check if the host (microcontroler) needs to reset the device.
    * @details **Call getStatus or getAllReceiverInfo before calling this function**      
    * @return  True or False.
  */
  inline bool  needHostReset() { return device_status.refined.HOSTRST; };


  /** 
   * @ingroup GB1 
   * @brief Gets the Channel Frequency.
   * @details The channel frequency is a 16-bit word of 4 digits in BCD format:
   * @details FM 0640..1090 (64.0–109.0 MHz)
   * @details AM 0504..1750 (504–1750 kHz)
   * @details SW4 *0230..2850 (2.3– 28.5 MHz)
   * @details For FM band, if the China TV channel audio sub-carrier display feature is enabled, the CHFREQ bit[15] MSB = 1 means the host controller needs to add an additional 50 kHz for the channel frequency. 
   * @details For SW band, the CHFREQ bit[15] MSB = 1 means the host controller needs to add an additional 5 kHz for the channel frequency.
   * @return 0 = Tune info not ready yet; 1 = Tune info ready 
   */
  inline uint16_t  getRawChannelFrequency() { return all_receiver_status.rawStatus.CHFREQ; };


  /** 
   * @ingroup GB1 
   * @brief Gets the current Band Mode
   * @return 0 = FM mode; 1 = AM mode; 2 = SW mode
   * @see getBandMode()
   */
  inline uint16_t  getStatusBandMode() { return all_receiver_status.refined.BANDMODE; };
  
  /** 
   * @ingroup GB1 
   * @brief Gets the current Band Index Detected.
   * @return 0~19: FM band; 20~24: AM band; 25~40: SW band
   */
  inline uint16_t  getStatusBandIndex() { return all_receiver_status.refined.BANDIDX; };



  inline uint16_t  getStatusCTS() { return all_receiver_status.refined.CTS; };


  inline uint16_t  getFirmwareReserved() { return firmware_response.refined.RESERVED; };
  inline uint16_t  getFirmwareErr() { return firmware_response.refined.ERR; };
  inline uint16_t  getFirmwareCTS() { return firmware_response.refined.CTS; };
  inline uint16_t  getFirmwarePartNumber() { return firmware_response.refined.PN; };
  inline uint16_t  getFirmwareMajorRevision() { return firmware_response.refined.FWMAJOR; };
  inline uint16_t  getFirmwareMinorRevision() { return firmware_response.refined.FWMINOR; };
  inline uint16_t  getFirmwareComponentMajorRevision() { return firmware_response.refined.CMPMAJOR; };
  inline uint16_t  getFirmwareComponentMinorRevision() { return firmware_response.refined.CMPMINOR; };
  inline uint16_t  getFirmwareChipRevision() { return firmware_response.refined.CHIPREV; };

  void setResetPin(uint16_t resetPin);
  void setInterruptPin(int interruptPin);

  uint8_t scanI2CBus(uint8_t *device, uint8_t limit);
  bool detectDevice();
  void convertToChar(uint32_t value, char *strValue, uint8_t len, uint8_t dot, uint8_t separator, bool remove_leading_zeros = true);
  char* getFormattedFrequency(uint8_t removeRightDigit = 0, uint8_t thousandsSeparator = ',');

};



