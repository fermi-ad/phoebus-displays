@echo off

set "BASEDIR=%~dp0"

set "ICON=%BASEDIR%ACQ400CSSP_icon.ico"
set "LAUNCHER=%BASEDIR%ACQ400CSSP_launcher.py"
set "SHORTCUT=%BASEDIR%ACQ400CSSP.lnk"

powershell -Command ^
    "$WS = New-Object -ComObject WScript.Shell;" ^
    "$S = $WS.CreateShortcut('%SHORTCUT%');" ^
    "$S.TargetPath = 'python3';" ^
    "$S.IconLocation = '%ICON%';" ^
    "$S.Arguments = '"%LAUNCHER%"';" ^
    "$S.Save()"
