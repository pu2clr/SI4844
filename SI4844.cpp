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
* Portuguese:
*   Esta função é executada toda vez que uma mudança de status ocorre no ATDD (SI4844).
*   Se você mover o sintonizador analógico (potenciômetro), por exemplo, em busca de uma 
*   estação, esta função será chamada
*/

void SI4844::waitInterrupr(void)  {
  while (!data_from_si4844);
}

/*
* Initiate the SI4844 instance and connect the device (SI4844) to Arduino. 
* Use of this method is required to handle ATDD.
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
    setVolume('?'); // Initiate with default volume control;
}

/*
 * reset
 * English
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
 * setBand 
 * English...: change the band 
 * Portuguese: Altera a banda 
 * Reference.: Silicon Labs; Si48XX ATDD PROGRAMMING GUIDE; AN610; pages 17 and 18  
 */
void SI4844::setBand(byte new_band)
{

    reset();

    // Assigning 1 to bit 7. It means we are using external crystal
    // Silicon Labs; Si48XX ATDD PROGRAMMING GUIDE; AN610; page 7

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
 * Portuguese:
 * Verifica se o Si4844 está pronto para receber o próximo comando
 * Veja a página 14 do guia de programação
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
 * Portuguese:
 * Espera o Si4844 ficar pronto para receber comando
 * Veja a página 14 do guia de programação
 */
inline void SI4844::waitToSend()
{

    while (!isClearToSend())
        ;
}



/*
 *  setVolume 
 *  English
 *  Send a command to chenge the sound volume. 
 *  Parameter: Use '+' to increase and '-' to decrease. 
 * 
 *  Portuguese 
 *  Envia um comando para o ATDD. 
 *  Parâmetro: '+' aumenta o volume e '-' diminui o volume. 
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

void SI4844::setVolume(byte volumeLavel) {

    if (volumeLavel > 63) return; 
    
    waitToSend();
    
    Wire.beginTransmission(SI4844_ADDRESS);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);
    Wire.write(RX_VOLUME);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(volume);
    Wire.endTransmission();
    delayMicroseconds(2500);
}

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

si4844_firmware_response *SI4844::getFirmware(void) {
  
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


float SI4844::getFrequency(void)
{

    getStatus();

    String s;
    int addFactor = 0;
    int multFactor = 1;
 
    // Check CHFREQ bit[15] MSB = 1 
    // See Page 15 of Si48XX ATDD PROGRAMMING GUIDE
    if (status_response.refined.BANDMODE == 0 ) {
        multFactor = 100;
        if ( status_response.refined.d1 & B00001000 ) { 
          status_response.refined.d1 &=  B11110111; 
          addFactor = 50;
        }
    } else if (status_response.refined.BANDMODE == 2)   { 
      multFactor = 10;
      if (status_response.refined.d1 & B00001000) {
         status_response.refined.d1 &=  B11110111; 
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
*  hasStatusChanged
*  Check if the SI4844 has its status changed. 
*
*  return true or false  
*/
bool SI4844::hasStatusChanged(void)
{
    return data_from_si4844;
}

void SI4844::resetStatus() {
      data_from_si4844 = false;
}
