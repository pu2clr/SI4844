# PowerShell script to install Arduino CLI and required libraries for PU2CLR SI4844
# This script runs on Windows and it is useful to install the basic environment to develop
# applications (receivers) using the PU2CLR Arduino Library.
#
# ATTENTION: Please, do not run it if you do not know what you are doing.
#
# Please, check the comments above

param(
    [switch]$SkipBoardInstall,
    [switch]$SkipLibraryInstall,
    [switch]$Help
)

function Show-Help {
    Write-Host @"
PU2CLR SI4844 Arduino Library Setup for Windows (PowerShell)

Usage: .\config_libraries_and_boards.ps1 [OPTIONS]

Options:
  -SkipBoardInstall     Skip board installation
  -SkipLibraryInstall   Skip library installation
  -Help                 Show this help message

Examples:
  .\config_libraries_and_boards.ps1
  .\config_libraries_and_boards.ps1 -SkipBoardInstall
  .\config_libraries_and_boards.ps1 -SkipLibraryInstall

"@ -ForegroundColor Green
}

if ($Help) {
    Show-Help
    exit 0
}

Write-Host "========================================================" -ForegroundColor Cyan
Write-Host " PU2CLR SI4844 Arduino Library Setup for Windows" -ForegroundColor Cyan
Write-Host " PowerShell Version" -ForegroundColor Cyan
Write-Host "========================================================" -ForegroundColor Cyan
Write-Host ""

# Check if running as Administrator (recommended but not required)
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Warning "Running without administrator privileges. Some operations might fail."
    Write-Host "Consider running as Administrator for better compatibility." -ForegroundColor Yellow
    Write-Host ""
}

# Set error handling
$ErrorActionPreference = "Stop"

