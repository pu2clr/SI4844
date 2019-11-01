# Making the Si4844 works with 5V Arduino UNO or similar

The SI4844 device works with  +3.3V only. If you are not using a +3.3V version of Arduino, you have to use a kind of converter. I have tested it with success by using the I2C Multiplexer TCA9548A and Arduino UNO. However, during my attempts, just using a regular bidirectional voltage converter was not enough. I mean, It has not worked  properly due to I2C communication requirement (I guess). So I have used the TCA9548A as a I2C bidirectional converter and a regular voltage converter to the digital pins, RESET and INTERRUPT. If you follow the idea of the schematic below you will get success.   


__I used what I had in my hands. However, I think there are other kind of components or methods that can be better than my approach__. 

The schematic below shows that approach. 

![Making the Si4844 works with 5V Arduino UNO or similar](https://github.com/pu2clr/SI4844/blob/master/extras/5V_to_3V3_CONVERTER/ARDUINO5V_WITH_4844_schematic_.png)



__IMPORTANT__:
You have to include in your sketch (setup function) these three lines:

```cpp
  Wire.beginTransmission(0x70);
  Wire.write(1 << channel);
  Wire.endTransmission(); 
```

where __channel__ is the port that you have choosen to connect the arduino to the Si4844. The schematic above shows channel (port) 0. So, the code should be:

```cpp
  Wire.beginTransmission(0x70);
  Wire.write(1); // Yes. In this particular case just Wire.write(1). See the C/C++ "<<" for more detail.
  Wire.endTransmission(); 
```

if you choose the port 7 
```cpp
  Wire.beginTransmission(0x70);
  Wire.write(1 << 7); 
  Wire.endTransmission(); 
```


#### The table below show the possibilities to select an I2C channel on TCA9548A.

| channel / port | byte to send |  
| ---------------| ---- | 
| 0 | 00000001 |
| 1 | 00000010 |
| 2 | 00000100 |
| 3 | 00001000 |
| 4 | 00010000 |
| 5 | 00100000 |
| 6 | 01000000 |
| 7 | 10000000 |


You can also implement the function below in your code

```cpp
inline void selectChannel(byte ch) {
  if ( ch < 8 ) {
    Wire.beginTransmission(0x70); // The const hex 0x70 is the TCA9548A I2C address
    Wire.write(1 << ch); 
    Wire.endTransmission(); 
  }  
}

```


## References

* [TCA9548ALow-Voltage8-ChannelI2C Switchwith Reset](http://www.ti.com/lit/ds/symlink/tca9548a.pdf)
* [TCA9548A Wiring & Test](https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout/wiring-and-test)
* [Adafruit TCA9548A 1-to-8 I2C Multiplexer Breakout](https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout)
* [Understanding the I2C Bus](http://www.ti.com/lit/an/slva704/slva704.pdf)
  
  

## Video about TCA9548A

* [TCA9548A I2C Multiplexer Module - With Arduino and NodeMCU](https://www.youtube.com/watch?v=XWQsqPQOW-U)
* [TCA9548A I2C MULTIPLEXER (& Voltage Shifter)](https://www.youtube.com/watch?v=LowMKYcBxNg)
  



