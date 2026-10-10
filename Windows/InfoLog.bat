@echo off
set LOGFILE=suikaemulatorlog.txt

del "%LOGFILE%" 2> NUL
if exist SuikaEmulatorWindows64.exe (
    SuikaEmulatorWindows64.exe --log="%LOGFILE%"
    goto exit
)
if exist SuikaEmulatorWindows.exe (
    SuikaEmulatorWindows.exe --log="%LOGFILE%"
    goto exit
)

echo Unable to find SuikaEmulatorWindows.exe.
pause

:exit