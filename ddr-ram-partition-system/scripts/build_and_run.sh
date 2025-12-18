#!/bin/bash

# Build script for DDR RAM Partition System

set -e

echo "Building DDR RAM Partition System..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Run CMake and make
cmake ..
make -j$(nproc)

echo "Build complete!"
echo ""
echo "=== Running Tests ==="
echo ""

# Run the test suite
./ddr_test_suite

echo ""
echo "=== Running Main Application ==="
echo ""

# Run the main application
./ddr_ram_system

cd ..
