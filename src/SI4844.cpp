/**
 * @mainpage SI48XX Arduino Library implementation
 *
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
 * 
 * @author PU2CLR - Ricardo Lima Caratti 
 * @date  2019-2020
 */

#include "SI4844.h"

/**
 * @brief Library handle interrupt
 * @details Handling interruptions.
 * @details This function simply captures the status change of the SI48XX via interruption.
 * @details Whenever the status of the ATDD changes, a hardware interrupt is triggered. For example, when you move the tuner
 * @details potenciometer, the ATDD sends a signal to the Arduino pin (INTERRUPT_PIN). The same is true when the ATDD is capturing 
 * @details mono FM signal and has switched to stereo. 
 * @details You can control the interrupt process via your sketch intead of this library. 
 * @see setStatusInterruptFromDevice, getStatusInterruptFromDevice, setup 
 */
#ifdef ESP8266    // if the controller is ESP8266, add IRAM_ATTR.
  IRAM_ATTR 
#endif
void interrupt_hundler()
{
   data_from_device = true;
}

/** @defgroup BF  Basic Functions */

/**
 * @ingroup BF
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
 * @ingroup BF 
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
 * @ingroup BF
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
 * @ingroup BF
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
 * @ingroup BF
 * @brief   Waiting for an external interrupt
 * @details This function is called whenever the status of ATDD (SI4844) changes. 
 * @details It can occur, for example, when you use the analog tuner.  
 */
void SI4844::waitInterrupt(void)
{
    
    while (!data_from_device)
        ;
}

/** @defgroup GB1  Receiver Functions */


/**
 * @ingroup GB1
 * @todo UNDER CONSTRUCTION
 * @brief   Initiates the SI48XX instance and connect the device (SI4844) to a microcontroller. 
 * @details This function initializes the Si48XX with the "Slide Switch Selects Band" configuration, where a resistor 
 * @details network (voltage divider) is connected to the BAND pin of the device (Si4844, Si4826, or Si4827). 
 * @details While in the setup function the band control is managed by the microcontroller, the setupSlideSwitch 
 * @details function assumes that you are using a mechanical band selector.
 * @details Calling this library should be the first thing to do to control the SI48XX.
 * @details If interruptPin is -1, it means you will control interrupt in your sketch. 
 * @details In this case, you have to call interrupt_hundler() (see SI4844.h)   
 * @param resetPin      arduino pin used to reset the device
 * @param interruptPin  interruprPin arduino pin used to handle interrupt 
 * @param hightClockSpeed hight I2C clock speed to be used by the system (optional - default 50000 - 50kHz).
 */
void SI4844::setupSlideSwitch(uint16_t resetPin, int interruptPin, uint32_t hightClockSpeed )
{
    int8_t newBand;
    this->resetPin = resetPin;
    this->interruptPin = interruptPin;

    setClockSpeed(hightClockSpeed);

    // Arduino interrupt setup.
    // if interruptPin parameter is < 0, it means the interrupt is being controlled by the user of this library
    if (interruptPin != -1 ) {
        pinMode(interruptPin, INPUT);
        attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_hundler, RISING);
    }

    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(1);    
    data_from_device = false;

    Serial.print("\nDEBUG - RESET");    
    this->reset();
    Serial.print("\nDEBUG - POWER UP");    
    this->powerUp();
    Serial.print("\nDEBUG - SET BAND");    

    newBand = this->getValidBandIndex();

    if ( newBand != 0 )
        this->setBandSlideSwitch(); // Starts with band index 0 and checks later if it corresponds with real band


    Serial.print("DEBUG - END SETUP");   
    // You need call it just once.
    // getFirmware();
}


/**
 * @ingroup GB1
 * @brief Initiates the SI4844 instance and connect the device (SI4844) to Arduino. 
 * @details Calling this library should be the first thing to do to control the SI4844.
 * @details If interruptPin is -1, it means you will control interrupt in your sketch. 
 * @details In this case, you have to call interrupt_hundler() (see SI4844.h)   
 * @param resetPin      arduino pin used to reset the device
 * @param interruptPin  interruprPin arduino pin used to handle interrupt 
 * @param defaultBand   band that the radio should start. If -1 no band is selected and you must select one after starting. Default 0 = FM.
 * @param hightClockSpeed hight I2C clock speed to be used by the system (optional - default 50000 - 50kHz).
 */
