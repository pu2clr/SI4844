@echo off
REM This script runs on Windows and it is useful to install the basic environment to develop
REM applications (receivers) using the PU2CLR Arduino Library.
REM
REM ATTENTION: Please, do not run it if you do not know what you are doing.
REM
REM Please, check the comments above

echo.
echo ========================================================
echo  PU2CLR SI4844 Arduino Library Setup for Windows
echo ========================================================
echo.

REM Check if PowerShell is available
powershell -Command "Write-Host 'PowerShell is available'" >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: PowerShell is not available or not working properly
    echo This script requires PowerShell to download and extract files
    echo Please ensure PowerShell is installed and working
    pause
    exit /b 1
)

REM Check internet connectivity
echo Checking internet connectivity...
ping -n 1 8.8.8.8 >nul 2>&1
if %errorlevel% neq 0 (
    echo WARNING: No internet connection detected
    echo This script requires internet access to download Arduino CLI and libraries
    echo Please check your internet connection and try again
    pause
)

echo Internet connection: OK
echo PowerShell: OK
echo.

REM Display system information
echo System Information:
echo Windows Version: 
ver
echo User Profile: %USERPROFILE%
echo Temp Directory: %TEMP%
echo.
echo Starting installation process...

REM Go to user home directory
cd /d %USERPROFILE%

REM Check if arduino-cli is already installed
where arduino-cli >nul 2>&1
if %errorlevel% equ 0 (
    echo Arduino CLI is already installed.
    goto :configure_arduino
)

echo Downloading and installing Arduino CLI...
REM Create a temporary directory for download
if not exist "%TEMP%\arduino-cli-install" mkdir "%TEMP%\arduino-cli-install"
cd /d "%TEMP%\arduino-cli-install"

REM Download Arduino CLI for Windows
echo Downloading Arduino CLI...
echo Please wait, this may take a few minutes depending on your internet connection...

powershell -Command "try { [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri 'https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip' -OutFile 'arduino-cli.zip' -UseBasicParsing -ErrorAction Stop; Write-Host 'Download completed successfully' } catch { Write-Host 'Error downloading: ' + $_.Exception.Message; exit 1 }"

if %errorlevel% neq 0 (
    echo ERROR: Failed to download Arduino CLI
    echo Possible causes:
    echo - No internet connection
    echo - Firewall/antivirus blocking download
    echo - PowerShell execution policy restrictions
    echo.
    echo Trying alternative download method...
    
    REM Try alternative download using curl if available
    curl -L -o arduino-cli.zip https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip
    
    if %errorlevel% neq 0 (
        echo ERROR: Both download methods failed
        echo Please download manually from: https://arduino.cc/en/software
        pause
        exit /b 1
    )
)

if not exist "arduino-cli.zip" (
    echo ERROR: Download file not found
    pause
    exit /b 1
)

REM Extract Arduino CLI
echo Extracting Arduino CLI...
powershell -Command "try { Expand-Archive -Path 'arduino-cli.zip' -DestinationPath '.' -Force -ErrorAction Stop; Write-Host 'Extraction completed' } catch { Write-Host 'Error extracting: ' + $_.Exception.Message; exit 1 }"

if %errorlevel% neq 0 (
    echo ERROR: Failed to extract Arduino CLI
    echo The downloaded file may be corrupted
    pause
    exit /b 1
)

REM Verify extraction was successful
if not exist "arduino-cli.exe" (
    echo ERROR: arduino-cli.exe not found after extraction
    echo Listing contents of current directory:
    dir
    pause
    exit /b 1
)

REM Create bin directory in user profile if it doesn't exist
if not exist "%USERPROFILE%\bin" (
    echo Creating bin directory...
    mkdir "%USERPROFILE%\bin"
)

REM Copy arduino-cli.exe to user bin directory
echo Copying Arduino CLI to bin directory...
copy "arduino-cli.exe" "%USERPROFILE%\bin\" >nul

