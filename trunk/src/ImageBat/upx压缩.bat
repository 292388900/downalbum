set upxfile="H:\360����\��ȫ\�ӿ����\upx202w\upx.exe"
set rar="C:\Program Files\WinRAR\winrar.exe"

set nowpath=%~dp0
set srcpath=%nowpath%bin
set outpath=F:\softhao\soft

%upxfile% -9kf -o "%outpath%\��Ƭ��������.exe" "%srcpath%\��Ƭ��������.exe"
%upxfile% -9kf -o %outpath%\��Ƭ��������2.exe %srcpath%\��Ƭ��������2.exe

%upxfile% -9kf -o %outpath%\�Ա�ͼƬ����.exe %srcpath%\�Ա�ͼƬ����.exe
%upxfile% -9kf -o %outpath%\�Ա�ͼƬ����2.exe %srcpath%\�Ա�ͼƬ����2.exe

copy %nowpath%imagebat.html "%outpath%\readme.html

cd /d	  %outpath%
%rar% a   %outpath%\ImageBat.rar ��Ƭ��������.exe
%rar% a   %outpath%\ImageBat.rar readme.html

%rar% a   %outpath%\ImageBat2.rar ��Ƭ��������2.exe
%rar% a   %outpath%\ImageBat2.rar readme.html

%rar% a   %outpath%\taobaoBat.rar �Ա�ͼƬ����.exe
%rar% a   %outpath%\taobaoBat.rar readme.html

%rar% a   %outpath%\taobaoBat2.rar �Ա�ͼƬ����2.exe
%rar% a   %outpath%\taobaoBat2.rar readme.html


del "%outpath%\readme.html

pause
