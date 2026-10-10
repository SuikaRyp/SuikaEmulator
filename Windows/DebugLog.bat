@echo off
set LOGFILE=suikaemulatorlog.txt

del "%LOGFILE%" 2> NUL
if exist SuikaEmulatorWindows64.exe (
    start SuikaEmulatorWindows64.exe --log="%LOGFILE%" -d
    goto exit
)
if exist SuikaEmulatorWindows.exe (
    start SuikaEmulatorWindows.exe --log="%LOGFILE%" -d
    goto exit
)

echo Unable to find SuikaEmulatorWindows.exe.
pause

:exit
