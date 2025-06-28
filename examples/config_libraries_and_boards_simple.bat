@echo off
REM Simplified version of config_libraries_and_boards.bat
REM Use this if the main script fails
REM This script assumes you have manually installed Arduino CLI

echo.
echo ========================================================
echo  PU2CLR SI4844 - Simple Library Installation Script
echo ========================================================
echo.

REM Check if Arduino CLI is available
arduino-cli version >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Arduino CLI not found in PATH
    echo.
    echo Please install Arduino CLI manually:
    echo 1. Download from: https://arduino.cc/en/software
    echo 2. Extract arduino-cli.exe to a folder in your PATH
    echo 3. Or add the folder containing arduino-cli.exe to your PATH
    echo.
    echo Alternative: Run the main config_libraries_and_boards.bat script
    pause
    exit /b 1
)

echo Arduino CLI found. Version:
arduino-cli version

echo.
echo Configuring Arduino CLI...

REM Initialize configuration
arduino-cli config init

REM Set timeout
arduino-cli config set network.connection_timeout 600s

echo.
echo Adding board manager URLs...

REM Add essential board URLs one by one
arduino-cli config add board_manager.additional_urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/dbuezas/lgt8fx/master/package_lgt8fx_index.json
arduino-cli config add board_manager.additional_urls https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json

echo.
echo Updating board index...
arduino-cli core update-index

echo.
echo Installing essential libraries...

REM Install only the most essential libraries
echo Installing PU2CLR SI4844 (main library)...
arduino-cli lib install "PU2CLR SI4844"

echo Installing Adafruit SSD1306...
arduino-cli lib install "Adafruit SSD1306"

echo Installing LiquidCrystal...
arduino-cli lib install "LiquidCrystal"

echo.
echo Installing essential boards...

echo Installing Arduino AVR boards...
arduino-cli core install arduino:avr

echo Installing ESP32 boards...
arduino-cli core install esp32:esp32

echo Installing ESP8266 boards...
arduino-cli core install esp8266:esp8266

echo.
echo ========================================
echo  Basic installation completed!
echo ========================================
echo.
echo The following have been installed:
echo - PU2CLR SI4844 library
echo - Basic display libraries
echo - Arduino AVR, ESP32, ESP8266 boards
echo.
echo You can install additional libraries manually using:
echo arduino-cli lib install "LibraryName"
echo.
echo You can install additional boards manually using:
echo arduino-cli core install BoardPackage:BoardArchitecture
echo.

pause
