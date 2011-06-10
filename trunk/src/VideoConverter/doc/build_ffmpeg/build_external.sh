tar zxfv zlib-1.2.3.tar.gz
cd zlib-1.2.3
./configure --prefix=/mingw
make
make install
cd ..
echo =================
echo zlib ok.
echo =================



tar zxfv bzip2-1.0.5.tar.gz
cd bzip2-1.0.5
patch -p0 < ../bzip2-1.0.5-extensions.diff
make
make install PREFIX=/mingw
cd ..
echo =================
echo bzip2 ok.
echo =================


tar zxfv lame-398-2.tar.gz
cd lame-398-2
patch -p0 < ../lame.diff
./configure --prefix=/mingw --disable-shared --enable-static --disable-frontend --enable-nasm
make
make install
cd ..
echo =================
echo mp3lame ok.
echo =================


tar xfvj xvidcore-1.2.1.tar.bz2
cd xvidcore/build/generic
./configure --prefix=/mingw
make
make install
rm /mingw/lib/xvidcore.dll
mv /mingw/lib/xvidcore.a /mingw/lib/libxvidcore.a
cd ..
cd ..
cd ..
echo =================
echo xvidcore ok.
echo =================



tar zxfv libogg-1.1.3.tar.gz
cd libogg-1.1.3
./configure --prefix=/mingw --enable-static --disable-shared
make
make install
cd ..
echo =================
echo libogg ok.
echo =================



tar zxfv libvorbis-1.2.0.tar.gz
cd libvorbis-1.2.0
./configure --prefix=/mingw --enable-static --disable-shared
make
make install
cd ..
echo =================
echo libvorbis ok.
echo =================


tar xfvj libtheora-1.0.tar.bz2
cd libtheora-1.0
./configure --prefix=/mingw --enable-static --disable-shared
make
make install
cd ..
echo =================
echo libtheora ok.
echo =================


tar zxfv speex-1.2rc1.tar.gz
cd speex-1.2rc1
./configure --prefix=/mingw --enable-static --disable-shared
make
make install
cd ..
echo =================
echo speex ok.
echo =================


tar zxfv faad2-2.6.1.tar.gz
patch -p0 < faad2-2.6.1.patch
cd faad2
sh bootstrap
./configure --prefix=/mingw --enable-static --disable-shared
make LDFLAGS="-no-undefined"
make install
cd ..
echo =================
echo faad2 ok.
echo =================


tar zxfv faac-1.26.tar.gz
patch -p0 < faac-1.26.diff
cd faac
sh bootstrap
./configure --prefix=/mingw --enable-static --disable-shared
make LDFLAGS="-no-undefined"
make install
cd ..
echo =================
echo faac ok.
echo =================


tar xfvj amrnb-7.0.0.2.tar.tar
cd amrnb-7.0.0.2
./configure --prefix=/mingw --enable-static --disable-shared
make
make install
cd ..
echo =================
echo amr_nb ok.
echo =================


tar xfvj amrwb-7.0.0.3.tar.tar
cd amrwb-7.0.0.3
./configure --prefix=/mingw --enable-static --disable-shared
make
make install
cd ..
echo =================
echo amr_wb ok.
echo =================


tar zxfv dirac-1.0.2.tar.gz
cd dirac-1.0.2
./configure --prefix=/mingw --enable-static --disable-shared
make
make install
cd ..
echo =================
echo libdirac ok.
echo =================



tar zxfv gsm-1.0.12.tar.gz
patch -p0 < gsm_1.0-pl12_01.diff
cd gsm-1.0-pl12/
make
cp lib/libgsm.a /mingw/lib/
cp inc/gsm.h /mingw/include/
cd ..
echo =================
echo libgsm ok.
echo =================


cd x264
./configure --prefix=/mingw
make
make install
cd ..
echo =================
echo libx264 ok.
echo =================



tar zxfv liboil-0.3.15.tar.gz
cd liboil-0.3.15
./configure --prefix=/mingw --enable-static --disable-shared
make
make install
cd ..
rm -rf liboil-0.3.15
tar zxfv liboil-0.3.15.tar.gz
cd liboil-0.3.15
./configure --prefix=/mingw --enable-static --enable-shared
make
cp liboil/.libs/liboil-0.3.a /mingw/lib/
cd ..
echo =================
echo liboil ok.
echo =================



tar zxfv schroedinger-1.0.5.tar.gz
cd schroedinger-1.0.5
./configure --prefix=/mingw --enable-static --disable-shared
make
make install
cd ..
echo =================
echo libschroedinger ok.
echo =================



