@echo off
REM Diagnostic script for Arduino CLI setup issues
REM Run this script to diagnose problems with the main installation script

echo.
echo ========================================================
echo  Arduino CLI Setup Diagnostic Tool
echo ========================================================
echo.

echo Running diagnostic checks...
echo.

echo [1/10] Checking Windows version...
ver
echo.

echo [2/10] Checking PowerShell availability...
powershell -Command "Write-Host 'PowerShell version:'; $PSVersionTable.PSVersion" 2>nul
if %errorlevel% neq 0 (
    echo ERROR: PowerShell not working properly
    echo This is required for downloading files
) else (
    echo PowerShell: OK
)
echo.

echo [3/10] Checking internet connectivity...
ping -n 1 8.8.8.8 >nul 2>&1
if %errorlevel% neq 0 (
    echo WARNING: Cannot reach 8.8.8.8
    echo Trying alternative...
    ping -n 1 1.1.1.1 >nul 2>&1
    if %errorlevel% neq 0 (
        echo ERROR: No internet connectivity detected
    ) else (
        echo Internet: OK (via 1.1.1.1)
    )
) else (
    echo Internet: OK (via 8.8.8.8)
)
echo.

echo [4/10] Testing HTTPS download capability...
powershell -Command "try { [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri 'https://httpbin.org/get' -UseBasicParsing -TimeoutSec 10 | Out-Null; Write-Host 'HTTPS downloads: OK' } catch { Write-Host 'ERROR: HTTPS download failed - ' + $_.Exception.Message }" 2>nul
echo.

echo [5/10] Checking Arduino CLI in PATH...
where arduino-cli >nul 2>&1
if %errorlevel% equ 0 (
    echo Arduino CLI found in PATH:
    where arduino-cli
    echo Version:
    arduino-cli version 2>nul
) else (
    echo Arduino CLI: Not found in PATH
)
echo.

echo [6/10] Checking Arduino CLI in user bin directory...
if exist "%USERPROFILE%\bin\arduino-cli.exe" (
    echo Arduino CLI found in user bin: %USERPROFILE%\bin\arduino-cli.exe
    "%USERPROFILE%\bin\arduino-cli.exe" version 2>nul
) else (
    echo Arduino CLI: Not found in user bin directory
)
echo.

echo [7/10] Checking user directory permissions...
echo test > "%USERPROFILE%\test_write.tmp" 2>nul
if exist "%USERPROFILE%\test_write.tmp" (
    echo User directory write: OK
    del "%USERPROFILE%\test_write.tmp"
) else (
    echo ERROR: Cannot write to user directory
)
echo.

echo [8/10] Checking temp directory permissions...
echo test > "%TEMP%\test_write.tmp" 2>nul
if exist "%TEMP%\test_write.tmp" (
    echo Temp directory write: OK
    del "%TEMP%\test_write.tmp"
) else (
    echo ERROR: Cannot write to temp directory
)
echo.

echo [9/10] Checking antivirus/security software...
echo NOTE: Some antivirus software may block downloads or execution
echo If you have security software, you may need to:
echo - Temporarily disable real-time protection
echo - Add exceptions for PowerShell and arduino-cli
echo - Allow downloads from arduino.cc
echo.

echo [10/10] Checking execution policy...
powershell -Command "Get-ExecutionPolicy" 2>nul
echo Current PowerShell execution policy (should allow scripts)
echo.

echo ========================================================
echo  Diagnostic Summary
echo ========================================================
echo.

echo If you see errors above, try these solutions:
echo.
echo 1. INTERNET ISSUES:
echo    - Check firewall settings
echo    - Try different network (mobile hotspot)
echo    - Disable VPN temporarily
echo.
echo 2. POWERSHELL ISSUES:
echo    - Run as Administrator
echo    - Update PowerShell: winget install Microsoft.PowerShell
echo    - Set execution policy: Set-ExecutionPolicy RemoteSigned
echo.
echo 3. DOWNLOAD ISSUES:
echo    - Temporarily disable antivirus
echo    - Download manually from https://arduino.cc/en/software
echo    - Use the simple installation script instead
echo.
echo 4. PERMISSION ISSUES:
echo    - Run script as Administrator
echo    - Check user account permissions
echo    - Try running from different location
echo.

echo For manual installation:
echo 1. Download Arduino CLI from https://arduino.cc/en/software
echo 2. Extract to %USERPROFILE%\bin\
echo 3. Add %USERPROFILE%\bin to PATH
echo 4. Run config_libraries_and_boards_simple.bat
echo.

pause