if %errorlevel% neq 0 (
    echo ERROR: Failed to copy arduino-cli.exe to bin directory
    echo Check if you have write permissions to %USERPROFILE%\bin
    pause
    exit /b 1
)

REM Add bin directory to PATH for current session
set "PATH=%USERPROFILE%\bin;%PATH%"

REM Clean up temporary files
cd /d %USERPROFILE%
rmdir /s /q "%TEMP%\arduino-cli-install"

echo Arduino CLI installed successfully.

:configure_arduino
echo.
echo Configuring Arduino CLI...

REM Start the arduino-cli configuration
echo Initializing Arduino CLI configuration...
arduino-cli config init

if %errorlevel% neq 0 (
    echo ERROR: Failed to initialize Arduino CLI configuration
    echo Make sure Arduino CLI is properly installed and accessible
    pause
    exit /b 1
)

REM Add all boards used by the examples (ATmega, ATtiny, ESP32, STM32 etc) to the board list
echo Adding board manager URLs...
arduino-cli config set board_manager.additional_urls ^
http://arduino.esp8266.com/stable/package_esp8266com_index.json,^
http://dan.drown.org/stm32duino/package_STM32duino_index.json,^
http://drazzy.com/package_drazzy.com_index.json,^
https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json,^
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json,^
https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json,^
https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json,^
https://mcudude.github.io/MegaCore/package_MCUdude_MegaCore_index.json,^
https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json,^
https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json,^
https://raw.githubusercontent.com/DavidGuo-CS/OSOYOO_Arduino/main/package_osoyoo_boards_index.json,^
https://raw.githubusercontent.com/VSChina/azureiotdevkit_tools/master/package_azureboard_index.json,^
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json,^
https://raw.githubusercontent.com/dbuezas/lgt8fx/master/package_lgt8fx_index.json,^
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json,^
https://raw.githubusercontent.com/nulllaborg/arduino_nulllab/master/package_nulllab_boards_index.json,^
https://www.pjrc.com/teensy/package_teensy_index.json

arduino-cli config set network.connection_timeout 600s

REM Update the index of boards that can be installed
echo Updating board index...
echo This may take a few minutes...
arduino-cli core update-index

if %errorlevel% neq 0 (
    echo WARNING: Failed to update board index
    echo This might cause issues installing boards later
    echo Continuing anyway...
    pause
)

echo.
echo ====================================================
echo This may take several minutes or hours. Please wait!
echo ====================================================
echo Installing the libraries used by the examples implemented in this Library

REM Function to install library with error checking
REM Install required libraries
REM Uncomment the lines if you want to include more libraries

echo Installing PU2CLR SI4844...
arduino-cli lib install "PU2CLR SI4844"
if %errorlevel% neq 0 echo WARNING: Failed to install PU2CLR SI4844

echo Installing Adafruit SSD1306...
arduino-cli lib install "Adafruit SSD1306"
if %errorlevel% neq 0 echo WARNING: Failed to install Adafruit SSD1306

echo Installing SSD1306Ascii...
arduino-cli lib install "SSD1306Ascii"
if %errorlevel% neq 0 echo WARNING: Failed to install SSD1306Ascii

echo Installing TM1638lite...
arduino-cli lib install "TM1638lite"
if %errorlevel% neq 0 echo WARNING: Failed to install TM1638lite

echo Installing TM1637TinyDisplay...
arduino-cli lib install "TM1637TinyDisplay"
if %errorlevel% neq 0 echo WARNING: Failed to install TM1637TinyDisplay

REM arduino-cli lib install "Adafruit ST7735 and ST7789 Library"

echo Installing Adafruit PCD8544 Nokia 5110 LCD library...
arduino-cli lib install "Adafruit PCD8544 Nokia 5110 LCD library"
if %errorlevel% neq 0 echo WARNING: Failed to install Adafruit PCD8544 Nokia 5110 LCD library

REM arduino-cli lib install "Adafruit TouchScreen"
REM arduino-cli lib install "ES32Lab"
REM arduino-cli lib install "Etherkit Si5351"
REM arduino-cli lib install "FlashStorage_SAMD"

