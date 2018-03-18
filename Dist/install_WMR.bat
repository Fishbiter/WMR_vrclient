@echo off
if not exist vrclient.dll goto :exit

if exist real_vrclient.dll goto :rename

copy vrclient.dll real_vrclient.dll /Y

IF ERRORLEVEL 1 goto :failed

copy vrclient_x64.dll real_vrclient_x64.dll /Y

IF ERRORLEVEL 1 goto :failed

:rename

copy WMR_vrclient.dll vrclient.dll /Y

IF ERRORLEVEL 1 goto :failed

copy WMR_vrclient_x64.dll vrclient_x64.dll /Y

IF ERRORLEVEL 1 goto :failed

echo Ready to go
pause

goto :exitclean

:exit
echo This should be run in the SteamVR app directory (SteamApps\common\SteamVR\bin)
pause
goto :exitclean

:failed
echo Failed to copy a file - try closing Steam and making sure SteamVR is not running
pause 

:exitclean