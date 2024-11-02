/**
 * @mainpage SI48XX Arduino Library implementation
 * This is a library for the SI4844, BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER,
 * ICs from Silicon Labs.  
 * This library is intended to provide an easier interface to control the SI4844.
 * 
 * This library can be freely distributed using the MIT Free Software model.
 * 
 * Copyright (c) 2019 Ricardo Lima Caratti
 */

#include "SI4844.h"

/** @defgroup GB  Member Functions */

/**
 * @ingroup GB
 * @brief Sends (sets) property to the SI48XX
 * 
 * @details This method is used for others to send generic properties and params to SI4844
 * 
 * @param propertyNumber property number (example: RX_VOLUME)
 * @param parameter   property value that will be seted
 */
void SI4844::setProperty(uint16_t propertyNumber, uint16_t parameter)
{
    si4844_property property;
    si4844_property param;

    property.value = propertyNumber;
    param.value = parameter;
    waitToSend();
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);
    Wire.write(property.raw.byteHigh); // Send property - High byte - most significant first
    Wire.write(property.raw.byteLow);  // Send property - Low byte - less significant after
    Wire.write(param.raw.byteHigh);    // Send the argments. High Byte - Most significant first
    Wire.write(param.raw.byteLow);     // Send the argments. Low Byte - Less significant after
    Wire.endTransmission();
    delayMicroseconds(550);
}

/**
 * @ingroup GB 
 * @brief Gets a given property from the SI4844
 * 
 * @details This method is used to get a given property from SI47XX
 * @details You might need to extract set of bits information from the returned value to know the real value
 * 
 * @param propertyNumber property number (example: RX_VOLUME)
 * @return property value  (the content of the property)
 */
uint16_t SI4844::getProperty(uint16_t propertyNumber)
{
    si4844_property property;
    si4844_status status;

    property.value = propertyNumber;
    waitToSend();
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(GET_PROPERTY);
    Wire.write(0x00);
    Wire.write(property.raw.byteHigh); // Send property - High byte - most significant first
    Wire.write(property.raw.byteLow);  // Send property - Low byte - less significant after
    Wire.endTransmission();

    delayMicroseconds(550);

    Wire.requestFrom(SI4844_ADDRESS, 4);
    status.raw = Wire.read();

    if (status.refined.ERR == 1)
        return 0;

    Wire.read(); // resp1 is always 0

    property.raw.byteHigh = Wire.read();
    property.raw.byteLow = Wire.read();

    return property.value;
}

/**
 * @ingroup GB
 * @brief Sends a given command to the SI4844 device. 
 * @details This function can be useful when you want to execute a SI4844 device command and it was not implemented by this library.
 * @details Also, you need to work with bit operators to compose the parameters of the command [ &(and), ˆ(xor), |(or) etc ].   
 *
 * @see getCommandResponse, setProperty, getProperty
 *  
 * @param cmd command number (see PROGRAMMING GUIDE)
 * @param parameter_size Parameter size in bytes. Tell the number of argument used by the command.
 * @param parameter uint16_t  byte array with the arguments of the command  
 */
void SI4844::sendCommand(uint8_t cmd, int parameter_size, const uint8_t *parameter)
{
    waitToSend();
    Wire.beginTransmission(SI4844_ADDRESS);
    // Sends the command to the device
    Wire.write(cmd);
    // Sends the argments (parameters) of the command
    for (byte i = 0; i < parameter_size; i++)
        Wire.write(parameter[i]);
    Wire.endTransmission();
}

/**
 * @ingroup GB
 * @brief   Returns with the command response.  
 * @details After a command is executed by the device, you can get the result (response) of the command by calling this method.
 * 
 * @see sendCommand, setProperty
 * 
 * @param response_size  num of bytes returned by the command.
 * @param response  byte array where the response will be stored.     
 */
void SI4844::getCommandResponse(int response_size, uint8_t *response)
{
    // Asks the device to return a given number o bytes response
    Wire.requestFrom(SI4844_ADDRESS, response_size);
    // Gets response information
    for (byte i = 0; i < response_size; i++)
        response[i] = Wire.read();
}

