Go to http://arrozcru.no-ip.org/ffmpeg_wiki for the latest build instructions.

Build system:

gcc 4.2.4
nasm 2.05.01
yasm 0.7.2
w32api 3.12
mingw-runtime 3.15.1
binutils 2.18.50


Build instructions:

zlib 1.2.3
    http://www.zlib.net/

$ tar zxfv zlib-1.2.3.tar.gz
$ cd zlib-1.2.3
$ ./configure --prefix=/mingw
$ make
$ make install

pthreads-win32 2.8.0
    ftp://sources.redhat.com/pub/pthreads-win32

$ tar zxfv pthreads-w32-2-8-0-release.tar.gz
$ patch -p0 < pthreads-w32-2-8-0.diff
$ cd pthreads-w32-2-8-0-release
$ make clean GC-static
$ cp libpthreadGC2.a /mingw/lib
$ cp pthread.h sched.h /mingw/include

libfaac 1.26
    http://www.audiocoding.com/

$ tar zxfv faac-1.26.tar.gz
$ patch -p0 < faac-1.26.diff
$ cd faac
$ sh bootstrap
$ ./configure --prefix=/mingw --enable-static --disable-shared
$ make
$ make install

lame-398-2.tar.gz
    http://www.mp3dev.org/

$ tar zxfv lame-398-2.tar.gz
$ cd lame-398-2
$ patch -p0 < ../lame-398-2.diff
$ ./configure --prefix=/mingw --disable-shared --enable-static --disable-frontend --enable-nasm
$ make
$ make install

libogg 1.1.3
    http://www.xiph.org/

$ tar zxfv libogg-1.1.3.tar.gz
$ cd libogg-1.1.3
$ ./configure --prefix=/mingw --enable-static --disable-shared
$ make
$ make install

libvorbis 1.2.0
    http://www.xiph.org/

$ tar zxfv libvorbis-1.2.0.tar.gz
$ cd libvorbis-1.2.0
$ ./configure --prefix=/mingw --enable-static --disable-shared
$ make
$ make install

libtheora 1.0
    http://www.xiph.org/

$ tar xfvj libtheora-1.0.tar.bz2
$ cd libtheora-1.0
$ ./configure --prefix=/mingw --enable-static --disable-shared
$ make
$ make install

speex-1.2rc1.tar.gz
    http://www.xiph.org/

$ tar zxfv speex-1.2rc1.tar.gz
$ cd speex-1.2rc1
$ ./configure --prefix=/mingw --enable-static --disable-shared
$ make
$ make install

libgsm 1.0.12
    http://kbs.cs.tu-berlin.de/~jutta/toast.html

$ tar zxfv gsm-1.0.12.tar.gz
$ patch -p0 < gsm_1.0-pl12.diff
$ cd gsm-1.0-pl12
$ make
$ cp lib/libgsm.a /mingw/lib/
$ cp inc/gsm.h /mingw/include/

xvidcore 1.2.0
    http://www.xvid.org

$ tar xfvj xvidcore-1.2.0.tar.bz2
$ patch -p0 < xvidcore-1.2.0
$ cd xvidcore/build/generic
$ ./configure --prefix=/mingw
$ make
$ make install
$ rm /mingw/lib/xvidcore.dll
$ mv /mingw/lib/xvidcore.a /mingw/lib/libxvidcore.a

x264 0.65.1042M 5df2a71
    http://developers.videolan.org/x264.html

$ git clone git://git.videolan.org/x264.git x264
$ cd x264
$ ./configure --prefix=/mingw
$ make
$ make install

liboil-0.3.15.tar.gz
    http://liboil.freedesktop.org

$ tar zxfv liboil-0.3.15.tar.gz
$ cd liboil-0.3.15
$ ./configure --prefix=/mingw --enable-static --disable-shared
$ make
$ make install
$ cd ..
$ rm -rf liboil-0.3.15
$ tar zxfv liboil-0.3.15.tar.gz
$ cd liboil-0.3.15
$ ./configure --prefix=/mingw --enable-static --enable-shared
$ make
$ cp liboil/.libs/liboil-0.3.a /mingw/lib/

schroedinger-1.0.5.tar.gz
    http://diracvideo.org

$ tar zxfv schroedinger-1.0.5.tar.gz
$ cd schroedinger-1.0.5
$ ./configure --prefix=/mingw --enable-static --disable-shared
$ make
$ make install

ffmpeg r15986
    http://www.ffmpeg.org

$ svn co svn://svn.ffmpeg.org/ffmpeg/trunk ffmpeg -r 15986
$ cd ffmpeg
$ patch -p0 < ../ffmpeg_r15986.diff
$ cd ..
$ mkdir build
$ cd build
$ ../ffmpeg/configure --extra-cflags=-fno-common --enable-memalign-hack \
               --enable-pthreads --enable-libmp3lame --enable-libxvid \
               --enable-libvorbis --enable-libtheora --enable-libspeex \
               --enable-libfaac --enable-libgsm --enable-libx264 \
               --enable-libschroedinger --enable-avisynth --enable-swscale \
               --enable-gpl --enable-shared --disable-static



/////////////////////////////////////////////////////////////////////

AC3 support : --enable-a52 --enable-gpl
3GP support : --enable-amr_nb --enable-amr_wb


--enable-nonfree
--enable-postproc




--cpu=prescott
--enable-avfilter --enable-avfilter-lavf 



http://ffmpeg.arrozcru.org/wiki/index.php?title=Shared