void SI4844::setup(uint16_t resetPin, int interruptPin, int8_t defaultBand, uint32_t hightClockSpeed )
{

    this->resetPin = resetPin;
    this->interruptPin = interruptPin;

    setClockSpeed(hightClockSpeed);

    // Arduino interrupt setup.
    // if interruptPin parameter is < 0, it means the interrupt is being controlled by the user of this library
    if (interruptPin != -1 ) {
        pinMode(interruptPin, INPUT);
        attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_hundler, RISING);
    }

    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(1);    
    data_from_device = false;

    reset();

    // FM is the default BAND
    // See pages 17 and 18 (Table 8. Pre-defined Band Table) for more details
    if ( defaultBand >= 0 )
        setBand(defaultBand);
        
    setVolume(30);

    // You need call it just once.
    getFirmware();
}


/**
 * @ingroup BF
 * @brief Sets the MCU RESET pin 
 * @param MCU pin (example Arduino Nano: 12 - Digital pin 12)
 */
void SI4844::setResetPin(uint16_t resetPin) {
    this->resetPin = resetPin;
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(1);
}

/**
 * @ingroup BF
 * @brief Sets the MCU Interrupt pin 
 * @param MCU pin (example Arduino Nano: 2 - The Arduino digital pin 2 allows external interrupt)
 */
void SI4844::setInterruptPin(int interruptPin) {
    this->interruptPin = interruptPin;
}

/**
 * @ingroup GB1
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
 * @ingroup GB1
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
    delayMicroseconds(200);
    digitalWrite(resetPin, HIGH);
    delayMicroseconds(200);
    waitInterrupt();
    delayMicroseconds(2500);
}


/**
 * @ingroup GB1
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

void SI4844::setDefaultBandIndx( uint8_t bandidx) {
    this->currentBand = bandidx;
}

/**
 * @ingroup GB1
 * @todo UNDER CONSTRUCTION
 * @brief Power the device up
 * @details Moves the SI4844 device from power down to power up 
 * @see Si48XX ATDD PROGRAMMING GUIDE; AN610; page 45
 */
void SI4844::powerUp(void)
{
    this->reset();      // Resets the system and wait for a iterrupt
    this->getStatus();  // Gets the first status after resetting

    // Chacks "ATDD device detects band" mode is configured
    if ( status_response.refined.BCFG0 != 0 ) this->setBand(0);

    Serial.print("\n=======> ATDD device detects band");

    this->currentBand = 0;
    this->xowait = 1;

    si4844_arg_band_index rxBandSetup; 

    rxBandSetup.refined.XOSCEN = this->xoscen;
    rxBandSetup.refined.XOWAIT = this->xowait;
    rxBandSetup.refined.BANDIDX = this->currentBand;

    data_from_device = false;

    Serial.print("\n=======> First powerUp ");

    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_POWER_UP);
    Wire.write(rxBandSetup.raw);
    Wire.endTransmission();
    delayMicroseconds(2500);

    waitInterrupt();

    Serial.print("\n=======> powerUp after powerUP");

    delayMicroseconds(2500);
    getStatus();

    Serial.print("\n=======> powerUp after powerUP and getStatus");
    
    delayMicroseconds(2500);

    this->currentBand = status_response.refined.BANDIDX;

    rxBandSetup.refined.XOSCEN = this->xoscen;
    rxBandSetup.refined.XOWAIT = this->xowait;
    rxBandSetup.refined.BANDIDX = this->currentBand;

    Serial.print("\n=======> Secound powerUp ");
    this->reset();

    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_POWER_UP);
    Wire.write(rxBandSetup.raw);
    Wire.endTransmission();
    delayMicroseconds(2500);

    waitInterrupt();

    this->setVolume(this->volume);

    Serial.print("\n=======> powerUp finish powerUP");
}

/**
 * @ingroup GB1
 * @brief Sets Crystal Oscillator Enable
 * @details 0 = Use external RCLK (crystal oscillator disabled).
 * @details 1 = Use crystal oscillator (XTALI and XTALO with external 32.768 kHz crystal).
 * @details See the SI48XX Data Sheet Application Schematic for external BOM details.
 * @see See Table 8. Pre-defined Band Table in Si48XX ATDD PROGRAMMING GUIDE; AN610; pages 17 and 18  
 * 
 * @param XOSCEN  1 = Enable (XTALI and XTALO with external 32.768 kHz crystal)
 */