/**
 * @ingroup GB
 * @brief   Waiting for an external interrupt
 * @details This function is called whenever the status of ATDD (SI4844) changes. 
 * @details It can occur, for example, when you use the analog tuner.  
 */
void SI4844::waitInterrupt(void)
{
    
    while (!data_from_device)
        ;
}

/**
 * @ingroup GB
 * @brief Initiates the SI4844 instance and connect the device (SI4844) to Arduino. 
 * @details Calling this library should be the first thing to do to control the SI4844.
 * @details If interruptPin is -1, it means you will control interrupt in your sketch. 
 * @details In this case, you have to call interrupt_hundler() (see SI4844.h)   
 * @param resetPin      arduino pin used to reset the device
 * @param interruptPin  interruprPin arduino pin used to handle interrupt 
 * @param defaultBand   band that the radio should start 
 */
void SI4844::setup(uint16_t resetPin, int interruptPin, byte defaultBand)
{

    this->resetPin = resetPin;
    this->interruptPin = interruptPin;


    // Arduino interrupt setup.
    // if interruptPin parameter is < 0, it means the interrupt is being controlled by the user of this library
    if (interruptPin != -1 ) {
        pinMode(interruptPin, INPUT);
        attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_hundler, RISING);
    }

    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);

    data_from_device = false;

    reset();

    // FM is the default BAND
    // See pages 17 and 18 (Table 8. Pre-defined Band Table) for more details
    setBand(defaultBand);

    // You need call it just once.
    getFirmware();
}

/**
 * @ingroup GB
 * @brief Used to debug  
 * @details use this function instead setup to check your implementation
 *  
 * @param resetPin          same call setup;
 * @param interruptPin      same call setup    
 * @param defaultBand       same call setup  
 * @param showFunc          function of your sketch that will be called to show something
 */
void SI4844::debugDevice(uint16_t resetPin, uint16_t interruptPin, byte defaultBand, void (*showFunc)(char *msg))
{

    if ( showFunc == NULL ) return;

    this->resetPin = resetPin;
    this->interruptPin = interruptPin;

    // Arduino interrupt setup.
    pinMode(interruptPin, INPUT);

    attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_hundler, RISING);
    // attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_hundler, RISING);
    // attachInterrupt(0, interrupt_hundler, CHANGE);
    
    // showFunc("So far so good 1");
    // delay(1000);

    data_from_device = false;
    reset();

    // showFunc("So far so good 2");
    // delay(1000);

    // FM is the default BAND
    // See pages 17 and 18 (Table 8. Pre-defined Band Table) for more details
    setBand(defaultBand);

    // showFunc("So far so good 3");
    // delay(1000);

    // You need call it just once.
    getFirmware();

    // showFunc("So far so good 3");
}

/**
 * @ingroup GB
 * @brief Resets the SI4844 device
 * @details Prepares the system  to be started.
 * @see pages 7, 8, 9 and 10 of the programming guide.
 */
void SI4844::reset()
{
    // waitToSend();

    setClockLow(); // See *Note on page 5
    data_from_device = false;
    digitalWrite(resetPin, LOW);
    delayMicroseconds(500);
    digitalWrite(resetPin, HIGH);
    delayMicroseconds(500);
    waitInterrupt();
    delayMicroseconds(5500);
}


/**
 * @ingroup GB
 * @brief  Power the device down
 * @details Moves the SI4844 device from power up to power down mode. 
 * @see Si48XX ATDD PROGRAMMING GUIDE; AN610; page 45
 */
void SI4844::powerDown(void)
{
    data_from_device = false;
    // Wait until rady to send a command
    waitToSend();
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_POWER_DOWN);
    Wire.endTransmission();
    delayMicroseconds(2500);
}

/**
 * @ingroup GB
 * @brief Power the device up
 * @details Moves the SI4844 device from power down to power up 
 * @see Si48XX ATDD PROGRAMMING GUIDE; AN610; page 45
 */
void SI4844::powerUp(void)
{

    setBand(currentBand);
}

