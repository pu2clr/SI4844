/*
  SI4844 Basic Test
  Simple test program for the Si4844 Arduino
  AM/FM Radio
  
  Original code from All About Circuits article by Raymond Genovese
  http://www.allaboutcircuits.com/projects/build-an-arduino-controlled-am-fm-sw-radio/
  
  Adapted for Teensy 3.2 by Nicholas Johnson 2017
*/

#include <Wire.h>

// chip I2C address is hard coded
#define Si_ADDRESS 0x11

// Si Commands --------------------------------------
#define ATDD_POWER_DOWN 0x11
#define ATDD_POWER_UP   0xE1
#define GET_REV         0x10
#define ATDD_XOSCEN     0x80
#define ATDD_XOWAIT     0x40
#define ATDD_GET_STATUS 0xE0

// Teensy Setup --------------------------------------
const byte INTpin=2;      // IRQ
const byte RESETpin=12;   // Reset
byte status_rsp[9];
volatile byte IRQflag;

void setup() {
  // set up pins and IRQ
  Serial.begin(115200); 

  pinMode(INTpin, INPUT);      // Interrupt
  pinMode(RESETpin,OUTPUT);     // Reset
  digitalWrite(RESETpin,HIGH);

  attachInterrupt(digitalPinToInterrupt(INTpin),SiISR,RISING);
  Wire.begin();
}

void loop() {
  byte band;
  byte bandmode;
  
  // Note: we are using "Band detection by system controller" mode 
  delay(2000);  // wait for serial monitor click
  Serial.println("*** Si4844 Quick Test **");
  Serial.println("...setting I2C speed to 10kHz");
  I2C_10kHz();
  Serial.println("...resetting Si4844");
  IRQflag=0;
  SiReset();
  
  // wait for an IRQ rising edge
  while(!IRQflag);
  // wait for 2.5 ms
  delayMicroseconds(2500);
  Serial.println("Si4844 reset complete");
  
  band=0;             // FM Band 0 - 87-108 mHz 
  bitSet(band,7);     // bit 7=1 for external crystal
  bitClear(band,6);   // bit 6=0 for normal crystal time
                      // arg[2]-[7] are not used in this mode
  IRQflag=0;
  
  Serial.println("...sending ATDD_POWER_UP with band set");
  
  Wire.beginTransmission(Si_ADDRESS);
  Wire.write(ATDD_POWER_UP);  
  // we are using an external 32.768 crystal (set bit 7)
  // no extra wait time (bit 6 is reset)
  Wire.write(band);
  Wire.endTransmission();
  
  // wait for a powerup IRQ
  while(!IRQflag);
  // wait for 2.5 ms because our IRQ is rising edge
  delayMicroseconds(2500);
  Serial.println("Si4844 ATDD_POWER_UP completed");
  Serial.println("...setting I2C speed to 50kHz");
  I2C_50kHz();
  delayMicroseconds(2000);
  
  // send a get status and wait for 2 ms before testing response byte
  Serial.println("...sending ATDD_GET_STATUS");
  get_status();
  Serial.println("...completed - tuner information is ready");

  // response bytes
  for(int x=0;x<4;x++)
  {
    Serial.print("Response byte ");
    Serial.print(x);
    Serial.print("= ");
    Serial.println(status_rsp[x],BIN);
  }
  
  Serial.print("Band Mode is ");
  bandmode=status_rsp[1]>>6;
  Serial.print(bandmode);
  Serial.println(" [0=FM 1=AM 2=SW]");
  Serial.print("Band Index is ");
  band=status_rsp[1]&B00011111;
  Serial.print(band);
  Serial.println(" [0-19 FM / 20-24 AM / 25-40 SW]");
  
  // print the frequency digits
  Serial.print("Tuner Frequency (mHz)= ");
  Serial.print(status_rsp[2]>>4);
  Serial.print(status_rsp[2]%16);
  Serial.print(status_rsp[3]>>4);
  Serial.print(".");
  Serial.println(status_rsp[3]%16);
  Serial.println();
  Serial.println("Chip information...");
  Wire.beginTransmission(Si_ADDRESS);
  Wire.write(GET_REV);  
  Wire.endTransmission();  
  delay(5);  
  
  Wire.requestFrom(Si_ADDRESS, 0x09);
  for(int x=0;x<9;x++){
    status_rsp[x]=Wire.read();
  }
  IRQflag=0;  
  
  // Tuning Loop --------------------------------------
  Serial.println("Entering tuning loop...");
  IRQflag=0;
  tuneloop:
  if(IRQflag)
   {
     Wire.beginTransmission(Si_ADDRESS);
     Wire.write(ATDD_GET_STATUS);  
     Wire.endTransmission();  
     Wire.requestFrom(Si_ADDRESS, 0x04);
     status_rsp[0]=Wire.read();
     status_rsp[1]=Wire.read();
     status_rsp[2]=Wire.read();
     status_rsp[3]=Wire.read();
     
     // print the frequency digits
     Serial.print(status_rsp[2]>>4);
     Serial.print(status_rsp[2]%16);
     Serial.print(status_rsp[3]>>4);
     Serial.print(".");
     Serial.println(status_rsp[3]%16);
     IRQflag=0;
   }
   goto tuneloop;
}
////////////////////////////////////////////////////////////////////////////////
// Utilities
////////////////////////////////////////////////////////////////////////////////
void SiReset()
{
  // reset Si4844 and back up
  digitalWrite(RESETpin,LOW);
  delayMicroseconds(200);
  digitalWrite(RESETpin,HIGH);
  delayMicroseconds(200);
}

void get_status()
{
  // do SiGetStatus until we have valid frequency info in status_rsp
  // an error out routine can be added here so it can escape
  // after a certain number of attempts
  Waitforgo:               
      SiGetStatus(); 
      if( ((bitRead(status_rsp[0],4)==0) || ( (status_rsp[2]==0) && (status_rsp[3]==0))) )
        {
        goto Waitforgo;       
        }
}

void SiGetStatus()
{
  // send a get status command and wait for 2 ms before testing response byte
  Wire.beginTransmission(Si_ADDRESS);
  Wire.write(ATDD_GET_STATUS);  
  Wire.endTransmission();  
  delayMicroseconds(2000);
  // test response byte
  Wire.requestFrom(Si_ADDRESS, 0x04);
  for(int x=0;x<4;x++)
  {
    status_rsp[x]=Wire.read();
  }
}
////////////////////////////////////////////////////////////////////////////////
// I2C Speed Settings
////////////////////////////////////////////////////////////////////////////////
/*
  NOTE: These speed routines for I2C are for an 8mhz, 3.3v Pro Mini
  They need to be changed for a 16 mHz UNO or other Arduinos
*/
void I2C_10kHz(){
  Wire.setClock(10000);
}
void I2C_50kHz(){
  Wire.setClock(50000);
} 

// The interrupt service routine
void SiISR(){
    IRQflag=1;
}


