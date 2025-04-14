#!/bin/bash

PROJECT_DIR="$(pwd)"
BUILD_DIR="${PROJECT_DIR}/build/tests"

if ! command -v cmake &> /dev/null; then
    echo "CMake is not installed. Please install it first."
    exit 1
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

cmake -S ../.. -B build-debug -DCMAKE_BUILD_TYPE=Debug
if [ $? -ne 0 ]; then
    echo "CMake configuration for tests failed!"
    exit 1
fi

cmake --build build-debug --clean-first
if [ $? -ne 0 ]; then
    echo "Test build failed!"
    exit 1
fi