/**
 * @ingroup GB
 * @brief Sets a new band to the device 
 * @details This method is used to select a band 
 * 
 * @see See Table 8. Pre-defined Band Table in Si48XX ATDD PROGRAMMING GUIDE; AN610; pages 17 and 18  
 * 
 * @param new_band  band number. 
 */
void SI4844::setBand(byte new_band)
{
    reset();

    currentBand = new_band;

    // Assigning 1 to bit 7. It means we are using external crystal
    // Silicon Labs; Si48XX ATDD PROGRAMMING GUIDE; AN610; page 7
    // Just another way to deal with bytes and bits using C/C++.
    new_band |= 0b10000000;
    new_band &= 0b10111111;

    data_from_device = false;

    // Wait until rady to send a command
    waitToSend();

    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_POWER_UP);
    Wire.write(new_band);
    Wire.endTransmission();
    delayMicroseconds(2500);
    waitInterrupt();

    delayMicroseconds(2500);
    getStatus();
    delayMicroseconds(2500);
}


/**
 * @ingroup GB 
 * @brief Checks the CTS status.
 * @details Checks whether the device is ready to receive a new command. 
 * @return true 
 * @return false 
 */
bool SI4844::isClearToSend(void)
{
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_GET_STATUS);
    Wire.endTransmission();
    delayMicroseconds(2000);
    Wire.requestFrom(SI4844_ADDRESS, 1);
    status_response.raw[0] = Wire.read();
    return status_response.refined.CTS; // return 0 (false) or 1 (true)
}

/**
 * @ingroup GB
 * @brief Wait for the ATDD become Clear to Send. 
 * @details Waits for CTS status
 */
inline void SI4844::waitToSend()
{

    while (!isClearToSend())
        ;
}

/**
 * @ingroup GB
 * @deprecated 
 * @brief Up or down the sound volume level
 * 
 * @param command '+' up and '-' down 
 */
void SI4844::changeVolume(char command)
{
    // See global variable volume;
    switch (command)
    {
    case '+':
        if (volume <= 58)
            volume += 4;
        break;
    case '-':
        if (volume >= 10)
            volume -= 4;
        break;
    default:
        volume = 44;
    }

    // See: Table 4. Using the SET_PROPERTY Command; page 11 for more details
    setVolume(volume);
}

/**
 * @ingroup GB
 * @brief Increases the volume level
 */
void SI4844::volumeUp()
{
    if (volume < 63)
        volume++;
    setVolume(volume);
}

/**
 * @ingroup GB
 * @brief Decreases the volume level
 */
void SI4844::volumeDown()
{
    if (volume >= 6)
        volume--;
    setVolume(volume);
}

/**
 * @ingroup GB
 * @brief Sets the volume level. 
 * @details Sets a value to the audio volume.
 * @see Table 4, Si48XX ATDD PROGRAMMING GUIDE; AN610; page 11
 * @param byte volumeLevel (domain: 0 to 63) 
 */
void SI4844::setVolume(byte volumeLavel)
{
    if (volumeLavel > 63)
        return;

    waitToSend();
    setProperty(RX_VOLUME, volumeLavel);

    this->volume = volumeLavel;
}

/**
 * @ingroup GB
 * @brief Gets the current volume value stored in SI4844 device. 
 * @details Use getVolume instead. 
 * @return byte 
 */
byte SI4844::getVolumeProperty() {
    uint16_t volumeProperty = getProperty(RX_ACTUAL_VOLUME);
    return (byte)volumeProperty;
}

/**
 * @ingroup GB
 * @brief Set the sound volume level, bass and treble.
 * 
 * | bass_treble | Description |
 * | ----------- | ----------- |
 * |   0         | Bass boost +4 (max) |
 * |   1         | Bass boost +3 |
 * |   2         | Bass boost +2 |
 * |   3         | Bass boost +1 (min) |
 * |   4         | Normal (No Bass/Treble effect) (Default) 5- Treble boost +1 (min) |
 * |   6         | Treble boost +2 |
 * |   7         | Treble boost +3 |
 * |   8         | Treble boost +4 (max) |
 * 
 * @see Si48XX ATDD PROGRAMMING GUIDE; AN610; pages 29, 35, 65 and 67
 * 
 * @param bass_treble bass and treble (domain: 0 to 8). See table above
 */