try {
    # Go to user home directory
    Set-Location $env:USERPROFILE

    # Check if arduino-cli is already installed
    $arduinoCliExists = Get-Command arduino-cli -ErrorAction SilentlyContinue
    if ($arduinoCliExists) {
        Write-Host "Arduino CLI is already installed." -ForegroundColor Green
    }
    else {
        Write-Host "Installing Arduino CLI..." -ForegroundColor Yellow
        
        # Create bin directory if it doesn't exist
        $binDir = Join-Path $env:USERPROFILE "bin"
        if (-not (Test-Path $binDir)) {
            New-Item -ItemType Directory -Path $binDir -Force | Out-Null
        }

        # Download and install Arduino CLI
        $tempDir = Join-Path $env:TEMP "arduino-cli-install"
        if (Test-Path $tempDir) {
            Remove-Item $tempDir -Recurse -Force
        }
        New-Item -ItemType Directory -Path $tempDir -Force | Out-Null
        
        Set-Location $tempDir
        
        Write-Host "Downloading Arduino CLI..." -ForegroundColor Yellow
        $downloadUrl = "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip"
        $zipFile = "arduino-cli.zip"
        
        Invoke-WebRequest -Uri $downloadUrl -OutFile $zipFile -UseBasicParsing
        
        Write-Host "Extracting Arduino CLI..." -ForegroundColor Yellow
        Expand-Archive -Path $zipFile -DestinationPath . -Force
        
        # Copy to bin directory
        Copy-Item "arduino-cli.exe" $binDir -Force
        
        # Add to PATH for current session
        $env:PATH = "$binDir;$env:PATH"
        
        # Clean up
        Set-Location $env:USERPROFILE
        Remove-Item $tempDir -Recurse -Force
        
        Write-Host "Arduino CLI installed successfully." -ForegroundColor Green
    }

    Write-Host ""
    Write-Host "Configuring Arduino CLI..." -ForegroundColor Yellow

    # Initialize Arduino CLI configuration
    arduino-cli config init

    # Board manager URLs
    $boardUrls = @(
        "http://arduino.esp8266.com/stable/package_esp8266com_index.json",
        "http://dan.drown.org/stm32duino/package_STM32duino_index.json",
        "http://drazzy.com/package_drazzy.com_index.json",
        "https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json",
        "https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json",
        "https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json",
        "https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json",
        "https://mcudude.github.io/MegaCore/package_MCUdude_MegaCore_index.json",
        "https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json",
        "https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json",
        "https://raw.githubusercontent.com/DavidGuo-CS/OSOYOO_Arduino/main/package_osoyoo_boards_index.json",
        "https://raw.githubusercontent.com/VSChina/azureiotdevkit_tools/master/package_azureboard_index.json",
        "https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json",
        "https://raw.githubusercontent.com/dbuezas/lgt8fx/master/package_lgt8fx_index.json",
        "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json",
        "https://raw.githubusercontent.com/nulllaborg/arduino_nulllab/master/package_nulllab_boards_index.json",
        "https://www.pjrc.com/teensy/package_teensy_index.json"
    )

    Write-Host "Adding board manager URLs..." -ForegroundColor Yellow
    $urlsString = $boardUrls -join ","
    arduino-cli config set board_manager.additional_urls $urlsString

    arduino-cli config set network.connection_timeout 600s

    Write-Host "Updating board index..." -ForegroundColor Yellow
    arduino-cli core update-index

    if (-not $SkipLibraryInstall) {
        Write-Host ""
        Write-Host "====================================================" -ForegroundColor Cyan
        Write-Host "Installing libraries..." -ForegroundColor Cyan
        Write-Host "This may take several minutes. Please wait!" -ForegroundColor Yellow
        Write-Host "====================================================" -ForegroundColor Cyan

        # Libraries to install
        $libraries = @(
            "PU2CLR SI4844",
            "Adafruit SSD1306",
            "SSD1306Ascii",
            "TM1638lite",
            "TM1637TinyDisplay",
            "Adafruit PCD8544 Nokia 5110 LCD library",
            "LiquidCrystal",
            "Tiny4kOLED",
            "TinyOLED-Fonts",
            "RotaryEncoder"
        )

        foreach ($library in $libraries) {
            Write-Host "Installing $library..." -ForegroundColor Yellow
            arduino-cli lib install "$library"
        }

        Write-Host ""
        Write-Host "----------------------------------------------------" -ForegroundColor Cyan
        Write-Host "Installing optional libraries via GitHub repository" -ForegroundColor Yellow
        $env:ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL = "true"
        # Uncomment the lines below if you want to install from GitHub
        # arduino-cli lib install --git-url https://github.com/wonho-maker/Adafruit_SH1106
        # arduino-cli lib install --git-url https://github.com/danilopinotti/Battery18650Stats
        Write-Host "----------------------------------------------------" -ForegroundColor Cyan
    }

    if (-not $SkipBoardInstall) {
        Write-Host ""
        Write-Host "Installing boards..." -ForegroundColor Cyan

        # Boards to install
        $boards = @(
            "arduino:avr",
            "lgt8fx:avr",
            "MiniCore:avr",
            "arduino:sam",
            "esp32:esp32",
            "esp8266:esp8266",
            "stm32duino:STM32F1",
            "ATTinyCore:avr"
        )

        foreach ($board in $boards) {
            Write-Host "Installing $board..." -ForegroundColor Yellow
            arduino-cli core install $board
        }
    }

    Write-Host ""
    Write-Host "============================================" -ForegroundColor Green
    Write-Host " Installation completed successfully!" -ForegroundColor Green
    Write-Host "============================================" -ForegroundColor Green
    Write-Host ""
    
    Write-Host "Arduino CLI and all required libraries and boards have been installed." -ForegroundColor Green
    Write-Host "You can now use Arduino CLI to compile and upload sketches." -ForegroundColor Green
    Write-Host ""
    
    # Check if bin directory is in PATH
    $binDir = Join-Path $env:USERPROFILE "bin"
    $pathEnv = [Environment]::GetEnvironmentVariable("Path", "User")
    if ($pathEnv -notlike "*$binDir*") {
        Write-Host "To permanently add Arduino CLI to your PATH:" -ForegroundColor Yellow
        Write-Host "1. Open System Properties (Win + Pause)" -ForegroundColor White
        Write-Host "2. Click 'Advanced system settings'" -ForegroundColor White
        Write-Host "3. Click 'Environment Variables'" -ForegroundColor White
        Write-Host "4. Under 'User variables', find and select 'Path', then click 'Edit'" -ForegroundColor White
        Write-Host "5. Click 'New' and add: $binDir" -ForegroundColor White
        Write-Host "6. Click 'OK' to save" -ForegroundColor White
        Write-Host ""
        Write-Host "Or run this command as Administrator:" -ForegroundColor Yellow
        Write-Host "[Environment]::SetEnvironmentVariable('Path', [Environment]::GetEnvironmentVariable('Path', 'User') + ';$binDir', 'User')" -ForegroundColor Cyan
    }
    else {
        Write-Host "Arduino CLI is already in your PATH." -ForegroundColor Green
    }

} catch {
    Write-Error "An error occurred: $($_.Exception.Message)"
    Write-Host "Please check your internet connection and try again." -ForegroundColor Red
    exit 1
}
