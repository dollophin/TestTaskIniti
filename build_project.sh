#!/bin/bash

PROJECT_DIR="$(pwd)"

if ! command -v cmake &> /dev/null
then
    echo "CMake is not installed. Please install CMake."
    exit 1
fi

BUILD_DIR="${PROJECT_DIR}/build"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

cmake .. && cmake --build . --clean-first

make

if [ $? -eq 0 ]; then
    echo "The project has been successfully builded!"
else
    echo "An error occurred while building the project!"
    exit 1
fi