void SI4844::setBassTreble(byte bass_treble) {
        setProperty(RX_BASS_TREBLE, bass_treble);
}

/**
 * @ingroup GB
 * @brief Less treble, more bass.
 */
void SI4844::bassTrebleDown()
{
    if (bassTreble < 8)
        bassTreble++;
    setBassTreble(bassTreble);
}

/**
 * @ingroup GB
 * @brief More treble, less bass
 */
void SI4844::bassTrebleUp()
{
    if (bassTreble > 0)
        bassTreble--;
    setBassTreble(bassTreble);
}

/**
 * @ingroup GB
 * @brief Set audio mode 
 * 
 * @see Si48XX ATDD PROGRAMMING GUIDE; AN610; page 21
 * 
 * @param byte audio_mode 
 *             0 = Digital volume mode (no bass/treble effect, volume levels from 0 to 63) 
 *             1 = Bass/treble mode (no digital volume control, fixed volume level at 59)
 *             2 = Mixed mode 1 (bass/treble and digital volume coexist, max volume = 59) 
 *             3 = Mixed mode 2 (bass/treble and digital volume coexist, max volume = 63) 
 *             Default is 3 (Mixed mode 2).
 * @param byte fm_mono
 *             0 = Stereo audio output (default)
 *             1 = Mono audio output
 * @param byte adjpt_attn Audio attention of adjacent tune wheel positions of a station
 *             0 = {–2 dB, -0dB, –2 dB} i.e., adjacent points volume levels –2 dB (default) 
 *             1 = {–0 dB, -0dB, –0 dB} i.e., adjacent points same volume levels
 * @param byte adjpt_steo
 *             0 = Adjacent points allow stereo separation and stereo indicator on (default) 
 *             1 = Adjacent points disable stereo separation and stereo indicator are off
 * @param byte opcode 
 *             0 = Set audio mode and settings
 *             1 = Get current audio mode and settings without setting
 */
si4844_audiomode_status_response SI4844::setAudioMode(byte audiomode, byte fm_mono, byte adjpt_attn, byte adjpt_steo, byte opcode)
{

    si4844_audiomode am;
    si4844_audiomode_status_response resp;

    am.arg1.OPCODE = opcode;
    am.arg1.AUDIOMODE = audiomode;
    am.arg1.ADJPT_ATTN = adjpt_attn;
    am.arg1.ADJPT_STEO = adjpt_steo;
    am.arg1.FM_MONO = fm_mono;

    // Wait until rady to send a command
    waitToSend();

    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_AUDIO_MODE);
    Wire.write(0x00);
    Wire.write(am.raw);
    Wire.endTransmission();
    delayMicroseconds(2500);
    // Wait for CTS
    do
    {
        Wire.requestFrom(SI4844_ADDRESS, 0x01); // request 1 byte response
        resp.raw = Wire.read();
    } while (!resp.status.CTS);

    return resp;
}

/**
 * @ingroup GB
 * @brief  Mutes the audio output.
 * 
 * @param value value 0 = normal (no mute); 1 = Right side mute; 2; Left side mute; 3 = both side 
 */
void SI4844::audioMute(byte value)
{
    setProperty(RX_HARD_MUTE, value);
}

/**
 * @ingroup GB
 * @brief Mutes the audio output.
 * 
 * @param bool on false = normal (no mute); true = mute 
 */
void SI4844::setAudioMute(bool on)
{
    if (on)
        audioMute(3);
    else
        audioMute(0);
}

/**
 * @ingroup GB
 * @brief Get tune freq, band, and others information, status of the device.
 * @details Use this method only if you want to deal with that information by yourself. 
 * @details This library has other methods to get that information easier. 
 * 
 * @return a pointer to a structure type si4844_status_response
 */
