#!/bin/bash

PROJECT_DIR="$(pwd)"
BUILD_DIR="${PROJECT_DIR}/build/tests"

if ! command -v cmake &> /dev/null; then
    echo "CMake is not installed. Please install it first."
    exit 1
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

echo "Configuring project..."
cmake -S ../.. -B . -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]; then
    echo "CMake configuration for tests failed!"
    exit 1
fi

echo "Building project..."
cmake --build . --clean-first
if [ $? -ne 0 ]; then
    echo "Test build failed!"
    exit 1
fi

echo "Running tests with CTest..."
ctest --output-on-failure
