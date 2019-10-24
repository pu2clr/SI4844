/*
 * This is a library for the SI4844, BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER,
 * ICs from Silicon Labs for the Arduino development environment.  
 * This library is intended to provide an easier interface for controlling the SI4844.
 * 
 * Author: Ricardo Lima Caratti (PU2CLR)
 * September, 2019
 */

#include "SI4844.h"

/*
* interrupt_hundler
* English: 
*   This function is called whenever the status of ATDD (SI4844) changes. 
*   It can occur, for example, when you use the analog tuner.  
*/

void SI4844::waitInterrupr(void)  {
  while (!data_from_si4844);
}

/*
* Initiate the SI4844 instance and connect the device (SI4844) to Arduino. 
* Calling this library should be the first thing to do to control the SI4844.
*
* @param resetPin  arduino pin used to reset the device
* @param interruprPin arduino pin used to handle interrupr      
* @param defaultBand band that the radio should start
*/
void SI4844::setup(unsigned int resetPin, unsigned int interruptPin, byte defaultBand) 
{

    this->resetPin = resetPin;
    this->interruptPin = interruptPin;
  
    // Arduino interrupt setup.
    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_hundler, RISING);

    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);

    data_from_si4844 = false;

    reset();

    // FM is the default BAND
    // See pages 17 and 18 (Table 8. Pre-defined Band Table) for more details
    setBand(defaultBand);

    // This sketch is using the value 44.
    setVolume(50); // Initiate with default volume control;

    // You need call it just once.  
    getFirmware();

}

/*
 * reset
 * See pages 7, 8, 9 and 10 of the programming guide.
 */
void SI4844::reset()
{
    waitToSend();
    
    setClockLow(); // See *Note on page 5
    data_from_si4844 = false;
    digitalWrite(resetPin, LOW);
    delayMicroseconds(200);
    digitalWrite(resetPin, HIGH);
    delayMicroseconds(200);
    waitInterrupr();
    delayMicroseconds(2500);
}

/*
 * Moves the device from power up to power down mode. 
 * See Si48XX ATDD PROGRAMMING GUIDE; AN610; page 45
 */
void SI4844::powerDown(void) {
    data_from_si4844 = false;
    // Wait until rady to send a command
    waitToSend();
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_POWER_DOWN);
    Wire.endTransmission();
    delayMicroseconds(2500);    
}

/*
 * Moves the device from power down to power up mode. 
 * See Si48XX ATDD PROGRAMMING GUIDE; AN610; page 45
 */
void SI4844::powerUp(void) {

    setBand(currentBand);

}

/*
 * Set the radio to a new band. 
 * See Table 8. Pre-defined Band Table in Si48XX ATDD PROGRAMMING GUIDE; AN610; pages 17 and 18  
 */
void SI4844::setBand(byte new_band)
{
    reset();

    currentBand = new_band;

    // Assigning 1 to bit 7. It means we are using external crystal
    // Silicon Labs; Si48XX ATDD PROGRAMMING GUIDE; AN610; page 7
    // Just another way to deal with bytes and bits using C/C++.
    new_band |= B10000000;
    new_band &= B10111111;

    data_from_si4844 = false;

    // Wait until rady to send a command
    waitToSend();

    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_POWER_UP);
    Wire.write(new_band);
    Wire.endTransmission();
    delayMicroseconds(2500);
    waitInterrupr();

    delayMicroseconds(2500);
    getStatus();
    delayMicroseconds(2500);
}

/*
 * isClearToSend
 * English:
 * Check if the ATDD (Si4844) is ready to receive the next command. 
 * See page 14 of the Program Guide.
 */
inline bool SI4844::isClearToSend(void)
{
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(ATDD_GET_STATUS);
    Wire.endTransmission();
    delayMicroseconds(2000);
    Wire.requestFrom(SI4844_ADDRESS, 0x01);
    status_response.raw[0] = Wire.read();
    return status_response.refined.CTS; // return 0 (false) or 1 (true)
}

/*
 * waitToSend
 * English:
 * Wait for the ATDD become Clear to Send. 
 * See page 14 of the Program Guide.
 */