si4844_status_response *SI4844::getStatus()
{
    waitToSend();
    setClockHigh();
    do
    {
        Wire.beginTransmission(SI4844_ADDRESS);
        Wire.write(ATDD_GET_STATUS);
        Wire.endTransmission();
        delayMicroseconds(2500);
        // request 4 bytes response from atdd (si4844)
        Wire.requestFrom(SI4844_ADDRESS, 0x04);
        for (int i = 0; i < 4; i++)
            status_response.raw[i] = Wire.read();
        // check response error. Exit when no error found. See page 7.
        // if INFORDY is 0 or CHFREQ is 0, not ready yet

    } while (status_response.refined.INFORDY == 0 || (status_response.raw[2] == 0 && status_response.raw[3] == 0));

    return &status_response;
}

/** 
 * @ingroup GB
 * @brief Get part number, chip revision, firmware, patch, and component revision numbers.
 * @details You do not need to call this method. It is executed just once at setup methos. 
 * @details There are other methods that give you that information.   
 * 
 * @see  Si48XX ATDD PROGRAMMING GUIDE; AN610;  page 22 
 * 
 * @return a pointer to a structure type  wirh the part number, chip revision, 
 *         firmware revision, patch revision, and component revision numbers.
 */
si4844_firmware_response *SI4844::getFirmware(void)
{

    // Check and wait until the ATDD is ready to receive command
    waitToSend();

    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(GET_REV);
    Wire.endTransmission();

    // Request for 9 bytes response
    Wire.requestFrom(SI4844_ADDRESS, 0x09);

    for (int i = 0; i < 9; i++)
        firmware_response.raw[i] = Wire.read();

    data_from_device = false;

    return &firmware_response;
}

/**
 * @ingroup GB
 * @brief Get the current frequency of the radio in KHz. 
 * @details For example: FM, 103900 KHz (103.9 MHz); SW, 7335 KHz (7.34 MHz, 41m)   
 * 
 * @return float current frequency in KHz.  
 */
float SI4844::getFrequency(void)
{
    getStatus();
    int addFactor = 0;
    int multFactor = 1;
    // Check CHFREQ bit[15] MSB = 1
    // See Page 15 of Si48XX ATDD PROGRAMMING GUIDE
    if (status_response.refined.BANDMODE == 0)
    {
        multFactor = 100;
        if (status_response.refined.d1 & 0b00001000)
        {
            status_response.refined.d1 &= 0b11110111;
            addFactor = 50;
        }
    }
    else if (status_response.refined.BANDMODE == 2)
    {
        multFactor = 10;
        if (status_response.refined.d1 & 0b00001000)
        {
            status_response.refined.d1 &= 0b11110111;
            addFactor = 5;
        }
    }

    float f;

    f = (status_response.refined.d4);
    f += (status_response.refined.d3) * 10;
    f += (status_response.refined.d2) * 100;
    f += (status_response.refined.d1) * 1000;

    data_from_device = false;

    return (f * multFactor + addFactor);
}

/** 
 * @ingroup GB 
 * @brief Checks whether the SI4844 has its status changed. 
 *  @details Actually it checks if the device triggered an external interruption due to some internal event. 
 *  @details For example: If you move the tuner, the status of the device is changed. 
 *
 *  @return true or false  
 */
bool SI4844::hasStatusChanged(void)
{
    return data_from_device;
}

/**
 * @ingroup GB
 * @brief set the interrupr status to false. It will turn true after next interrupr  
 */
void SI4844::resetStatus()
{
    data_from_device = false;
}

/** 
 * @ingroup GB
 * @brief This method allows you to customize the frequency range of a band.
 * @details The SI4844 can work from 2.3–28.5 MHz on SW, 64.0–109.0MHz on FM
 * @details You can configure the band index 40, for example, to work between 27 to 28 MHz.
 * 
 * @see Si48XX ATDD PROGRAMMING GUIDE, pages 17, 18, 19 and 20.
 * 
 * (top – button)/(bandSpace) must be betwenn 50 and 230
 * 
 * @param  bandIndes Predefined band index (valid values: betwenn 0 and 40)
 * @param  button Band Bottom Frequency Limit
 * @param  top Band Top Frequency Limit
 * @param  bandSpace Channel Spacing (use 5 or 10 - On FM 10 = 100KHz)
 */
