set upxfile="H:\360云盘\安全\加壳软件\upx202w\upx.exe"
set rar="C:\Program Files\WinRAR\winrar.exe"

set nowpath=%~dp0
set srcpath=%nowpath%bin
set outpath=F:\softhao\soft

%upxfile% -9kf -o "%outpath%\照片批量助手.exe" "%srcpath%\照片批量助手.exe"
%upxfile% -9kf -o %outpath%\照片批量助手2.exe %srcpath%\照片批量助手2.exe

%upxfile% -9kf -o %outpath%\淘宝图片助手.exe %srcpath%\淘宝图片助手.exe
%upxfile% -9kf -o %outpath%\淘宝图片助手2.exe %srcpath%\淘宝图片助手2.exe

copy %nowpath%imagebat.html "%outpath%\readme.html

cd /d	  %outpath%
%rar% a   %outpath%\ImageBat.rar 照片批量助手.exe
%rar% a   %outpath%\ImageBat.rar readme.html

%rar% a   %outpath%\ImageBat2.rar 照片批量助手2.exe
%rar% a   %outpath%\ImageBat2.rar readme.html

%rar% a   %outpath%\taobaoBat.rar 淘宝图片助手.exe
%rar% a   %outpath%\taobaoBat.rar readme.html

%rar% a   %outpath%\taobaoBat2.rar 淘宝图片助手2.exe
%rar% a   %outpath%\taobaoBat2.rar readme.html


del "%outpath%\readme.html

pause
