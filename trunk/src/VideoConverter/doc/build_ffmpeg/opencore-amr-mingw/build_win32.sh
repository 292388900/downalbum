#!/bin/sh

export PREFIX=/mingw
export BUILD_AS_C=1
export CC=gcc
export CFLAGS="-O3"
export CXXFLAGS="$CFLAGS"

make -C amrnb clean
make -C amrnb "$@" || exit 1

make -C amrwb clean
make -C amrwb "$@" || exit 1

make -C amrnb install || exit 1

make -C amrwb install || exit 1

