# This script uses the arduino-cli to compile the arduino sketches using command line (without Arduino IDE).
# It is very useful to check the library on multiple board types after bug fixes and improvements.
# It runs on a MacOS but you can port it to Linux or Windows easily.
# Some compilation results (bin and hex files) will be stores in your Download directory (/Users/<username>/Downloads/hex)
# ATTENTION: 1) Be careful with --output-dir arduino-cli option. Before compiling, this option removes all the contents of the last level folder. 
#               For example: if you use "--output-dir ~/Downloads", all the current content of the Downloads folder will be lost. 
#                            if you use "--output-dir ~/Downloads/hex", all current content of the hex folder will be lost and the Downloads 
#                            content will be preserved. 
#            2) I have enabled all the compile warnings (--warnings all) to check some questionable situations that can be avoided or modified to prevent future warnings.  
#            3) I have enabled  the "--verbose" parameter to show the deteiled logs of the compiling process.
# Please, see the file config_libraries_and_boards.sh
# Ricardo Lima Caratti Mar 2023

# compiles POC
echo "********************"
echo "Arduino ATmega328 based board"
echo "********************"
arduino-cli compile -b arduino:avr:nano ./SI4844_00_MINIMAL --output-dir ~/Downloads/hex/atmega/SI4844_00_MINIMAL --warnings all
arduino-cli compile -b arduino:avr:nano ./SI4844_00_POC --output-dir ~/Downloads/hex/atmega/SI4844_00_POC  --warnings all
arduino-cli compile -b arduino:avr:nano ./SI4844_00_POC_CUSTOM_BAND --output-dir ~/Downloads/hex/atmega/SI4844_00_POC_CUSTOM_BAND  --warnings all
arduino-cli compile -b arduino:avr:nano ./SI4844_01_OLED --output-dir ~/Downloads/hex/atmega/SI4844_01_OLED --warnings all
arduino-cli compile -b arduino:avr:nano ./SI4844_01_OLED_V2 --output-dir ~/Downloads/hex/atmega/SI4844_01_OLED_V2 --warnings all
arduino-cli compile -b arduino:avr:nano ./SI4844_02_LCD16x02 --output-dir ~/Downloads/hex/atmega/SI4844_02_LCD16x02  --warnings all
arduino-cli compile -b arduino:avr:nano ./SI4844_03_TM1638_7SEG_DISPLAY --output-dir ~/Downloads/hex/atmega/7SEG  --warnings all
arduino-cli compile -b arduino:avr:nano ./SI4844_04_NOKIA5110 --output-dir ~/Downloads/hex/atmega/NOKIA5110 
arduino-cli compile -b arduino:avr:nano ./TEST/SI4844_BASS_TREBLE --output-dir ~/Downloads/hex/atmega/SI4844_BASS_TREBLE  --warnings all
arduino-cli compile -b arduino:avr:nano ./TEST/SI4844_00_POC_CUSTOM_BAND3 --output-dir ~/Downloads/hex/atmega/SI4844_00_POC_CUSTOM_BAND3  --warnings all
arduino-cli compile -b arduino:avr:nano ./TEST/SI4844_FIRMWARE --output-dir ~/Downloads/hex/atmega/SI4844_FIRMWARE  --warnings all
arduino-cli compile -b arduino:avr:nano ./SLIDE_SWITCH/SI4844_01_OLED_SLIDE_SWITCH  --output-dir ~/Downloads/hex/atmega/SLIDE_SWITCH_OLED  --warnings all
arduino-cli compile -b arduino:avr:nano ./SLIDE_SWITCH/SI4844_02_LCD16x2_SLIDE_SWITCH  --output-dir ~/Downloads/hex/atmega/SLIDE_SWITCH_LCD16x2  --warnings all
arduino-cli compile -b arduino:avr:nano ./SLIDE_SWITCH/SI4844_03_TM1637_CUSTM_BAND_SWITCH  --output-dir ~/Downloads/hex/atmega/SI4844_03_TM1637_CUSTM_BAND_SWITCH 
arduino-cli compile -b arduino:avr:nano ./SLIDE_SWITCH/SI4844_03_TM1637_SLIDE_SWITCH  --output-dir ~/Downloads/hex/atmega/SI4844_03_TM1637_SLIDE_SWITCH  



echo "********************"
echo "ATmega MiniCore - Standalone" 
echo "********************"

arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto ./SI4844_01_OLED --output-dir ~/Downloads/hex/minicore/SI4844_01_OLED --warnings all
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto ./SI4844_01_OLED_V2 --output-dir ~/Downloads/hex/minicore/SI4844_01_OLED_V2 --warnings all
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto ./SI4844_02_LCD16x02 --output-dir ~/Downloads/hex/minicore/SI4844_02_LCD16x02  --warnings all
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto ./SI4844_03_TM1638_7SEG_DISPLAY --output-dir ~/Downloads/hex/minicore/7SEG  --warnings all
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto  ./SLIDE_SWITCH/SI4844_01_OLED_SLIDE_SWITCH  --output-dir ~/Downloads/hex/minicore/SLIDE_SWITCH_OLED  --warnings all
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto  ./SLIDE_SWITCH/SI4844_02_LCD16x2_SLIDE_SWITCH  --output-dir ~/Downloads/hex/minicore/SLIDE_SWITCH_LCD16x2  --warnings all
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto ./SLIDE_SWITCH/SI4844_03_TM1637_CUSTM_BAND_SWITCH  --output-dir ~/Downloads/hex/minicore/SI4844_03_TM1637_CUSTM_BAND_SWITCH  
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto ./SLIDE_SWITCH/SI4844_03_TM1637_SLIDE_SWITCH  --output-dir ~/Downloads/hex/minicore/SI4844_03_TM1637_SLIDE_SWITCH  



echo "********************"
echo "Arduino LGT8FX based board"
echo "********************"
arduino-cli compile -b lgt8fx:avr:328  ./SI4844_00_MINIMAL --output-dir ~/Downloads/hex/lgt/SI4844_00_MINIMAL --warnings all
arduino-cli compile -b lgt8fx:avr:328 ./SI4844_00_POC --output-dir ~/Downloads/hex/lgt/SI4844_00_POC  --warnings all
arduino-cli compile -b lgt8fx:avr:328 ./SI4844_01_OLED --output-dir ~/Downloads/hex/lgt/SI4844_01_OLED --warnings all
arduino-cli compile -b lgt8fx:avr:328 ./SI4844_02_LCD16x02 --output-dir ~/Downloads/hex/atmega/SI4844_02_LCD16x02  --warnings all
arduino-cli compile -b lgt8fx:avr:328 ./SI4844_02_LCD16x02 --output-dir ~/Downloads/hex/lgt/SI4844_02_LCD16x02  --warnings all
arduino-cli compile -b lgt8fx:avr:328 ./SI4844_03_TM1638_7SEG_DISPLAY --output-dir ~/Downloads/hex/lgt/7SEG  --warnings all
arduino-cli compile -b lgt8fx:avr:328 ./SI4844_04_NOKIA5110 --output-dir ~/Downloads/hex/atmega/NOKIA5110 
arduino-cli compile -b lgt8fx:avr:328 ./TEST/SI4844_BASS_TREBLE --output-dir ~/Downloads/hex/lgt/SI4844_BASS_TREBLE  --warnings all
arduino-cli compile -b lgt8fx:avr:328 ./TEST/SI4844_FIRMWARE --output-dir ~/Downloads/hex/lgt/SI4844_FIRMWARE  --warnings all
arduino-cli compile -b lgt8fx:avr:328 ./SLIDE_SWITCH/SI4844_01_OLED_SLIDE_SWITCH  --output-dir ~/Downloads/hex/lgt/SLIDE_SWITCH_OLED  
arduino-cli compile -b lgt8fx:avr:328 ./SLIDE_SWITCH/SI4844_02_LCD16x2_SLIDE_SWITCH  --output-dir ~/Downloads/hex/lgt/SLIDE_SWITCH_LCD16x2  --warnings all
arduino-cli compile -b lgt8fx:avr:328  ./SLIDE_SWITCH/SI4844_03_TM1637_CUSTM_BAND_SWITCH  --output-dir ~/Downloads/hex/lgt/SI4844_03_TM1637_CUSTM_BAND_SWITCH  
arduino-cli compile -b lgt8fx:avr:328 ./SLIDE_SWITCH/SI4844_03_TM1637_SLIDE_SWITCH  --output-dir ~/Downloads/hex/lgt/SI4844_03_TM1637_SLIDE_SWITCH  





echo "********************"
echo "ESP32"
echo "********************"
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso  ./SI4844_ESP32_POC --output-dir ~/Downloads/hex/esp32/SI4844_ESP32_POC  --warnings all

echo "********************"
echo "ESP8266"
echo "********************"
arduino-cli compile --fqbn esp8266:esp8266:generic  ./SI4844_ESP8266_POC --output-dir ~/Downloads/hex/esp8266/SI4844_ESP8266_POC  --warnings all

echo "********************"
echo "STM32"
echo "********************"
arduino-cli compile --fqbn STMicroelectronics:stm32:GenF1:pnum=BLUEPILL_F103C8 ./SI4844_STM32_POC --output-dir ~/Downloads/hex/STM32/SI4844_STM32_POC  --warnings all


echo "********************"
echo "SEEEDUINO"
echo "********************"
arduino-cli compile --fqbn Seeeduino:samd:seeed_XIAO_m0 ./SI4844_SEEDUINO_OLED --output-dir ~/Downloads/hex/SEEEDUINO/SI4844_SEEDUINO_OLED  #--warnings all