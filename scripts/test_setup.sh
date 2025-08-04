#!/bin/bash

# LSB Steganography Tool - Test Setup Script
# This script helps create a test BMP file and run the tool

echo "LSB Steganography Tool - Test Setup"
echo "==================================="
echo

# Check if ImageMagick is available
if command -v convert &> /dev/null; then
    echo "✓ ImageMagick found - creating test BMP file..."
    
    # Create a simple test image (100x100 pixels, 24-bit color)
    convert -size 100x100 xc:lightblue -depth 8 image.bmp
    
    if [ $? -eq 0 ]; then
        echo "✓ Test image created: image.bmp (100x100 pixels, 24-bit)"
        echo "  Capacity: ~12,500 characters"
        echo
        
        # Build and test the tool
        echo "Building and testing the tool..."
        make clean
        make
        
        if [ $? -eq 0 ]; then
            echo
            echo "Running the tool..."
            ./steg
        else
            echo "✗ Build failed"
            exit 1
        fi
    else
        echo "✗ Failed to create test image"
        exit 1
    fi
    
elif command -v python3 &> /dev/null; then
    echo "✓ Python3 found - creating test BMP file using PIL..."
    
    # Create BMP using Python PIL
    python3 -c "
from PIL import Image
import numpy as np

# Create a 100x100 test image with safe color values
img_array = np.zeros((100, 100, 3), dtype=np.uint8)
for i in range(100):
    for j in range(100):
        # Use safe values that won't overflow uint8 (0-255)
        r = min(i * 2, 255)
        g = min(j * 2, 255)
        b = min((i + j) // 2, 255)
        img_array[i, j] = [r, g, b]

img = Image.fromarray(img_array)
img.save('image.bmp')
print('Test image created: image.bmp (100x100 pixels, 24-bit)')
print('Capacity: ~12,500 characters')
"
    
    if [ $? -eq 0 ]; then
        echo
        echo "Building and testing the tool..."
        make clean
        make
        
        if [ $? -eq 0 ]; then
            echo
            echo "Running the tool..."
            ./steg
        else
            echo "✗ Build failed"
            exit 1
        fi
    else
        echo "✗ Failed to create test image with Python"
        exit 1
    fi
    
else
    echo "✗ Neither ImageMagick nor Python3 found"
    echo
    echo "To create a test BMP file, you can:"
    echo "1. Install ImageMagick: brew install imagemagick (macOS) or apt-get install imagemagick (Ubuntu)"
    echo "2. Install Python3 with PIL: pip install pillow"
    echo "3. Use GIMP, Photoshop, or any image editor to create a 24-bit BMP file"
    echo "4. Download a sample 24-bit BMP file from the internet"
    echo
    echo "The file must be named 'image.bmp' and placed in this directory."
    echo "Recommended size: 100x100 pixels or larger for testing."
    exit 1
fi

echo
echo "Test completed!"
echo "Files created:"
echo "  - image.bmp (original test image)"
echo "  - output.bmp (image with hidden message)"
echo "  - steg (executable)"

# Ask if user wants to clean up
echo
read -p "Clean up generated files? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Cleaning up..."
    rm -f output.bmp
    make clean
    echo "✓ Cleanup complete"
else
    echo "Files preserved for inspection"
fi 