void SI4844::setCrystalOscillatorEnable(uint8_t XOSCEN ) {
    this->xoscen = XOSCEN;
}

/**
 * @ingroup GB1
 * @brief Sets Crystal Oscillator Stabilization Wait Time After Reset.
 * @details 0 = 600 ms (for typical crystal)
 * @details 1 = 900 ms (for crystal requiring extra stabilization time)
 * @details Note: Applicable to Si4822/26/40/44A parts only. Later Si4827/44B parts don’t care this bit and will wait till crystal oscillation is stable unconditionally.
 * @see See Table 8. Pre-defined Band Table in Si48XX ATDD PROGRAMMING GUIDE; AN610; pages 17 and 18  
 * 
 * @param XOSCEN  1 = Enable (XTALI and XTALO with external 32.768 kHz crystal)
 */
void SI4844::setCrystalOscillatorStabilizationWaitTime(uint8_t XOWAIT) {
    this->xowait = XOWAIT;
} 



/**
 * @ingroup GB1
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

    this->currentBand = new_band;

    // Assigning 1 to bit 7. It means we are using external crystal
    // Silicon Labs; Si48XX ATDD PROGRAMMING GUIDE; AN610; page 7
    // Just another way to deal with bytes and bits using C/C++.

    si4844_arg_band_index rxBandSetup; 

    rxBandSetup.refined.XOSCEN = this->xoscen;
    rxBandSetup.refined.XOWAIT = this->xowait;
    rxBandSetup.refined.BANDIDX = this->currentBand;

    data_from_device = false;

    // Wait until rady to send a command
    waitToSend();

    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_POWER_UP);
    Wire.write(rxBandSetup.raw);
    Wire.endTransmission();
    delayMicroseconds(2500);
    waitInterrupt();

    delayMicroseconds(2500);
    getStatus();
    delayMicroseconds(2500);

    this->setVolume(this->volume);
}

/**
 * @ingroup GB1
 * @todo UNDER CONSTRUCTION...
 * @brief Sets a new band to the device configured as Slide Switch
 * @see See Table 8. Pre-defined Band Table in Si48XX ATDD PROGRAMMING GUIDE; AN610; pages 17 and 18  
 * @param band  band index number. 
 * @see Si4822/26/27/40/44 A NTENNA , SCHEMATIC , LAYOUT, AND DESIGN GUIDELINES 
 */
void SI4844::setBandSlideSwitch()
{
    int8_t newBand;
   
    // do {
        newBand = this->getValidBandIndex();
    // } while (newBand == -1);



    Serial.print("**** NewBand: ");
    Serial.print(newBand); 

    // Set band to real band defined by the slice switch

    if (newBand >= 0 && newBand != this->getCurrentBand() ) {
        si4844_arg_band_index rxBandSetup; 

        Serial.print("\n Antes do reset");
        this->reset();
        delayMicroseconds(2500);
        rxBandSetup.refined.XOSCEN = this->xoscen;
        rxBandSetup.refined.XOWAIT = this->xowait;
        rxBandSetup.refined.BANDIDX = newBand;

        data_from_device = false;  
        Serial.print("\n Antes do waitToSend");
        waitToSend();

        Wire.beginTransmission(SI4844_ADDRESS);
        Wire.write(ATDD_POWER_UP);
        Wire.write(rxBandSetup.raw);
        Wire.endTransmission();
        delayMicroseconds(2500);
        waitInterrupt();

        Serial.print("\n PASSOU PELO ATDD_POWER_UP");

        delayMicroseconds(2500);
        getStatus();
        delayMicroseconds(2500);

        Serial.print("\n PASSOU PELO getStatus");

        this->setVolume(this->volume);      
        this->currentBand = newBand;
        Serial.print("\n FIM da CHAMADA");
    }

}



