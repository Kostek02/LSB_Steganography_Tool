# Sample Images and Test Files

This directory contains sample images and test files for the LSB Steganography Tool v1.1.

## 📁 File Structure

### **Sample Images (Clean)**
These are clean images that you can use to hide your own messages:

- `image.bmp` - Original 24-bit BMP image (100x100 pixels)
- `sample.bmp` - Sample BMP image (100x100 pixels)
- `sample.png` - Sample PNG image (100x100 pixels)
- `sample.jpg` - Sample JPEG image (100x100 pixels, quality 90)

### **Test Files (With Hidden Messages)**
These files contain hidden messages that you can extract to test the tool:

- `test_bmp_with_message.bmp` - Contains: "v1.1 Message: Welcome to LSB Steganography Tool! This is a test message hidden in a BMP image."
- `test_png_with_message.png` - Contains: "v1.1 PNG"
- `test_jpeg_with_message.jpg` - Contains: "v1.1 JPEG"

## 🧪 How to Use These Files

### **Testing Message Extraction**
To test if the tool can extract hidden messages:

```bash
# Extract message from BMP test file
./steg_cli -x -i samples/test_bmp_with_message.bmp

# Extract message from PNG test file
./steg_cli -x -i samples/test_png_with_message.png

# Extract message from JPEG test file
./steg_cli -x -i samples/test_jpeg_with_message.jpg
```

### **Hiding Your Own Messages**
To hide your own messages in the clean sample images:

```bash
# Hide message in BMP image
./steg_cli -e -m "Your secret message here" -i samples/sample.bmp -o your_output.bmp

# Hide message in PNG image
./steg_cli -e -m "Your secret message here" -i samples/sample.png -o your_output.png

# Hide message in JPEG image
./steg_cli -e -m "Your secret message here" -i samples/sample.jpg -o your_output.jpg
```

### **Checking Image Capacity**
To see how much data can be hidden in each image:

```bash
# Check BMP capacity
./steg_cli -c -i samples/sample.bmp

# Check PNG capacity
./steg_cli -c -i samples/sample.png

# Check JPEG capacity
./steg_cli -c -i samples/sample.jpg
```

## 📊 Format Support Status

### **BMP (24-bit uncompressed)**
- ✅ **Fully Supported** - Complete LSB steganography implementation
- ✅ **High Capacity** - Can hide large messages
- ✅ **Perfect Recovery** - 100% message extraction accuracy

### **PNG (Lossless)**
- ✅ **Fully Supported** - Complete LSB steganography implementation
- ✅ **Format Detection** - Recognizes PNG files correctly
- ✅ **LSB Embedding** - Embeds messages in IDAT chunk data
- ✅ **Message Extraction** - Extracts messages from PNG files

### **JPEG (Lossy)**
- ✅ **Fully Supported** - Complete LSB steganography implementation
- ✅ **Format Detection** - Recognizes JPEG files correctly
- ✅ **LSB Embedding** - Embeds messages in image data
- ✅ **Message Extraction** - Extracts messages from JPEG files

## 🎯 Expected Results

### **BMP Test File**
```bash
$ ./steg_cli -x -i samples/test_bmp_with_message.bmp
Extracted message: "v1.1 Message: Welcome to LSB Steganography Tool! This is a test message hidden in a BMP image."
```

### **PNG Test File**
```bash
$ ./steg_cli -x -i samples/test_png_with_message.png
Extracted message: "v1.1 PNG"
```

### **JPEG Test File**
```bash
$ ./steg_cli -x -i samples/test_jpeg_with_message.jpg
Extracted message: "v1.1 JPEG"
```

## 📝 Notes

- **All formats** now work perfectly with full LSB steganography
- **BMP files** provide the highest capacity and perfect recovery
- **PNG files** support lossless compression with good capacity
- **JPEG files** support lossy compression with moderate capacity
- All files are **100x100 pixels** for consistent testing
- The **original `image.bmp`** is preserved for backward compatibility
- **Sample images** are created fresh for each format

## 🔧 Technical Details

- **Image Size**: 100x100 pixels
- **Color Depth**: 24-bit RGB
- **File Formats**: BMP (uncompressed), PNG (lossless), JPEG (lossy, quality 90)
- **Message Encoding**: LSB (Least Significant Bit) steganography
- **Capacity**: Varies by format and image size

---

**Happy Steganography Testing! 🔐✨** 