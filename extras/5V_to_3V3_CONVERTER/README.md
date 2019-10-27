# Making the Si4844 works with 5V Arduino UNO or similar

The SI4844 device works with  +3.3V only. If you are not using a +3.3V version of Arduino, you have to use a kind of converter. I have tested with success the SI4844 with 5V Arduino using the I2C Multiplexer TCA9548A. However, during my attempts, just using a regular bidirectional voltage converter was not enough. I mean, It has not worked  properly due to I2C communication requirement (I guess). So I have used the TCA9548A as a I2C bidirectional converter and a regular voltage converter to the digital pins, RESET and INTERRUPT.   

The schematic below shows that approach. 

![Making the Si4844 works with 5V Arduino UNO or similar](https://github.com/pu2clr/SI4844/blob/master/extras/5V_to_3V3_CONVERTER/ARDUINO5V_WITH_4844_schematic_.png)