/** 
 * @ingroup GB1
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
 * @param  dfband Default Band Settings; 0 = Allow host controller to override the band property settings; 1 = Force to use tuner default band property settings (Applicable to Si4827 part only)
 * @param  uni_am Universal AM Band. 0 = Disable universal AM band (default AFC range of 1.1 kHz); 1 = Enable universal AM band (wider AFC range in tuning); Applicable to Si4827 and Si4844B parts and AMRX mode only
 * @param  tvreq TV Audio Channel Frequency Display; 0 = Disable TV audio channel frequency display format; 1 = Enable TV audio channel frequency display format; Applicable to Si4827 and Si4844B parts and FMRX mode only
 */
void SI4844::setCustomBand(uint8_t bandIndex, uint16_t  botton, uint16_t  top, uint8_t bandSpace, uint8_t dfband, uint8_t uni_am, uint8_t tvreq )
{
    SI4844_arg_band customband;

    this->currentBand = bandIndex;

    reset();

    // Now we can customize the band.
    data_from_device = false;
    customband.refined.BANDIDX = bandIndex;
    customband.refined.XOSCEN = this->xoscen;
    customband.refined.XOWAIT = this->xowait;
    customband.refined.BANDBOT_HIGH = highByte(botton);
    customband.refined.BANDBOT_LOW = lowByte(botton);
    customband.refined.BANDTOP_HIGH = highByte(top);
    customband.refined.BANDTOP_LOW = lowByte(top);
    customband.refined.CHSPC = bandSpace;
    customband.refined.DFBAND = dfband; 
    customband.refined.UNI_AM = uni_am;
    customband.refined.TVFREQ = tvreq;
    customband.refined.DUMMY = 0;

    // Wait until rady to send a command
    waitToSend();

    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_POWER_UP);
    Wire.write(customband.raw[0]);
    Wire.write(customband.raw[1]);
    Wire.write(customband.raw[2]);
    Wire.write(customband.raw[3]);
    Wire.write(customband.raw[4]);
    Wire.write(customband.raw[5]);
    Wire.write(customband.raw[6]);

    Wire.endTransmission();
    delayMicroseconds(2500);
    waitInterrupt();

    delayMicroseconds(2500);
    getStatus();
    delayMicroseconds(2500);
    this->setVolume(this->volume);

}




/**
 * @ingroup BF 
 * @brief Checks the CTS status.
 * @details Checks whether the device is ready to receive a new command. 
 * @return true 
 * @return false 
 */
bool SI4844::isClearToSend(void)
{
    delayMicroseconds(2000);
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_GET_STATUS);
    Wire.endTransmission();
    delayMicroseconds(2000);
    Wire.requestFrom(SI4844_ADDRESS, 1);
    status_response.raw[0] = Wire.read();
    return status_response.refined.CTS; // return 0 (false) or 1 (true)
}

/**
 * @ingroup BF
 * @brief Wait for the ATDD become Clear to Send. 
 * @details Waits for CTS status
 */
inline void SI4844::waitToSend()
{

    while (!isClearToSend())
        ;
}

/**
 * @ingroup GB1
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
 * @ingroup GB1
 * @brief Increases the volume level
 */
void SI4844::volumeUp()
{
    if (volume < 63)
        volume++;
    setVolume(volume);
}

/**
 * @ingroup GB1
 * @brief Decreases the volume level
 */
void SI4844::volumeDown()
{
    if (volume >= 6)
        volume--;
    setVolume(volume);
}

/**
 * @ingroup GB1
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
 * @ingroup GB1
 * @brief Gets the current volume value stored in SI4844 device. 
 * @details Use getVolume instead. 
 * @return byte 
 */
byte SI4844::getVolumeProperty() {
    uint16_t volumeProperty = getProperty(RX_ACTUAL_VOLUME);
    return (byte)volumeProperty;
}

/**
 * @ingroup GB1
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
    this->bassTreble = bass_treble;
}

/**
 * @ingroup GB1
 * @brief Less treble, more bass.
 */
void SI4844::bassTrebleDown()
{
    if (bassTreble < 8)
        bassTreble++;
    setBassTreble(bassTreble);
}

/**
 * @ingroup GB1
 * @brief More treble, less bass
 */
void SI4844::bassTrebleUp()
{
    if (bassTreble > 0)
        bassTreble--;
    setBassTreble(bassTreble);
}

/**
 * @ingroup GB1
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
 * @ingroup GB1
 * @brief  Mutes the audio output.
 * 
 * @param value value 0 = normal (no mute); 1 = Right side mute; 2; Left side mute; 3 = both side 
 */
