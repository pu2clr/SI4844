# Windows Setup Scripts for PU2CLR SI4844 Arduino Library

This directory contains Windows-compatible setup scripts for installing Arduino CLI and all required libraries/boards for the PU2CLR SI4844 Arduino Library.

## Available Scripts

### 1. `config_libraries_and_boards.bat` (Main Installation Script)
- **Compatibility**: Windows 7, 8, 10, 11
- **Requirements**: PowerShell (usually pre-installed)
- **Execution**: Double-click or run from Command Prompt
- **Features**: Complete installation with error checking and diagnostics

### 2. `config_libraries_and_boards_simple.bat` (Simple Installation)
- **Use case**: When the main script fails or Arduino CLI is already installed
- **Requirements**: Arduino CLI must be manually installed first
- **Features**: Installs only essential libraries and boards

### 3. `config_libraries_and_boards.ps1` (PowerShell Script)
- **Compatibility**: Windows 10, 11 (PowerShell 5.1+)
- **Requirements**: PowerShell with execution policy allowing scripts
- **Features**: Advanced options and better error handling

### 4. `diagnose_setup.bat` (Diagnostic Tool)
- **Purpose**: Troubleshoot installation problems
- **Use**: Run this first if you encounter issues
- **Features**: Comprehensive system and network checks

## Quick Start

### Having Problems? Start Here!
1. **First, run the diagnostic tool**: `diagnose_setup.bat`
2. **Review the diagnostic results** and follow suggested solutions
3. **Then proceed with installation**

### Option 1: Main Installation Script (Recommended)
1. Right-click on `config_libraries_and_boards.bat`
2. Select "Run as administrator" (recommended)
3. Follow the on-screen instructions
4. If it fails, check the diagnostic tool results

### Option 2: Simple Installation (If main script fails)
1. **First, manually install Arduino CLI**:
   - Download from: https://arduino.cc/en/software
   - Extract `arduino-cli.exe` to `%USERPROFILE%\bin\`
   - Add `%USERPROFILE%\bin` to your PATH
2. **Then run**: `config_libraries_and_boards_simple.bat`

### Option 3: PowerShell Script (Advanced users)
1. Open PowerShell as Administrator
2. Navigate to the script directory:
   ```powershell
   cd "path\to\SI4844\examples"
   ```
3. Set execution policy (if needed):
   ```powershell
   Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
   ```
4. Run the script:
   ```powershell
   .\config_libraries_and_boards.ps1
   ```

## What These Scripts Do

Both scripts perform the following actions:

1. **Download and Install Arduino CLI**
   - Downloads the latest Arduino CLI for Windows
   - Installs it to `%USERPROFILE%\bin`
   - Adds it to the current session PATH

2. **Configure Arduino CLI**
   - Initializes Arduino CLI configuration
   - Adds board manager URLs for various microcontrollers:
     - ESP32/ESP8266
     - STM32
     - ATtiny
     - LGT8F328
     - And many more...

3. **Install Required Libraries**
   - PU2CLR SI4844 (main library)
   - Display libraries (SSD1306, Nokia 5110, etc.)
   - Input libraries (TM1637, TM1638, etc.)
   - Standard libraries (LiquidCrystal, etc.)

4. **Install Supported Boards**
   - Arduino AVR boards
   - ESP32/ESP8266 boards
   - STM32 boards
   - ATtiny boards
   - LGT8F328 boards
   - And more...

## PowerShell Script Options

The PowerShell script supports additional options:

```powershell
# Skip board installation (only install libraries)
.\config_libraries_and_boards.ps1 -SkipBoardInstall

# Skip library installation (only install boards)
.\config_libraries_and_boards.ps1 -SkipLibraryInstall

# Show help
.\config_libraries_and_boards.ps1 -Help
```

## Troubleshooting

### Step 1: Run Diagnostic Tool
Always start with: `diagnose_setup.bat`

### Common Issues and Solutions

#### 1. Download Failures
**Symptoms**: "Failed to download Arduino CLI"
**Solutions**:
- Run `diagnose_setup.bat` to check internet connectivity
- Temporarily disable antivirus/firewall
- Try different network (mobile hotspot)
- Use manual installation method

#### 2. PowerShell Execution Policy Error
**Symptoms**: "Cannot be loaded because running scripts is disabled"
**Solutions**:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

#### 3. Arduino CLI Not Found After Installation
**Symptoms**: "'arduino-cli' is not recognized"
**Solutions**:
- Restart Command Prompt/PowerShell
- Manually add to PATH:
  1. Win + R, type `sysdm.cpl`
  2. Advanced → Environment Variables
  3. User variables → Path → Edit → New
  4. Add: `%USERPROFILE%\bin`
- Use the simple installation script instead

#### 4. Permission Denied Errors
**Symptoms**: "Access denied" or "Permission denied"
**Solutions**:
- Run as Administrator
- Check antivirus real-time protection
- Ensure user has write permissions to home directory

#### 5. Library Installation Failures
**Symptoms**: "Error installing library"
**Solutions**:
- Check internet connection stability
- Try installing libraries individually
- Use Arduino IDE Library Manager as backup

## Manual Installation Alternative

If the scripts fail, you can install manually:

1. **Download Arduino CLI manually**:
   - Go to: https://arduino.cc/en/software
   - Download "Arduino CLI" for Windows

2. **Extract and place in PATH**:
   - Extract `arduino-cli.exe` to a folder in your PATH
   - Or create `%USERPROFILE%\bin` and add to PATH

3. **Run commands manually**:
   ```cmd
   arduino-cli config init
   arduino-cli lib install "PU2CLR SI4844"
   arduino-cli core install arduino:avr
   ```

## Supported Platforms

These scripts configure support for:

- **Arduino**: Uno, Nano, Pro Mini, Mega
- **ESP32**: All variants
- **ESP8266**: NodeMCU, Wemos D1, etc.
- **STM32**: STM32F1, STM32F4 families
- **ATtiny**: ATtiny85, ATtiny84, etc.
- **LGT8F328**: Arduino Nano compatible with 3.3V
- **Seeeduino**: SAMD boards
- **Teensy**: Various models

## Files Created/Modified

After running the scripts, you'll have:

- `%USERPROFILE%\bin\arduino-cli.exe` - Arduino CLI executable
- `%USERPROFILE%\.arduino15\` - Arduino CLI configuration and libraries
- Updated system PATH (after manual configuration)

## Getting Help

If you encounter issues:

1. Check the official Arduino CLI documentation: https://arduino.github.io/arduino-cli/
2. Review the PU2CLR SI4844 library documentation: https://github.com/pu2clr/SI4844
3. Open an issue on the project repository

## License

These scripts are part of the PU2CLR SI4844 Arduino Library project and follow the same MIT license.
