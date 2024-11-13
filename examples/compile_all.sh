# This script uses the arduino-cli to compile the arduino sketches using command line (without Arduino IDE).
# It is very useful to check the library on multiple board types after bug fixes and improvements.
# It runs on a MacOS but you can port it to Linux or Windows easily.
# Some compilation results (bin and hex files) will be stores in your Download directory (/Users/<username>/Downloads/hex)
# ATTENTION: 1) Be careful with --output-dir arduino-cli option. Before compiling, this option removes all the contents of the last level folder. 
#               For example: if you use "--output-dir ~/Downloads", all the current content of the Downloads folder will be lost. 
#                         if you use "--output-dir ~/Downloads/hex", all current content of the hex folder will be lost and the Downloads 
#                         content will be preserved. 
#            2) I have enabled all the compile warnings (--warnings all) to check some questionable situations that can be avoided or modified to prevent future warnings.  
#            3) I have enabled  the "--verbose" parameter to show the deteiled logs of the compiling process.
# Please, see the file config_libraries_and_boards.sh
# Ricardo Lima Caratti Mar 2023

# compiles POC
echo "********************"
echo "Arduino ATmega328 based board"
echo "********************"
arduino-cli compile -b arduino:avr:nano ./SI4844_00_MINIMAL --output-dir ~/Downloads/hex/atmega/SI4844_00_MINIMAL 
arduino-cli compile -b arduino:avr:nano ./SI4844_00_POC --output-dir ~/Downloads/hex/atmega/SI4844_00_POC  
arduino-cli compile -b arduino:avr:nano ./SI4844_01_OLED --output-dir ~/Downloads/hex/atmega/SI4844_01_OLED 
arduino-cli compile -b arduino:avr:nano ./SI4844_02_LCD16x02 --output-dir ~/Downloads/hex/atmega/SI4844_02_LCD16x02  
arduino-cli compile -b arduino:avr:nano ./TEST/SI4844_BASS_TREBLE --output-dir ~/Downloads/hex/atmega/SI4844_BASS_TREBLE  
arduino-cli compile -b arduino:avr:nano ./TEST/SI4844_CUSTOM_BAND --output-dir ~/Downloads/hex/atmega/SI4844_CUSTOM_BAND  
arduino-cli compile -b arduino:avr:nano ./TEST/SI4844_FIRMWARE --output-dir ~/Downloads/hex/atmega/SI4844_FIRMWARE  




echo "********************"
echo "Arduino LGT8FX based board"
echo "********************"

arduino-cli compile -b lgt8fx:avr:328  ./SI4844_00_MINIMAL --output-dir ~/Downloads/hex/ltg/SI4844_00_MINIMAL 
arduino-cli compile -b lgt8fx:avr:328 ./SI4844_00_POC --output-dir ~/Downloads/hex/ltg/SI4844_00_POC  
arduino-cli compile -b lgt8fx:avr:328 ./SI4844_01_OLED --output-dir ~/Downloads/hex/ltg/SI4844_01_OLED 
arduino-cli compile -b lgt8fx:avr:328 ./SI4844_02_LCD16x02 --output-dir ~/Downloads/hex/ltg/SI4844_02_LCD16x02  
arduino-cli compile -b lgt8fx:avr:328 ./TEST/SI4844_BASS_TREBLE --output-dir ~/Downloads/hex/ltg/SI4844_BASS_TREBLE  
arduino-cli compile -b lgt8fx:avr:328 ./TEST/SI4844_CUSTOM_BAND --output-dir ~/Downloads/hex/ltg/SI4844_CUSTOM_BAND  
arduino-cli compile -b lgt8fx:avr:328 ./TEST/SI4844_FIRMWARE --output-dir ~/Downloads/hex/ltg/SI4844_FIRMWARE  