void SI4844::audioMute(byte value)
{
    setProperty(RX_HARD_MUTE, value);
}

/**
 * @ingroup GB1
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
 * @ingroup GB1
 * @brief Get tune freq, band, and others information, status of the device.
 * @details Use this method only if you want to deal with that information by yourself. 
 * @details This library has other methods to get that information easier. 
 * 
 * @return a pointer to a structure type si4844_status_response
 */
si4844_status_response *SI4844::getStatus()
{
    // setClockHigh();
    setClockLow();
    waitToSend();
    do
    {
        // data_from_device = false;
        // waitInterrupt();
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
    } while (status_response.refined.CTS == 0 || status_response.refined.INFORDY == 0 || (status_response.raw[2] == 0 && status_response.raw[3] == 0));
    setClockLow();
    return &status_response;
}

/**
 * @ingroup GB1
 * @brief Band CFG0 (Band Detection Configuration)
 * @details The ATDD device has two operating modes of band detection configuration options: 
 * @details either the ATDD device detects the band or the system controller detects and controls 
 * @details the band by its own mechanism. 
 * @details For the ATDD device, the BAND pin of the device must be connected to the band switch resistor.
 * 
 * @return false = ATDD device detects band; True = Host detects band
 */
bool SI4844::isHostDetectionBandConfig() {
    si4844_status_response *s;
    s = this->getStatus();
    return s->refined.BCFG0; 
}


/**
 * @ingroup GB1
 * @brief   Retrieves the current valid BAND INDEX.  
 * @details Waits until INFORDY is set to 1.  
 * @details Once INFORDY is 1, the host can read and display the current status, including band mode, station, and stereo states.  
 * @return  The current valid BANDIDX.  
 */
int8_t SI4844::getValidBandIndex() {
    uint8_t count = 0;
    do { 
         this->getStatus();
         delay(1);
         count++;
      } while (this->status_response.refined.INFORDY == 0 && count < 50 );  

    if ( this->status_response.refined.BANDIDX > DEVICE_LAST_VALID_INDEX_BAND || count >= 50  ) return -1;
    return  this->status_response.refined.BANDIDX;     
}



/** 
 * @ingroup GB1
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
 * @ingroup GB1
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
 * @ingroup GB1
 * @brief Get the current frequency of the radio in KHz in uint32_t (long integer) . 
 * @details For example: FM, 103900 KHz (103.9 MHz); SW, 7335 KHz (7.34 MHz, 41m)   
 * @details It is useful to save memory
 * 
 * @return uint32_t current frequency in KHz.  
 */
uint32_t SI4844::getFrequencyInteger(void)
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

    uint32_t f;

    f = (status_response.refined.d4);
    f += (status_response.refined.d3) * 10;
    f += (status_response.refined.d2) * 100;
    f += (status_response.refined.d1) * 1000;

    data_from_device = false;

    return (f * multFactor + addFactor);
}



/** 
 * @ingroup GB1 
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
 * @ingroup GB1
 * @brief set the interrupr status to false. It will turn true after next interrupr  
 */
void SI4844::resetStatus()
{
    data_from_device = false;
}



/**
 * @ingroup GB1
 * @brief Sets AM Soft Mute Max Attenuation..  
 * @details Maximum attenuation to apply when in soft mute. Specified in units of dB. Default maximum attenuation is 16 dB.
 * @details Set to 0 to disable soft mute. Valid range: 0 - 63. 
 * @param value  number between 0 and 63
 */
void SI4844::setAmSoftMuteMaxAttenuation(uint8_t value) {
    setProperty(AM_SOFT_MUTE_MAX_ATTENUATION, value);
}

/**
 * @ingroup GB1
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
 * @ingroup GB1
 * @brief Sets de-emphasis time constant. 
 * @details Sets the FM Receive de-emphasis to 50 or 75 μs. Default is 75 μs.
 * 
 * @param value  1 = 50 μs. Used in Europe, Australia, Japan, China; 2 = 75 μs. Used in USA
 */
void SI4844::setFmDeemphasis(uint8_t value) {
    setProperty(FM_DEEMPHASIS, value);
}