inline void SI4844::waitToSend()
{

    while (!isClearToSend())
        ;
}

/*
 *  Deprecated
 *  Up or down the sound volume level/  
 *  @param char '+' up and '-' down 
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

/*
 *  Set sound volume level Up   
 *  
 */
void SI4844::volumeUp()
{
    if (volume <= 58) volume += 4;
    setVolume(volume);
}

/*
 *  Set sound volume level Down   
 *  
 */
void SI4844::volumeDown()
{
    if (volume >= 6) volume -= 4;
    setVolume(volume);
}


/*
 * Set the sound volume level. 
 * See Table 4, Si48XX ATDD PROGRAMMING GUIDE; AN610; page 11
 * @param byte volumeLevel (domain: 0 to 63) 
 */
void SI4844::setVolume(byte volumeLavel) {

    if (volumeLavel > 63) return; 
    
    waitToSend();
    
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(SET_PROPERTY);   // 
    Wire.write(0x00);           // ARG1 (is always 0x00)
    Wire.write(RX_VOLUME);      // ARG2 RX_VOLUME = 0X4000  (0x40)
    Wire.write(0x00);           // ARG3 RX_VOLUME = 0X4000  (0x00)
    Wire.write(0x00);           // ARG4
    Wire.write(volumeLavel);    // ARG5
    Wire.endTransmission();
    delayMicroseconds(2500);

    this->volume = volumeLavel;
}

/*
 * Set the sound volume level, bass and treble. 
 * @param byte bass and treble (domain: 0 to 8)
 *      0 -Bass boost +4 (max)
 *      1- Bass boost +3
 *      2- Bass boost +2
 *      3- Bass boost +1 (min)
 *      4- Normal (No Bass/Treble effect) (Default) 5- Treble boost +1 (min)
 *      6- Treble boost +2
 *      7- Treble boost +3
 *      8- Treble boost +4 (max)
 */
void SI4844::setBassTreble(byte bass_treble) {
    waitToSend();
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(0x00);
    Wire.write(SET_PROPERTY);
    Wire.write(0x40); // RX_BASS_TREBLE = 0x4002
    Wire.write(0x02);
    Wire.write(0x00);           // most significant byte
    Wire.write(bass_treble);    // BASSTREBLE[4:0]
    Wire.endTransmission();
    delayMicroseconds(2500);
}

/*
 * Less treble, more bass.
 */
void SI4844::bassTrebleDown() {
    if (bassTreble < 8 ) bassTreble++;
    setBassTreble(bassTreble);
}

/*
 * more treble, less bass
 */
void SI4844::bassTrebleUp() {
    if (bassTreble > 0 ) bassTreble--;
    setBassTreble(bassTreble);
}

/*
 * Set audio mode 
 * See Si48XX ATDD PROGRAMMING GUIDE; AN610; page 21
 * @param byte audio_mode 
 *             0 = Digital volume mode (no bass/treble effect, volume levels from 0 to 63) 
 *             1 = Bass/treble mode (no digital volume control, fixed volume level at 59)
 *             2 = Mixed mode 1 (bass/treble and digital volume coexist, max volume = 59) 
 *             3 = Mixed mode 2 (bass/treble and digital volume coexist, max volume = 63) 
 *             Default is 3 (Mixed mode 2)
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
si4844_audiomode_status_response SI4844::setAudioMode(byte audiomode, byte fm_mono, byte adjpt_attn, byte adjpt_steo, byte opcode) {
    
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
    do  { 
        Wire.requestFrom(SI4844_ADDRESS, 0x01); // request 1 byte response
        resp.raw = Wire.read();
    } while (!resp.status.CTS);

    return resp;
}


/*
 * Mutes the audio output.
 * 
 * @param byte value 0 = normal (no mute); 1 = Right side mute; 2; Left side mute; 3 = both side 
 */
