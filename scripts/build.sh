#!/bin/bash

source config.sh

Main()
{
    # Create the build directory if it does not already exist.
    mkdir -pv "$SIERPINSKI_BUILD_DIR"

    # Set the build type according to the first program arg if any.
    BUILD_TYPE="Release"
    if [ -n "$1" ]
    then
        BUILD_TYPE=$1
    fi

    pushd "$SIERPINSKI_BUILD_DIR" > /dev/null || exit
    cmake ../ \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DBUILD_DOCS=ON \
        -DCMAKE_INSTALL_PREFIX="$SIERPINSKI_BIN_DIR" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" && \
        make -j"$(nproc)" all && \
        make install

    # Exit if any of the above commands fails.
    if [ $? -ne 0 ];
    then
        exit 1
    fi
    popd > /dev/null || exit
}

Main "$1"
