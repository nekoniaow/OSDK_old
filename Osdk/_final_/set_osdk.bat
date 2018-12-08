@ECHO OFF
::
:: This simply sets the OSDK environment variable to the current folder,
:: which should be the root of a valid OSDK installation.
::
:: This allows not having to hardcode that variable on the system to a given path.
:: But the downside is that this needs to be executed again after every fresh login.
::

:: Obtain the current path
SET SCRIPTPATH=%~dp0

:: Set OSDK to the path minus the ending slash.
SET OSDK=%SCRIPTPATH:~0,-1%

echo OSDK set to "%OSDK%"
