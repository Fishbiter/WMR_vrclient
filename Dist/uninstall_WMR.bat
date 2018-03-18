@echo off
if not exist real_vrclient.dll goto :exit

copy real_vrclient.dll vrclient.dll /Y
copy real_vrclient_x64.dll vrclient_x64.dll /Y

echo Uninstalled
pause

goto :exitclean

:exit
echo This should be run in the SteamVR app directory (SteamApps\common\SteamVR\bin)
pause

:exitclean