void SI4844::setCustomBand(byte bandIndex, uint16_t  botton, uint16_t  top, byte bandSpace)
{

    union {
        struct
        {
            byte bandindex : 6;
            byte xowait : 1;
            byte xoscen : 1;
            uint16_t  botton;
            uint16_t  top;
            byte bandspace;
        } refined;
        byte raw[6];
    } customband;

    // The first thing that we have to do is switch to desired band
    setBand(bandIndex);

    // Now we can customize the band.
    data_from_device = false;
    customband.refined.bandindex = bandIndex;
    customband.refined.xowait = 0;
    customband.refined.xoscen = 1;
    customband.refined.botton = botton;
    customband.refined.top = top;
    customband.refined.bandspace = bandSpace;

    // Wait until rady to send a command
    waitToSend();

    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_POWER_UP);
    Wire.write(customband.raw[0]);
    Wire.write(customband.raw[2]);
    Wire.write(customband.raw[1]);
    Wire.write(customband.raw[4]);
    Wire.write(customband.raw[3]);
    Wire.write(customband.raw[5]);
    Wire.write(0x00);

    Wire.endTransmission();
    delayMicroseconds(2500);
    waitInterrupt();

    delayMicroseconds(2500);
    getStatus();
    delayMicroseconds(2500);
}

/**
 * @ingroup GB
 * @brief Sets AM Soft Mute Max Attenuation..  
 * @details Maximum attenuation to apply when in soft mute. Specified in units of dB. Default maximum attenuation is 16 dB.
 * @details Set to 0 to disable soft mute. Valid range: 0 - 63. 
 * @param value  number between 0 and 63
 */
void SI4844::setAmSoftMuteMaxAttenuation(uint8_t value) {
    setProperty(AM_SOFT_MUTE_MAX_ATTENUATION, value);
}

/**
 * @ingroup GB
 * @brief FM Soft Mute Maximum Attenuation. 
 * @details Maximum attenuation to apply when in soft mute. Specified in units of dB. Default maximum attenuation is 16 dB.
 * @details Set to 0 to disable soft mute. Valid range: 0 - 31. 
 * @param value  number between 0 and 31
 */
void SI4844::setFmSoftMuteMaxAttenuation(uint8_t value)
{
    setProperty(FM_SOFT_MUTE_MAX_ATTENUATION, value);
}

/**
 * @ingroup GB
 * @brief Sets de-emphasis time constant. 
 * @details Sets the FM Receive de-emphasis to 50 or 75 μs. Default is 75 μs.
 * 
 * @param value  1 = 50 μs. Used in Europe, Australia, Japan, China; 2 = 75 μs. Used in USA
 */
void SI4844::setFmDeemphasis(uint8_t value) {
    setProperty(FM_DEEMPHASIS, value);
}




/** @defgroup TOOLS Device Checking*/

/**
 * @ingroup TOOLS Detect Device
 * @brief   Checks communication with SI4844 via I2C
 * @details Checks if the SI4844 is available on the I2C bus. Remember that the SI4844 responds to the address 0x11
 * @return  true or false
 */
bool SI4844::detectDevice() {

  Wire.begin();
  // check 0x11 I2C address
  Wire.beginTransmission(SI4844_ADDRESS);
  return !Wire.endTransmission();
}


/**
 * @ingroup TOOLS Scan I2C Devices
 * @brief  Scans the I2C bus and returns the addresses of the devices found.
 * @details Searches for devices connected to the I2C bus. The addresses of the devices found are stored in the "device" array.
 * @param device array of device addresses found.
 * @return uint8_t number of devices found or 0 if no device found or error.
 */
uint8_t SI4844::scanI2CBus(uint8_t *device, uint8_t limit) {

  uint8_t error, address;
  uint8_t idxDevice = 0;

  Wire.begin();

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      device[idxDevice] = address;
      idxDevice++;
      if ( idxDevice > limit ) break;
    } else if (error == 4) {
      return 0;
    }
  }
  return idxDevice;
}