/**
 * @ingroup GB1
 * @brief Sets the attack and decay rates when entering and leaving soft mute. 
 * @details Later values increase rates, and lower values decrease rates. 
 * @details The CTS is set when it is safe to send the next command. This property may only be set or read when in powerup mode. The default is 64.
 * 
 * @param value  1—255 (default 64)
 */
void SI4844::setFmSoftMuteRate(uint8_t value) {
    setProperty(FM_SOFT_MUTE_RATE, value);
}



/**
 * @ingroup GB1
 * @brief Configures attenuation slope during soft mute in dB attenuation per dB SNR below the soft mute SNR threshold.
 * @details Soft mute attenuation is the minimum of SMSLOPE x (SMTHR – SNR) and SMATTN.
 * @details The recommended SMSLOPE value is CEILING(SMATTN/SMTHR). SMATTN and SMTHR are set via the FM_SOFT_MUTE_MAX_ATTENUATION and FM_SOFT_MUTE_SNR_THRESHOLD properties.
 * @details The CTS bit is set when it is safe to send the next command. This property may only be set or read when in power up mode.
 * 
 * @param value  0–63 (default 2)
 */
void SI4844::setFmSoftMuteSlope(uint8_t value) {
    setProperty(FM_SOFT_MUTE_RATE, value);
}


/**
 * @ingroup GB1
 * @brief Sets the blend threshold for stereo indicator.
 * @details The CTS bit is set when it is safe to send the next command. This property may only be set or read when in powerup mode.
 * @details The default value is band dependent (either 0x9F or 0xB2)
 * @details Default: 0x009F or 0x00B2 (Band dependent)
 * @param value  0–0xFF
 */
void SI4844::setBlendThresholdStereoIndicator(uint16_t value) {
    setProperty(FM_STEREO_IND_BLEND_THRESHOLD, value);
}


/**
 * @ingroup GB1
 * @brief Sets the attack and decay rates when entering or leaving soft mute. 
 * @details The value specified is multiplied by 4.35 dB/s to come up with the actual attack rate. The CTS bit is set when it is safe to send the next command.
 * @details This property may only be set or read when in power up mode. The default rate is 278 dB/s.
 * @details Determines how quickly the AM goes into soft mute when soft mute is enabled. The actual rate is calculated by taking the value written to the field
 * @details and multiplying it with 4.35 dB/s. The default rate is 278 dB/s (SMRATE[15:0] = 0x0040).
 * @details Default: 0x0040; Actual Rate: SMRATE x 4.35; Units: dB/s
 * 
 * @param value  1–255
 */
void SI4844::setAmSoftMuteRate(uint8_t value) {
    setProperty(AM_SOFT_MUTE_RATE, value);
}

/**
 * @ingroup GB1
 * @brief Configures attenuation slope during soft mute in dB attenuation per dB SNR below the soft mute SNR threshold.
 * @details Soft mute attenuation is the minimum of SMSLOPE x (SMTHR – SNR) and SMATTN. The recommended SMSLOPE value is CEILING(SMATTN/SMTHR).
 * @details SMATTN and SMTHR are set via the AM_SOFT_MUTE_MAX_ATTENUATION and AM_SOFT_MUTE_SNR_THRESHOLD properties. The CTS bit is
 * @details set when it is safe to send the next command. This property may only be set or read when in power up mode. The
 * @details default slope is 2 dB/dB. Default: 0x0001 Units: dB/dB
 * 
 * @param value  1–5
 */
void SI4844::setAmSoftMuteSlope(uint8_t value) {
    setProperty(AM_SOFT_MUTE_SLOPE, value);
}

/**
 * @ingroup GB1
 * @brief  Sets the SNR threshold to engage soft mute.
 * @details Whenever the SNR for a tuned frequency drops below this threshold the AM reception will go in soft mute, provided soft mute max attenuation property is non-zero. The CTS bit is set when it is safe to send the next command. 
 * @details This property may only be set or read when in power up mode. The default SNR threshold is 8.
 * @details Default: 0x0008; Units: dB; 
 * 
 * @param value   0–63
 */
void SI4844::setAmSoftMuteSnrThreshold(uint8_t value) {
    setProperty(AM_SOFT_MUTE_SNR_THRESHOLD, value);
}


