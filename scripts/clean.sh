#!/bin/bash

source config.sh

# Remove the binary directory.
if [ -d "$SIERPINSKI_BIN_DIR" ]
then
    echo "removing '$SIERPINSKI_BIN_DIR'"
    rm -r "$SIERPINSKI_BIN_DIR"
fi

# Remove the CMake build directory.
if [ -d "$SIERPINSKI_BUILD_DIR" ]
then
    echo "removing '$SIERPINSKI_BUILD_DIR'"
    rm -rf "$SIERPINSKI_BUILD_DIR"
fi
