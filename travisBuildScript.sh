#!/bin/sh
set -e

mkdir $BUILD_DIR
cd $BUILD_DIR

CMAKE_FLAGS="-DCMAKE_CXX_COMPILER=$COMPILER"

if [ -n "$COMPILER_FLAGS" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DCMAKE_CXX_FLAGS=$COMPILER_FLAGS"
fi

if [ "$BUILD_DOCUMENTATION" = "true" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DTULPAR_BUILD_DOCUMENTATION=ON"
fi

cmake $CMAKE_FLAGS ..

if [ "$BUILD_DOCUMENTATION" != "true" ]; then
    make -j${nproc}
    ctest -VV
else
    make docs
fi