void SI4844::audioMute(byte value) {

    waitToSend();
    
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(SET_PROPERTY);   // 
    Wire.write(0x00);           // ARG1 (is always 0x00)
    Wire.write(0x40);           // ARG2 RX_HARD_MUTE = 0X4001  (0x40)
    Wire.write(0x01);           // ARG3 RX_HARD_MUTE = 0X4001  (0x01)
    Wire.write(0x00);           // ARG4
    Wire.write(value);          // ARG5
    Wire.endTransmission();
    delayMicroseconds(2500);    
}

/*
 * Mutes the audio output.
 * 
 * @param bool on false = normal (no mute); true = mute 
 */
void SI4844::setAudioMute(bool on)
{
    if ( on )
        audioMute(3);
    else 
        audioMute(0);    
}

/*
 * Get tune freq, band, and others information, status of the device.
 * Use this method only if you want to deal with that information by yourself. 
 * This library has other methods to get that information easier. 
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

    /*
     * Get part number, chip revision, firmware, patch, and component revision numbers.
     * You do not need to call this method. It is executed just once at setup methos. 
     * There are other methods that give you that information.   
     * See page 22 of programming guide.
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

        data_from_si4844 = false;

        return &firmware_response;
    }

    /*
 * Get the current frequency of the radio in KHz. 
 * For example: FM, 103900 KHz (103.9 MHz);
 *              SW, 7335 KHz (7.34 MHz, 41m)   
 * 
 * @return float current frequency in KHz.  
 */
    float SI4844::getFrequency(void)
    {
        getStatus();
        String s;
        int addFactor = 0;
        int multFactor = 1;
        // Check CHFREQ bit[15] MSB = 1
        // See Page 15 of Si48XX ATDD PROGRAMMING GUIDE
        if (status_response.refined.BANDMODE == 0)
        {
            multFactor = 100;
            if (status_response.refined.d1 & B00001000)
            {
                status_response.refined.d1 &= B11110111;
                addFactor = 50;
            }
        }
        else if (status_response.refined.BANDMODE == 2)
        {
            multFactor = 10;
            if (status_response.refined.d1 & B00001000)
            {
                status_response.refined.d1 &= B11110111;
                addFactor = 5;
            }
        }

        s.concat(status_response.refined.d1);
        s.concat(status_response.refined.d2);
        s.concat(status_response.refined.d3);
        s.concat(status_response.refined.d4);

        float f = s.toFloat();

        data_from_si4844 = false;

        return (f * multFactor + addFactor);
    }

    /*
*  Check if the SI4844 has its status changed. If you move the tuner, for example,
*  the status of the device is changed. 
*
*  return true or false  
*/
    bool SI4844::hasStatusChanged(void)
    {
        return data_from_si4844;
    }

    /*
 * set the interrupr status to false. It will turn true after next interrupr  
 */
    void SI4844::resetStatus()
    {
        data_from_si4844 = false;
    }

    /* 
 * This method allows you to customize the frequency range of a band.
 * The SI4844 can work from 2.3–28.5 MHz on SW, 64.0–109.0MHz on FM
 * You can configure the band index 40, for example, to work between 27 to 28 MHz.
 * See Si48XX ATDD PROGRAMMING GUIDE, pages 17, 18, 19 and 20.
 * 
 * (top – button)/(bandSpace) must be betwenn 50 and 230
 * 
 * @param byte bandIndes; Predefined band index (valid values: betwenn 0 and 40)
 * @param unsigned button; Band Bottom Frequency Limit
 * @param unsigned top; Band Top Frequency Limit
 * @param byte bandSpace; Channel Spacing (use 5 or 10 - On FM 10 = 100KHz)
 * @return void
 */
    void SI4844::setCustomBand(byte bandIndex, unsigned botton, unsigned top, byte bandSpace)
    {

        union {
            struct
            {
                byte bandindex : 6;
                byte xowait : 1;
                byte xoscen : 1;
                unsigned botton;
                unsigned top;
                byte bandspace;
            } refined;
            byte raw[6];
        } customband;

        // The first thing that we have to do is switch to desired band
        setBand(bandIndex);

        // Now we can customize the band.
        data_from_si4844 = false;
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
        waitInterrupr();

        delayMicroseconds(2500);
        getStatus();
        delayMicroseconds(2500);
    }
