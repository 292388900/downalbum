set nowpath=%~dp0 
set rar="C:\Program Files\WinRAR\rar.exe"
 
set sTime=%date:~0,10%_

%rar%  a %sTime%ImageBat *.rc *.sln *.vcproj *.bat *.h *.cpp *.txt res\*.* doc\*.* images\*.*