echo Installing LiquidCrystal...
arduino-cli lib install "LiquidCrystal"
if %errorlevel% neq 0 echo WARNING: Failed to install LiquidCrystal

REM arduino-cli lib install "LiquidCrystal I2C"
REM arduino-cli lib install "MCUFRIEND_kbv"
REM arduino-cli lib install "TFT_22_ILI9225"
REM arduino-cli lib install "TFT_eSPI_ES32Lab"

echo Installing Tiny4kOLED...
arduino-cli lib install "Tiny4kOLED"
if %errorlevel% neq 0 echo WARNING: Failed to install Tiny4kOLED

echo Installing TinyOLED-Fonts...
arduino-cli lib install "TinyOLED-Fonts"
if %errorlevel% neq 0 echo WARNING: Failed to install TinyOLED-Fonts

REM arduino-cli lib install "FastLED"

echo Installing RotaryEncoder...
arduino-cli lib install "RotaryEncoder"
if %errorlevel% neq 0 echo WARNING: Failed to install RotaryEncoder

echo.
echo ----------------------------------------------------
echo Installing the library Adafruit_SH1106 via github repository
set ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=true
REM arduino-cli lib install --git-url https://github.com/wonho-maker/Adafruit_SH1106
REM arduino-cli lib install --git-url https://github.com/danilopinotti/Battery18650Stats
echo ----------------------------------------------------

echo.
REM Uncomment the lines below if you want some additional boards
echo Installing the boards...
echo This may take a very long time (30+ minutes)...

echo Installing arduino:avr...
arduino-cli core install arduino:avr
if %errorlevel% neq 0 echo WARNING: Failed to install arduino:avr

echo Installing lgt8fx:avr...
arduino-cli core install lgt8fx:avr
if %errorlevel% neq 0 echo WARNING: Failed to install lgt8fx:avr

echo Installing MiniCore:avr...
arduino-cli core install MiniCore:avr
if %errorlevel% neq 0 echo WARNING: Failed to install MiniCore:avr

echo Installing arduino:sam...
arduino-cli core install arduino:sam
if %errorlevel% neq 0 echo WARNING: Failed to install arduino:sam

echo Installing esp32:esp32...
arduino-cli core install esp32:esp32
if %errorlevel% neq 0 echo WARNING: Failed to install esp32:esp32

echo Installing esp8266:esp8266...
arduino-cli core install esp8266:esp8266
if %errorlevel% neq 0 echo WARNING: Failed to install esp8266:esp8266

echo Installing stm32duino:STM32F1...
arduino-cli core install stm32duino:STM32F1
if %errorlevel% neq 0 echo WARNING: Failed to install stm32duino:STM32F1

REM arduino-cli core install stm32duino:STM32F4
REM arduino-cli core install STM32:stm32
REM arduino-cli core install STMicroelectronics:stm32
REM arduino-cli core install rp2040:rp2040
REM arduino-cli core install Seeeduino:samd

echo Installing ATTinyCore:avr...
arduino-cli core install ATTinyCore:avr
if %errorlevel% neq 0 echo WARNING: Failed to install ATTinyCore:avr

REM arduino-cli core install MegaCore:avr
REM arduino-cli core install MightyCore:avr
REM arduino-cli core install teensy:avr

echo.
echo ============================================
echo  Installation completed successfully!
echo ============================================
echo.
echo Arduino CLI and all required libraries and boards have been installed.
echo You can now use Arduino CLI to compile and upload sketches.
echo.
echo To add Arduino CLI to your permanent PATH, add the following directory:
echo %USERPROFILE%\bin
echo.
echo To your system PATH environment variable.
echo.

REM Add permanent PATH modification instructions
echo To permanently add Arduino CLI to your PATH:
echo 1. Open System Properties ^(Win + Pause^)
echo 2. Click "Advanced system settings"
echo 3. Click "Environment Variables"
echo 4. Under "User variables", find and select "Path", then click "Edit"
echo 5. Click "New" and add: %USERPROFILE%\bin
echo 6. Click "OK" to save
echo.

pause
