rm -rf build
mkdir build
cd build
../ffmpeg/configure --extra-cflags=-fno-common --enable-memalign-hack \
               --enable-avisynth \
               --enable-bzlib \
               --enable-libopencore-amrnb \
               --enable-libfaac \
               --enable-libfaad \
               --enable-libgsm \
               --enable-libmp3lame \
               --enable-libschroedinger \
               --enable-libspeex \
               --enable-libtheora \
               --enable-libvorbis \
               --enable-libx264 \
               --enable-libxvid \
               --enable-zlib \
               --disable-static --enable-shared \
               --enable-gpl --enable-version3 --enable-nonfree \
               --enable-runtime-cpudetect \
               --enable-avfilter --disable-network --disable-ipv6 \
               --disable-ffserver
