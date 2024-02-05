#!/bin/bash

# Root directory.
SIERPINSKI_PROJECT_PATH=$(dirname "$(pwd)")

# Scripts directory.
export SIERPINSKI_SCRIPTS_PATH="${SIERPINSKI_PROJECT_PATH}/scripts/linux"

# Binary directory.
export SIERPINSKI_BIN_DIR="${SIERPINSKI_PROJECT_PATH}/bin"

# CMake build files and cache.
export SIERPINSKI_BUILD_DIR="${SIERPINSKI_PROJECT_PATH}/build"