/**
 * @ingroup GB1
 * @brief Sets the frequency of the REFCLK from the output of the prescaler
 * @details The REFCLK range is 31130 to 34406 Hz (32768 ±5% Hz) in 1 Hz steps, or 0 (to disable AFC).
 * @details For example, an reference clock at XTALI pin of 13 MHz would require a prescaler value of 400 to divide it to 32500 Hz REFCLK.
 * @details The reference clock frequency property would then need to be set to 32500 Hz. Reference clock frequencies between 31130 Hz and 40 MHz are supported, however, there are gaps in frequency coverage for prescaler values ranging from 1 to 10, or frequencies up to 311300 Hz.
 * @param value  31130-34406 (default 32768)
 * @see SI48XX ATDD Programming Guide (AN610) page 26.
 */
void SI4844::setReferenceClockFrequency(uint16_t value) {
    setProperty(REFCLK_FREQ, value);
}


/**
 * @ingroup GB1
 * @brief Sets the number used by the prescaler to divide the external reference clock frequency down to the internal REFCLK.
 * @details The range may be between 1 and 4095 in 1 unit steps. For example, an XTALI reference clock of 13 MHz would require a prescaler value of 400 to divide it to 3250 Hz. 
 * @details The reference clock frequency property would then need to be set to 32500 Hz. The reference clock must be valid 10 ns before the chip reset signal. In addition, the reference clock must be valid at all times for proper tuning and AFC operations. 
 * @details XTALI reference clock input frequency could be changed but is not recommended and REFCLK properties values need to be reconfigured accordingly.
 * @details Prescaler for Reference Clock is an integer number used to divide clock frequency down to REFCLK frequency. The allowed REFCLK frequency range is between 31130 and 34406 Hz (32768 5%), or 0 (to disable AFC).
 * @param value 1-4095 (default 1)
 * @see SI48XX ATDD Programming Guide (AN610) page 27.
 */
void SI4844::setReferenceClockPrescaler(uint16_t value) {
    setProperty(REFCLK_PRESCALE, value);
}



/** @defgroup TOOLS Helper Functions */

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



/**
 * @ingroup TOOLS Covert numbers to char array
 * @brief Converts a number to a char array
 * @details It is useful to mitigate memory space used by functions like sprintf or other generic similar functions
 * @details You can use it to format frequency using decimal or thousand separator and also to convert small numbers.
 *
 * @param value  value to be converted
 * @param strValue char array that will be receive the converted value
 * @param len final string size (in bytes)
 * @param dot the decimal or thousand separator position
 * @param separator symbol "." or ","
 * @param remove_leading_zeros if true removes up to two leading zeros (default is true)
 */
void SI4844::convertToChar(uint32_t value, char *strValue, uint8_t len, uint8_t dot, uint8_t separator, bool remove_leading_zeros)
{
    char d;
    for (int i = (len - 1); i >= 0; i--)
    {
        d = value % 10;
        value = value / 10;
        strValue[i] = d + 48;
    }
    strValue[len] = '\0';
    if (dot > 0)
    {
        for (int i = len; i >= dot; i--)
        {
            strValue[i + 1] = strValue[i];
        }
        strValue[dot] = separator;
    }

    if (remove_leading_zeros)
    {
        if (strValue[0] == '0')
        {
            strValue[0] = ' ';
            if (strValue[1] == '0')
                strValue[1] = ' ';
        }
    }
}


/**
 * @ingroup TOOLS 
 * @brief Gets Formatted Frequency
 * @details Gets the currente frequency of the receiver and return a point to char (string) with the formatted frequency 
 * @param  removeRightDigit number of less significant digits to be removed (FM mode only)
 * @return  point to char with the formatted current frequency
 */
char* SI4844::getFormattedFrequency(uint8_t removeRightDigit, uint8_t thousandsSeparator) {

    uint32_t f = this->getFrequencyInteger();
    if ( this->getStatusBandMode() == 0 ) { 
        this->convertToChar(f,this->strFormattedCurrentFrequenct,6,3,thousandsSeparator,true);
        strFormattedCurrentFrequenct[7-removeRightDigit] = '\0';
    } else { 
         this->convertToChar(f,this->strFormattedCurrentFrequenct,5,(f > 999)? 2:0,'.',true);
    }   

    return this->strFormattedCurrentFrequenct;
}
