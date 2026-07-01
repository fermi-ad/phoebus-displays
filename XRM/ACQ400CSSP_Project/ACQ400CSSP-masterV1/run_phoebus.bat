@echo off
setlocal EnableDelayedExpansion

:: Start phoebus using ACQ400CSSP helper script

set "ROOT_DIR=%~dp0"
set "ROOT_DIR=%ROOT_DIR:~0,-1%"

:: Get UUT
set UUT=%1
if not defined UUT (
    echo.
    echo [ACQ400CSSP]
    set /p UUT="Connect to UUT : "
)
if "%UUT:~8%"=="" (
    echo "Invalid hostname"
    exit /b 1
)

:: CHANGE ME
set "PHOEBUS_JAR=%ROOT_DIR%\phoebus-4.7.4-SNAPSHOT\product-4.7.4-SNAPSHOT.jar"

:: Vars
set "JAVA_EXE=java"
::set "JAVA_EXE=C:\Program Files\Eclipse Adoptium\jre-17.0.12.7-hotspot\bin\java.exe" & :: Set full java path if multiple version installed
set "SETTINGS_BASE=%ROOT_DIR%\src\settings_base.ini"
set "LAUNCHER=%ROOT_DIR%\src\acq400_launcher.bob"
set "RESOURCE=%LAUNCHER%"
set "WORKSPACE=%ROOT_DIR%\workspaces\%UUT%"
set "JAVA_PREFS=-Dphoebus.user=%WORKSPACE% -Dphoebus.folder.name.preference= "
set "SETTINGS=%WORKSPACE%\settings.ini"
set "MEMENTO=%WORKSPACE%\memento"

:: Attempt to locate a phobus jar if not found
if exist "%PHOEBUS_JAR%" (
    goto :end
) 
set "PHOEBUS_JAR="
for /r "%ROOT_DIR%" %%F in (*.jar) do (
    set "PHOEBUS_JAR=%%F"
    goto :found
)
echo "Error: Unable to locate Phoebus jar"
exit /b 1
:found
if defined PHOEBUS_JAR (
    echo Found Phoebus jar: "!PHOEBUS_JAR!"
)
:end

:: If ioc_addr.conf or acq2106_000_addr.conf files exist use contents for ADDR_LIST
set "ADDR_LIST="
if exist "%ROOT_DIR%\ioc_addr.conf" (
    set /p ADDR_LIST=<"%ROOT_DIR%\ioc_addr.conf"
)
if exist "%ROOT_DIR%\%UUT%_addr.conf" (
    set /p ADDR_LIST=<"%ROOT_DIR%\%UUT%_addr.conf"
)

if defined ADDR_LIST (
    set "JAVA_PREFS=%JAVA_PREFS% -Djca.use_env=true"
    set "EPICS_PVA_ADDR_LIST=%ADDR_LIST%"
    set "EPICS_CA_ADDR_LIST=%ADDR_LIST%"
    echo "Using addr list: %ADDR_LIST%"
)

:: Init workspace if needed
if not exist "%MEMENTO%" (
    echo Creating new workspace %WORKSPACE%
    mkdir "%WORKSPACE%" 2>nul
    copy "%SETTINGS_BASE%" "%SETTINGS%" >nul
    echo org.csstudio.display.builder.model/macros=^<UUT^>%UUT%^</UUT^> >> "%SETTINGS%"
    set TARGET=-resource "%RESOURCE%" -layout null
) else (
    echo Using existing workspace %WORKSPACE%
    set TARGET=-layout "%MEMENTO%"
)

:: Run
set CMD="%JAVA_EXE%" %JAVA_PREFS% -jar "%PHOEBUS_JAR%" -nosplash -settings "%SETTINGS%" %TARGET%
echo Running cmd: %CMD%
%CMD% >nul 2>&1
echo "Done"